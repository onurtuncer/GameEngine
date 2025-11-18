#pragma once

#include "Hazel/Core/Base.h"
//#include "Hazel/Core/Events/Event.h"
#include "Hazel/Core/Window.h"
#include "Hazel/Core/LayerStack.h"
#include "Hazel/Core/ImGuiLayer.h"

#include "Hazel/Core/Events/ApplicationEvent.h"
#include "Hazel/Core/Timestep.h"

namespace Hazel{

class Application{

public:
    Application();
    virtual ~Application();

    void Run();

	virtual void OnInit() {}
	virtual void OnShutdown() {}
	virtual void OnUpdate() {}

	virtual void OnEvent(Event& event);

	static Application& Get();
	Window& GetWindow() { return *m_Window; }

	ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* layer);

private:
	bool OnWindowResize(WindowResizeEvent& e);
	bool OnWindowClose(WindowCloseEvent& e);

private:
	std::unique_ptr<Window> m_Window;
	bool m_Running{ true };
	LayerStack m_LayerStack;
	ImGuiLayer* m_ImGuiLayer;

	static Application* s_Instance;
};

// To be defined in CLIENT
Application* CreateApplication();

}