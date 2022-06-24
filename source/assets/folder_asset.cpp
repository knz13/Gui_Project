#include "folder_asset.h"
#include "../../vendor/stb/stb_image.h"
#include "../kv.h"

FolderAsset::FolderAsset(entt::entity e) : AssetObjectSpecifier(e)
{

}

std::vector<std::string> FolderAsset::GetAssetExtensions()
{
	return {};
}

void FolderAsset::ReadFile()
{
	std::string path = GetPath();

	if (!std::filesystem::exists(path)) {
		std::filesystem::create_directory(path);
	}
}

void FolderAsset::OnShowProperties()
{

	if (Storage().m_IconTexture) {
		ImGui::Dummy(ImVec2(ImGui::GetWindowSize().x - ImGui::CalcTextSize("OpenSelf").x, 1));
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2*ImGui::GetFontSize());
		if (ImGui::Button("Open")) {
			GuiLayer::ExplorerView::SetCurrentPath(GetPath());
		}
	}
	
}



void FolderAsset::SetupExplorerIcon(ImVec2 size)
{
	if (Storage().m_IconTexture) {
		ImGui::Image((void*)Storage().m_IconTexture.GetID(), ImVec2(size.x, size.y), ImVec2(0, 0), ImVec2(1, 1));
		if (ImGui::BeginDragDropTarget()) {
			const auto* payload = ImGui::AcceptDragDropPayload("AllAssets");

			if (payload) {
				entt::entity e = *(entt::entity*)payload->Data;
				if (ecspp::ObjectHandle(e) && e != ID()) {
					AssetObject(e).MoveTo(GetPath() + "/" + std::filesystem::path(AssetObject(e).GetPath()).filename().string());
				}
				

			}

			ImGui::EndDragDropTarget();
		}
	}

}

FolderAssetProperties::FolderAssetProperties() : m_IconTexture(HelperFunctions::LoadTextureFromFile<Type2D>("defaults/images/folder3.png"))
{
}
