#include "material_asset.h"

MaterialAsset::MaterialAsset(entt::entity e) : AssetObjectSpecifier(e)
{
}

std::string MaterialAsset::GetAssetExtension()
{
	return ".mat";
}
