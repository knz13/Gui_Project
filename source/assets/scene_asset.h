#pragma once
#include "asset_object.h"



class SceneAssetStorage {
	Texture<Type2D> m_IconTexture;


	friend class SceneAsset;
};


class SceneAsset : public AssetObjectSpecifier<SceneAsset,SceneAssetStorage> {
public:
	SceneAsset(entt::entity e);

	static std::vector<std::string> GetAssetExtensions();

private:
	void OnCreate() override;
	
	void OnShowProperties() override;
	
	void SetupExplorerIcon(ImVec2 size) override;


};

