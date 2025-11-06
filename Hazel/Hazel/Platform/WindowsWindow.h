#pragma once

#include "Hazel/Core/Window.h"

namespace Hazel {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_EventCallbackFn = callback; }

		inline unsigned int GetWidth() const override { return m_Width; }
		inline unsigned int GetHeight() const override { return m_Height; }
	protected:
		virtual void Init(const WindowProps& props) override;
		virtual void Shutdown() override;
	private:
		std::string m_Title;
		unsigned int m_Width;
		unsigned int  m_Height;

		EventCallbackFn m_EventCallbackFn;
	};

}