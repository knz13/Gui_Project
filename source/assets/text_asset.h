#pragma once
#include "asset_object.h"


class TextAssetStorage {
private:
	std::string m_TextContents = "";

	friend class TextAsset;
};

class TextAsset : public AssetObjectSpecifier<TextAsset,TextAssetStorage> {
public:
	TextAsset(entt::entity e);


	static std::vector<std::string> GetAssetExtensions();

	void ReadFile() override;
private:
	void ShowProperties() override;



};
