#pragma once
#include "asset_object.h"


class ShaderAssetStorage {

};


class ShaderAsset : public AssetObjectSpecifier<ShaderAsset, ShaderAssetStorage> {
public:
	ShaderAsset(entt::entity e);

	static std::string GetAssetExtension();

};

