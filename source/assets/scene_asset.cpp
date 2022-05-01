#include "scene_asset.h"

SceneAsset::SceneAsset(entt::entity e) : AssetObjectSpecifier(e)
{
}

std::vector<std::string> SceneAsset::GetAssetExtensions()
{
	return {".scene"};
}

void SceneAsset::ShowProperties()
{
	if (Storage().m_IconTexture) {
		ImGui::Image((void*)Storage().m_IconTexture.GetID(), ImVec2(40, 40));
		ImGui::SameLine();
		ImGui::TextWrapped((std::filesystem::path(GetPath()).stem().string() + " (Scene Asset)").c_str());

		ImGui::Dummy(ImVec2(ImGui::GetWindowSize().x - ImGui::CalcTextSize("OpenSelf").x, 1));
		ImGui::SameLine();
		if (ImGui::Button("Open")) {
			GuiLayer::LoadScene(GetPath());
		}
	}
}

void SceneAsset::ReadFile()
{
	Storage().m_IconTexture = HelperFunctions::LoadTextureFromFile<Type2D>("defaults/images/ScenePlay.png");
}

void SceneAsset::SetupExplorerIcon(ImVec2 size)
{
	if (Storage().m_IconTexture) {
		ImGui::Image((ImTextureID)Storage().m_IconTexture.GetID(),size);
	}
}
