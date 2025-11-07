#pragma once

#include "Hazel/Core/Window.h"

#include <utility>

#include <glad/glad.h>   // MUST be first GL include
#include <SDL.h>         // brings SDL_GL_* declarations via SDL_video.h

namespace Hazel {

	

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes

		void SetEventCallback(const EventCallbackFn& cb) override { m_Data.EventCallback = cb; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		void* GetNativeWindow() const override { return m_Window; }


	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		//GLFWwindow* m_Window;
		SDL_Window* m_Window = nullptr;
		SDL_GLContext m_GLContext = nullptr;
		EventCallbackFn m_EventCallbackFn;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;  
		};

		WindowData m_Data;
	};

}