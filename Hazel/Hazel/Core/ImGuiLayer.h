#pragma once

#include "Hazel/Core/Layer.h"

namespace Hazel {

	class ImGuiLayer : public Layer {

	public:
		ImGuiLayer(const std::string& name = "ImGui Layer");
		virtual ~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(Event& event) override;

	};

} // namesoace Hazel