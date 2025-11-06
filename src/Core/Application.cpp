#include "Hazel/Core/Application.h"
#include "Hazel/Core/Events/ApplicationEvent.h"
#include "Hazel/Core/Log.h"

namespace Hazel{


Application::Application(){

	m_Window = std::unique_ptr<Window>(Window::Create());
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


	while (true) {
		m_Window->OnUpdate();
	}
}


}