#include "folder_asset.h"

FolderAsset::FolderAsset(entt::entity e) : AssetObjectSpecifier(e)
{
}

std::vector<std::string> FolderAsset::GetAssetExtensions()
{
	return {};
}

void FolderAsset::ReadFile()
{
	std::string data = "Nano";
	data += "a";
}
