#pragma once
#include "asset_object.h"
#include "../opengl_wrappers/shader.h"

class ShaderAssetStorage {
public:
	ecspp::HelperClasses::PointerHolder<Shader> m_Shader;
};


class ShaderAsset : public AssetObjectSpecifier<ShaderAsset, ShaderAssetStorage> {
public:
	ShaderAsset(entt::entity e);

	Shader& GetUnderlyingShader();

	operator bool() {
		return Storage().m_Shader.operator bool();
	}

	static std::vector<std::string> GetAssetExtensions();
	static ecspp::ObjectHandle LoadFromPath(std::string path);

private:
	static inline std::unordered_map<std::string, ecspp::ObjectHandle> m_LoadedShaders;

	void ReadFile() override;

};

