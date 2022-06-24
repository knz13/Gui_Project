#pragma once
#include "asset_object.h"
#include "../opengl_wrappers/shader.h"

class ShaderAssetStorage {
public:
	ecspp::HelperClasses::PointerHolder<Shader> m_Shader;
	std::vector<std::string> m_Stages;
	Texture<Type2D> m_Texture;
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
	void OnCreate() override;
	void SetupExplorerIcon(ImVec2 size) override;
	void CreateShaderAtLocationFromType(std::string path);
	void OnShowProperties() override;
	static inline std::unordered_map<std::string, ecspp::ObjectHandle> m_LoadedShaders;
	static inline std::unordered_map<std::string, std::string> m_ShaderTypes = {
		{"Vertex Shader",".vert"},
		{"Fragment Shader",".frag" },
		{"Compute Shader",".comp"},
		{"Geometry Shader",".geom"}
	};

	void ReadFile() override;

};

