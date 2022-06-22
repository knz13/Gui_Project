#pragma once
#include "asset_object.h"
#include "../opengl_wrappers/texture.h"

class FolderAssetProperties {
public:
	FolderAssetProperties();
private:
	Texture<Type2D> m_IconTexture;

	friend class FolderAsset;
};

class FolderAsset : public AssetObjectSpecifier<FolderAsset, FolderAssetProperties> {
public:
	FolderAsset(entt::entity e);

	static std::vector<std::string> GetAssetExtensions();


private:
	void OnShowProperties() override;
	void SetupExplorerIcon(ImVec2 size) override;

};