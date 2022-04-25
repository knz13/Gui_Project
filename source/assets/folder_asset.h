#pragma once
#include "asset_object.h"


class FolderAssetProperties {



};

class FolderAsset : public AssetObjectSpecifier<FolderAsset, FolderAssetProperties> {
public:
	FolderAsset(entt::entity e);

	static std::vector<std::string> GetAssetExtensions();

	void ReadFile() override;

};