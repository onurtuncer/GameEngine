#pragma once

#include <string>
#include <functional>
#include <cstdint>
#include <SDL.h>          // global namespace SDL
#include <SDL_video.h>

#include "Hazel/Core/Window.h"     // your base Window + WindowProps
#include "Hazel/Core/Events/Event.h"

namespace Hazel {

    class WindowsWindow : public Window
    {
    public:
        explicit WindowsWindow(const WindowProps& props);
        ~WindowsWindow() override;

        void OnUpdate() override;

        unsigned int GetWidth()  const override { return m_Data.Width; }
        unsigned int GetHeight() const override { return m_Data.Height; }

        // Window attributes
        void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override { return m_Data.VSync; }

        void* GetNativeWindow() const override { return m_Window; } // or SDL_Window* if your interface allows

    private:
        void Init(const WindowProps& props);
        void Shutdown();

    private:
        struct WindowData
        {
            std::string Title;
            unsigned int Width = 0;
            unsigned int Height = 0;
            bool VSync = false;

            EventCallbackFn EventCallback;
        };

    private:
        inline static bool s_SDLInitialized = false;

        ::SDL_Window* m_Window = nullptr;   // NOTE: global namespace
        SDL_GLContext m_GLContext = nullptr;
        WindowData    m_Data;
    };

    // Small utility keys used with SDL_(Get|Set)WindowData
    inline constexpr const char* kHazelDataKey = "hazel_data";
    inline constexpr const char* kCloseCbKey = "window_close_callback";
    inline constexpr const char* kSizeCbKey = "window_size_callback";

    // Helpers to stash callbacks into SDL window data
    void SetWindowCloseCallback(::SDL_Window* window, std::function<void(::SDL_Window*)> cb);
    void SetWindowSizeCallback(::SDL_Window* window, std::function<void(::SDL_Window*, int, int)> cb);

} // namespace Hazel
