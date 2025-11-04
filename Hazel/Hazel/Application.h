#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Hazel{

class Application{

public:
    Application();
    virtual ~Application();

    void Run();

};

// To be defined in CLIENT
Application* CreateApplication();

}