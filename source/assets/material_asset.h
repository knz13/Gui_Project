#pragma once
#include "asset_object.h"






class MaterialAssetStorage {


};


class MaterialAsset : public AssetObjectSpecifier<MaterialAsset, MaterialAssetStorage> {
public:
	MaterialAsset(entt::entity e);

	static std::vector<std::string> GetAssetExtensions();

private:
	

};