#include "Hazel/Core/Application.h"
#include "Hazel/Core/Events/ApplicationEvent.h"
#include "Hazel/Core/Log.h"

namespace Hazel{

#define BIND_EVENT_FN(fn) std::bind(&Application::##fn, this, std::placeholders::_1)


Application::Application(){

	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
}

Application::~Application(){


}

void Application::Run(){

/*	WindowResizeEvent e(1280, 720);
	if (e.IsInCategory(EventCategoryApplication))
	{
		HZ_TRACE(e.ToString());
	}
	if (e.IsInCategory(EventCategoryInput))
	{
		HZ_TRACE(e.ToString());
	} */


	while (m_Running) {
		m_Window->OnUpdate();
	}
}

void Application::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

	HZ_CORE_TRACE("{}", event.ToString());
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
	//HZ_CORE_INFO("Window Resize: {0}, {1}", e.GetWidth(), e.GetHeight());
	return false;
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}


}