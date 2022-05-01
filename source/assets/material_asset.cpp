#include "material_asset.h"

MaterialAsset::MaterialAsset(entt::entity e) : AssetObjectSpecifier(e)
{
}

std::vector<std::string> MaterialAsset::GetAssetExtensions()
{
	return {".mat"};
}
