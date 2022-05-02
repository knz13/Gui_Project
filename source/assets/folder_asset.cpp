#include "folder_asset.h"
#include "../../vendor/stb/stb_image.h"
#include "../kv.h"

FolderAsset::FolderAsset(entt::entity e) : AssetObjectSpecifier(e)
{

}

std::string FolderAsset::GetAssetExtension()
{
	return {};
}

void FolderAsset::ShowProperties()
{
	if (Storage().m_IconTexture) {
		ImGui::Image((void*)Storage().m_IconTexture.GetID(), ImVec2(40, 40), ImVec2(0, 0), ImVec2(1, 1));
		ImGui::SameLine();
		ImGui::TextWrapped((std::filesystem::path(GetPath()).stem().string() + " (Folder Asset)").c_str());

		ImGui::Dummy(ImVec2(ImGui::GetWindowSize().x - ImGui::CalcTextSize("OpenSelf").x, 1));
		ImGui::SameLine();
		if (ImGui::Button("Open")) {
			GuiLayer::ExplorerView::SetCurrentPath(GetPath());
		}
	}
	
}

void FolderAsset::ReadFile()
{

}

void FolderAsset::SetupExplorerIcon(ImVec2 size)
{


	if (Storage().m_IconTexture) {
		ImGui::Image((void*)Storage().m_IconTexture.GetID(), ImVec2(size.x, size.y), ImVec2(0, 0), ImVec2(1, 1));

	}

}

FolderAssetProperties::FolderAssetProperties() : m_IconTexture(HelperFunctions::LoadTextureFromFile<Type2D>("defaults/images/folder3.png"))
{
}
