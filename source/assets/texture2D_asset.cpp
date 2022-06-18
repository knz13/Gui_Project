#include "texture2D_asset.h"

std::vector<std::string> Texture2DAsset::GetAssetExtensions()
{
	return {".png",".jpg","jpeg"};
}

void Texture2DAsset::ReadFile()
{
	std::string path = GetPath();

	Storage().m_Texture = HelperFunctions::LoadTextureFromFile<Type2D>(path);

}

void Texture2DAsset::ShowProperties()
{
}

void Texture2DAsset::SetupExplorerIcon(ImVec2 size)
{
}
