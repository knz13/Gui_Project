#include "scene_asset.h"

SceneAsset::SceneAsset(entt::entity e) : AssetObjectSpecifier(e)
{
}

std::vector<std::string> SceneAsset::GetAssetExtensions()
{
	return {".scene"};
}

void SceneAsset::ReadFile()
{
	std::string path = GetPath();

	if (!std::filesystem::exists(path)) {
		std::ofstream stream;
		stream.open(path);
		stream.close();
	}
}

void SceneAsset::OnCreate()
{
	Storage().m_IconTexture = HelperFunctions::LoadTextureFromFile<Type2D>("defaults/images/ScenePlay.png");

}

void SceneAsset::OnShowProperties()
{
	if (Storage().m_IconTexture) {
		ImGui::Dummy(ImVec2(ImGui::GetWindowSize().x - ImGui::CalcTextSize("OpenSelf").x, 1));
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2*ImGui::GetFontSize());
		if (ImGui::Button("Open")) {
			GuiLayer::LoadScene(GetPath());
		}
	}
}



void SceneAsset::SetupExplorerIcon(ImVec2 size)
{
	if (Storage().m_IconTexture) {
		ImGui::Image((ImTextureID)Storage().m_IconTexture.GetID(),size);
	}
}
