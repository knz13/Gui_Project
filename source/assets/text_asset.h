#pragma once
#include "asset_object.h"


class TextAssetStorage {

};

class TextAsset : public AssetObject<TextAssetStorage> {
public:
	TextAsset(entt::entity e, std::string path);


private:
	void ShowProperties() override;

};
