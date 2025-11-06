#pragma once

extern Hazel::Application* Hazel::CreateApplication();

#ifndef HAZEL_NO_MAIN

int main(int argc, char** argv)
{
	Hazel::Log::Init();
	HZ_CORE_WARN("Initialized Log!");
	int a = 5;
	HZ_INFO("Hello! Var={0}", a);


    auto app = Hazel::CreateApplication();
	HZ_CORE_ASSERT(app, "Client Application is null!");
    app->Run();
    delete app;
}

#endif