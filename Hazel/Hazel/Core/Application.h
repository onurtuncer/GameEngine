#pragma once

#include "Hazel/Core/Core.h"
//#include "Hazel/Core/Events/Event.h"
#include "Hazel/Core/Window.h"

#include "Hazel/Core/Events/ApplicationEvent.h"

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
private:
	bool OnWindowResize(WindowResizeEvent& e);
	bool OnWindowClose(WindowCloseEvent& e);

private:
	std::unique_ptr<Window> m_Window;
	bool m_Running{ true };

};

// To be defined in CLIENT
Application* CreateApplication();

}