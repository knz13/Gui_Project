#include "shader_asset.h"

ShaderAsset::ShaderAsset(entt::entity e) : AssetObjectSpecifier(e)
{
}

std::vector<std::string> ShaderAsset::GetAssetExtensions()
{
	return {".shader"};
}
