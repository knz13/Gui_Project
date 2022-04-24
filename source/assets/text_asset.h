#pragma once
#include "asset_object.h"


class TextAssetStorage {

};

class TextAsset : public AssetObjectSpecifier<TextAsset,TextAssetStorage> {
public:
	TextAsset(entt::entity e);


	static std::string GetAssetExtension();
private:
	void ShowProperties() override;



};
