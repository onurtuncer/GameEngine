#include "Hazel/Platform/WindowsWindow.h"

#include "Hazel/Core/Log.h"

namespace Hazel {

	static bool s_SDLInitialized = false;

	static void SDLErrorCheck(const char* context)
	{
		const char* err = SDL_GetError();
		if (err && err[0] != '\0')
		{
			HZ_CORE_ERROR("SDL Error in {0}: {1}", context, err);
			// After logging, you can clear it if you want to avoid repeats:
			// SDL_ClearError();
		}
	}

	/*static void GLFWErrorCallback(int error, const char* description)
	{
		HZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	} */

	//static bool s_GLFWInitialized = false;

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

		HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_SDLInitialized)
		{
			/*if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
			{
				HZ_CORE_ASSERT(false, "SDL_Init failed: {0}", SDL_GetError());
			}*/

			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
			{
				HZ_CORE_ERROR("SDL_Init failed: {0}", SDL_GetError());
				// handle fatal
			}
			s_SDLInitialized = true;
		}

		// OpenGL attributes (match your GL version)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
		m_Window = SDL_CreateWindow(
			m_Data.Title.c_str(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			(int)m_Data.Width, (int)m_Data.Height,
			flags
		);
		HZ_CORE_ASSERT(m_Window, "SDL_CreateWindow failed: {0}", SDL_GetError());

		m_GLContext = SDL_GL_CreateContext(m_Window);
		HZ_CORE_ASSERT(m_GLContext, "SDL_GL_CreateContext failed: {0}", SDL_GetError());

		SDL_GL_MakeCurrent(m_Window, m_GLContext);  // like glfwMakeContextCurrent

		// Load GL via GLAD using SDL's proc loader
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			HZ_CORE_ASSERT(false, "Failed to initialize GLAD");
		}

		SetVSync(true);

		// Query drawable size for HiDPI aware viewport sizing
		int fbW, fbH;
		SDL_GL_GetDrawableSize(m_Window, &fbW, &fbH);
		glViewport(0, 0, fbW, fbH); // your renderer may override later

		/*if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			HZ_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				auto& data = *((WindowData*)glfwGetWindowUserPointer(window));
				data.Width = width;
				data.Height = height;
				// TODO: Create and dispatch event
			}); */
	}

	void WindowsWindow::Shutdown()
	{
		if (m_GLContext) {
			SDL_GL_DeleteContext(m_GLContext);
			m_GLContext = nullptr;
		}
		if (m_Window) {
			SDL_DestroyWindow(m_Window);
			m_Window = nullptr;
		}
		// Do NOT SDL_Quit() here if you may open multiple windows later.
	}

	void WindowsWindow::OnUpdate()
	{
		HZ_CORE_INFO("Window size = {0}, {1}", GetWidth(), GetHeight());
		//glfwPollEvents();
		//glfwSwapBuffers(m_Window);

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				// Dispatch your WindowCloseEvent here
				if (m_Data.EventCallback) {
					// WindowCloseEvent ev; m_Data.EventCallback(ev);
				}
				break;
			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
					m_Data.Width = (uint32_t)e.window.data1;
					m_Data.Height = (uint32_t)e.window.data2;

					int fbW, fbH;
					SDL_GL_GetDrawableSize(m_Window, &fbW, &fbH);
					glViewport(0, 0, fbW, fbH);

					if (m_Data.EventCallback) {
						// WindowResizeEvent ev((uint32_t)fbW, (uint32_t)fbH);
						// m_Data.EventCallback(ev);
					}
				}
				break;
			case SDL_KEYDOWN:
				// translate to your KeyPressedEvent (e.key.keysym.sym, e.key.repeat)
				break;
			case SDL_KEYUP:
				// translate to your KeyReleasedEvent
				break;
			case SDL_MOUSEMOTION:
				// translate to your MouseMovedEvent (e.motion.x, e.motion.y)
				break;
			case SDL_MOUSEWHEEL:
				// translate to your MouseScrolledEvent (e.wheel.x, e.wheel.y)
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				// translate to your MouseButton events
				break;
			case SDL_DROPFILE:
				// char* path = e.drop.file; ... SDL_free(path);
				break;
			default: break;
			}
		}

		SDL_GL_SwapWindow(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		SDL_GL_SetSwapInterval(enabled ? 1 : 0);
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	/*void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	} */

}