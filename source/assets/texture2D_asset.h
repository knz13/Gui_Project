#pragma once
#include "asset_object.h"
#include "../opengl_wrappers/texture.h"

struct Texture2DAssetStorage {
	Texture<Type2D> m_Texture;
};


class Texture2DAsset : public AssetObjectSpecifier<Texture2DAsset, Texture2DAssetStorage> {
public:
	Texture2DAsset(entt::entity e) : AssetObjectSpecifier(e) {

	}


	static std::vector<std::string> GetAssetExtensions();

private:

	void ReadFile() override;
	void OnShowProperties() override;
	void SetupExplorerIcon(ImVec2 size) override;



};