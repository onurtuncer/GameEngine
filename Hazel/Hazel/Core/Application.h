#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Events/Event.h"
#include "Hazel/Core/Window.h"

namespace Hazel{

class Application{

public:
    Application();
    virtual ~Application();

    void Run();

	virtual void OnInit() {}
	virtual void OnShutdown() {}
	virtual void OnUpdate() {}

private:
	std::unique_ptr<Window> m_Window;

};

// To be defined in CLIENT
Application* CreateApplication();

}