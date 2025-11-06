#pragma once

#include "Hazel/Core/Window.h"

namespace Hazel {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_EventCallbackFn = callback; }

		inline unsigned int GetWidth() const override { return m_Width; }
		inline unsigned int GetHeight() const override { return m_Height; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		std::string m_Title;
		unsigned int m_Width, m_Height;

		EventCallbackFn m_EventCallbackFn;
	};

}