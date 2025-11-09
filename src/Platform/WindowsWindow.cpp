#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#endif

#include <glad/glad.h>   // include AFTER Windows.h on Win32

#include "Hazel/Platform/WindowsWindow.h"

#include "Hazel/Core/Events/ApplicationEvent.h" // WindowCloseEvent, WindowResizeEvent
#include "Hazel/Core/Events/KeyEvent.h"
#include "Hazel/Core/Events/MouseEvent.h"


#include <glad/glad.h>     // if you use GL
#include <cassert>
#include "Hazel/Core/Log.h"

// Simple error check macro
static void SDLErrorCheck(const char* where)
{
    const char* err = SDL_GetError();
    if (err && *err)
    {
        HZ_CORE_ERROR("[SDL] {}: {}", where, err);
        SDL_ClearError();
    }
}

namespace Hazel {

    // ---------------- Callback storage helpers ----------------

    void SetWindowCloseCallback(::SDL_Window* window, std::function<void(::SDL_Window*)> cb)
    {
        // store a heap-allocated function object
        auto* heapFn = new std::function<void(::SDL_Window*)>(std::move(cb));
        // free old one if present
        if (auto* oldPtr = static_cast<std::function<void(::SDL_Window*)>*>(SDL_GetWindowData(window, kCloseCbKey)))
            delete oldPtr;

        SDL_SetWindowData(window, kCloseCbKey, static_cast<void*>(heapFn));
    }

    void SetWindowSizeCallback(::SDL_Window* window, std::function<void(::SDL_Window*, int, int)> cb)
    {
        auto* heapFn = new std::function<void(::SDL_Window*, int, int)>(std::move(cb));
        if (auto* oldPtr = static_cast<std::function<void(::SDL_Window*, int, int)>*>(SDL_GetWindowData(window, kSizeCbKey)))
            delete oldPtr;

        SDL_SetWindowData(window, kSizeCbKey, static_cast<void*>(heapFn));
    }

    // Install a single global event watcher once
    static void EnsureGlobalWatcherInstalled()
    {
        static bool watcherInstalled = false;
        if (watcherInstalled) return;

        SDL_AddEventWatch([](void*, SDL_Event* e) -> int
            {
                if (e->type == SDL_WINDOWEVENT)
                {
                    ::SDL_Window* win = SDL_GetWindowFromID(e->window.windowID);
                    if (!win) return 0;

                    switch (e->window.event)
                    {
                    case SDL_WINDOWEVENT_CLOSE:
                        if (auto* fn = static_cast<std::function<void(::SDL_Window*)>*>(SDL_GetWindowData(win, kCloseCbKey)))
                            (*fn)(win);
                        break;
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                    {
                        int w = 0, h = 0;
                        SDL_GetWindowSize(win, &w, &h);
                        if (auto* fn = static_cast<std::function<void(::SDL_Window*, int, int)>*>(SDL_GetWindowData(win, kSizeCbKey)))
                            (*fn)(win, w, h);
                        break;
                    }
                    default: break;
                    }
                }
                return 0;
            }, nullptr);

        watcherInstalled = true;
    }

    // ---------------- WindowsWindow ----------------

    // Factory: create the platform window (raw pointer per your signature)
    Window* Window::Create(const WindowProps& props)
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        if (!s_SDLInitialized)
        {
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) != 0)
            {
                SDLErrorCheck("SDL_Init");
                assert(false && "SDL_Init failed");
            }
            s_SDLInitialized = true;
        }

        // GL attributes if you use GL
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        const Uint32 flags =
            SDL_WINDOW_OPENGL |
            SDL_WINDOW_RESIZABLE |
            SDL_WINDOW_ALLOW_HIGHDPI;

        m_Window = SDL_CreateWindow(
            m_Data.Title.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            static_cast<int>(m_Data.Width),
            static_cast<int>(m_Data.Height),
            flags);

        if (!m_Window)
        {
            SDLErrorCheck("SDL_CreateWindow");
            assert(false && "SDL_CreateWindow failed");
        }

        // Attach Hazel window data (so other code can fetch our WindowData*)
        SDL_SetWindowData(m_Window, kHazelDataKey, static_cast<void*>(&m_Data));

        // Create GL context
        m_GLContext = SDL_GL_CreateContext(m_Window);
        if (!m_GLContext)
        {
            SDLErrorCheck("SDL_GL_CreateContext");
            assert(false && "SDL_GL_CreateContext failed");
        }

        // Load GL
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            assert(false && "Failed to initialize GLAD");
        }

        SDL_GL_MakeCurrent(m_Window, m_GLContext);
        SDL_GL_SetSwapInterval(1); // vsync on by default
        m_Data.VSync = true;

        EnsureGlobalWatcherInstalled();

        // Wire callbacks into SDL window data
        SetWindowCloseCallback(m_Window, [this](::SDL_Window*)
            {
                WindowCloseEvent ev;
                if (m_Data.EventCallback) m_Data.EventCallback(ev);
            });

        SetWindowSizeCallback(m_Window, [this](::SDL_Window*, int w, int h)
            {
                m_Data.Width = static_cast<unsigned>(w);
                m_Data.Height = static_cast<unsigned>(h);
                WindowResizeEvent ev(w, h);
                if (m_Data.EventCallback) m_Data.EventCallback(ev);
            });

    }

    void WindowsWindow::Shutdown()
    {
        if (m_Window)
        {
            // Clean stored callbacks
            if (auto* fnClose = static_cast<std::function<void(::SDL_Window*)>*>(SDL_GetWindowData(m_Window, kCloseCbKey)))
            {
                delete fnClose; SDL_SetWindowData(m_Window, kCloseCbKey, nullptr);
            }
            if (auto* fnSize = static_cast<std::function<void(::SDL_Window*, int, int)>*>(SDL_GetWindowData(m_Window, kSizeCbKey)))
            {
                delete fnSize; SDL_SetWindowData(m_Window, kSizeCbKey, nullptr);
            }

            SDL_SetWindowData(m_Window, kHazelDataKey, nullptr);

            if (m_GLContext)
            {
                SDL_GL_DeleteContext(m_GLContext);
                m_GLContext = nullptr;
            }

            SDL_DestroyWindow(m_Window);
            m_Window = nullptr;
        }
    }

    void WindowsWindow::OnUpdate()
    {
        // Pump events elsewhere if you prefer
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                WindowCloseEvent ev;
                if (m_Data.EventCallback) m_Data.EventCallback(ev);
            }
        }

        SDL_GL_SwapWindow(m_Window);
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        SDL_GL_SetSwapInterval(enabled ? 1 : 0);
        m_Data.VSync = enabled;
    }

} // namespace Hazel

