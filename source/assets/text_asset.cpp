#include "text_asset.h"

TextAsset::TextAsset(entt::entity e) : AssetObjectSpecifier(e)
{
}

std::vector<std::string> TextAsset::GetAssetExtensions()
{
	return {};
}

void TextAsset::ReadFile()
{
	std::string path = this->GetPath();

	std::fstream stream(path);

	std::string temp;
	while (stream.good()) {
		stream >> temp;
		Storage().m_TextContents += temp;
	}

	Storage().m_Texture = HelperFunctions::LoadTextureFromFile<Type2D>("defaults/images/textFile.png");

	
}

void TextAsset::ShowProperties()
{
	ImGui::Image((void*)Storage().m_Texture.GetID(), ImVec2(40,40), ImVec2(0, 0), ImVec2(1, 1));
	ImGui::SameLine();
	ImGui::Text((std::filesystem::path(GetPath()).stem().string() + " (Text Asset)").c_str());

	if (ImGui::BeginTable(GuiLayer::GetImGuiID(&Storage()).c_str(), 1, ImGuiTableFlags_Borders)) {
		ImGui::TableNextColumn();
		ImGui::Text("Contents:");
		GuiLayer::SetupChildStyle([&]() {
			
			ImGui::BeginChild(("ChildWindowForTextAsset" + GuiLayer::GetImGuiID(&Storage())).c_str(),ImVec2(ImGui::GetContentRegionAvail().x,400));
			ImGui::TextWrapped(Storage().m_TextContents.c_str());
			ImGui::EndChild();
			});
		ImGui::EndTable();
	}
}

void TextAsset::SetupExplorerIcon(ImVec2 size)
{
	ImGui::Image((void*)Storage().m_Texture.GetID(), ImVec2(size.x, size.y), ImVec2(0, 0), ImVec2(1, 1));
}
