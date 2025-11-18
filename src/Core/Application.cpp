#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>     // include BEFORE glad on Windows (avoids APIENTRY re-def)
#endif

#include <glad/glad.h>     // provides glClearColor, glClear, GL_COLOR_BUFFER_BIT

#include "Hazel/Core/Application.h"
#include "Hazel/Core/Events/ApplicationEvent.h"
#include "Hazel/Core/Log.h"

#include <memory>

namespace Hazel{

	Application* Application::s_Instance = nullptr;

#define BIND_EVENT_FN(fn) std::bind(&Application::##fn, this, std::placeholders::_1)


Application::Application(){

	HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
	s_Instance = this;

	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);
}

Application::~Application()
{

}

Application& Application::Get()
{
	return *s_Instance;
}

void Application::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* layer)
{
	m_LayerStack.PushOverlay(layer);
}

void Application::Run()
{
	OnInit();
	while (m_Running)
	{
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		for (Layer* layer : m_LayerStack)
			layer->OnUpdate();

		// ImGui frame
		if (m_ImGuiLayer)
		{
			m_ImGuiLayer->Begin();

			// Optional: ImGuiLayer's own debug UI (demo window, etc.)
			m_ImGuiLayer->OnImGuiRender();

			// All layers' ImGui
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			m_ImGuiLayer->End();
		}

		// 4) Swap buffers / process window stuff
		m_Window->OnUpdate();

	}
	OnShutdown();
}

void Application::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

	HZ_CORE_TRACE("{}", event.ToString());

	for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
	{
		(*--it)->OnEvent(event);
		if (event.Handled)
			break;
	}
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


} // namespace Hazel