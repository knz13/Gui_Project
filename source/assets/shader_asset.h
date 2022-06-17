#pragma once
#include "asset_object.h"


class ShaderAssetStorage {

};


class ShaderAsset : public AssetObjectSpecifier<ShaderAsset, ShaderAssetStorage> {
public:
	ShaderAsset(entt::entity e);

	static std::vector<std::string> GetAssetExtensions();
private:

	void ReadFile() override;

};

