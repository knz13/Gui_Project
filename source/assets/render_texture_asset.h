#pragma once
#include "asset_object.h"
#include "../opengl_wrappers/framebuffer.h"


struct RenderTextureAssetStorage {
	ecspp::HelperClasses::PointerHolder<Framebuffer> m_Framebuffer;
};

class RenderTextureAsset : public AssetObjectSpecifier<RenderTextureAsset,RenderTextureAssetStorage> {
public:
	RenderTextureAsset(entt::entity e) : AssetObjectSpecifier(e) {
		
	};

	static std::vector<std::string> GetAssetExtensions() {
		return {".renderTexture"};
	};

	void OnCreate() override;
private:
	void ReadFile() override;
	void OnShowProperties() override;
	void SetupExplorerIcon(ImVec2 size) override;


};