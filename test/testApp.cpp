#include <Hazel.h>

class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer() : Layer("ExampleLayer") {}

    void OnImGuiRender() override
    {
        ImGui::Begin("Hello ImGui from Sandbox!");
        ImGui::Text("This is rendered via Hazel’s ImGuiLayer.");
        ImGui::End();
    }
};

class Sandbox: public Hazel::Application{

    public:
        Sandbox() {
            PushLayer(new ExampleLayer());
        }
        ~Sandbox(){}
};


Hazel::Application* Hazel::CreateApplication(){
    return new Sandbox();
}


