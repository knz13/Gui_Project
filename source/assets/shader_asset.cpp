#include "shader_asset.h"

ShaderAsset::ShaderAsset(entt::entity e) : AssetObjectSpecifier(e)
{
}

std::string ShaderAsset::GetAssetExtension()
{
	return {".shader"};
}
