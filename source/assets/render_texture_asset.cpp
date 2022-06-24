#include "render_texture_asset.h"

void RenderTextureAsset::OnCreate()
{
	Storage().m_Framebuffer.HoldType<Framebuffer>(200,200);

	Storage().m_Framebuffer.Get()->Clear();
}

void RenderTextureAsset::ReadFile()
{
	std::string path = GetPath();

	if (!std::filesystem::exists(path)) {
		std::ofstream stream;
		stream.open(path);
		stream.close();
	}
}

void RenderTextureAsset::OnShowProperties()
{
}

void RenderTextureAsset::SetupExplorerIcon(ImVec2 size)
{
	ImGui::Image((ImTextureID)Storage().m_Framebuffer.Get()->GetAttachedTexture().GetID(), size);
}