#include "Hazel/hzpch.h"
#include "Hazel/Core/ImGuiLayer.h"

namespace Hazel {

	ImGuiLayer::ImGuiLayer(const std::string& name) {

		m_DebugName = name;

	}

	ImGuiLayer::~ImGuiLayer() {
	}

	void ImGuiLayer::OnAttach() {

	}

	void ImGuiLayer::OnDetach(){

	}

	void ImGuiLayer::OnEvent(Event& e) {
		
	}

	void ImGuiLayer::OnUpdate() {

	}

} // nanespace Hazel
