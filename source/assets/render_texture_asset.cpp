#include "render_texture_asset.h"

void RenderTextureAsset::OnCreate()
{
	Storage().m_Framebuffer.HoldType<Framebuffer>(200,200);

	Storage().m_Framebuffer.Get()->Clear();
}

void RenderTextureAsset::ShowProperties()
{
	ImGui::Image((void*)Storage().m_Framebuffer.Get()->GetAttachedTexture().GetID(), ImVec2(40, 40));
	ImGui::SameLine();
	ImGui::TextWrapped((std::filesystem::path(GetPath()).stem().string() + " (Render Texture Asset)").c_str());
}

void RenderTextureAsset::SetupExplorerIcon(ImVec2 size)
{
	ImGui::Image((ImTextureID)Storage().m_Framebuffer.Get()->GetAttachedTexture().GetID(), size);
}