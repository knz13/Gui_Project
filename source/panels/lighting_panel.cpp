#include "lighting_panel.h"
#include "../gui_layer/gui_layer.h"

void LightingPanel::Update(bool& isOpen)
{	
	if (isOpen) {
		GuiLayer::SetupWindowStyle("Lighting Panel", [&](ImGuiWindowFlags flags) {
		ImGui::Begin("Lighting Panel", &isOpen, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
			});

		ImGui::Text("Skybox");
		ImGui::SameLine();
		ImGui::AlignNextRight();
		bool val = false;

		ImGui::Selectable(("None*##" + HelperFunctions::GenerateStringHash(&isOpen)).c_str(), &val, 0, ImVec2(ImGui::GetFontSize() * 4, ImGui::GetFontSize()));

		if (ImGui::BeginDragDropTarget()) {
			if (const auto* payload = ImGui::AcceptDragDropPayload("MaterialAsset"); payload) {
				SetSkybox({ *(entt::entity*)payload->Data });
			}
			ImGui::EndDragDropTarget();
		}


		ImGui::End();
	}
}

void LightingPanel::Init()
{
}

void LightingPanel::SetSkybox(ecspp::ObjectHandle handle)
{
	if (handle) {
		if (handle.GetAs<GameObject>().GetType() == "MaterialAsset") {
			m_Skybox = handle;
		}
	}
}
