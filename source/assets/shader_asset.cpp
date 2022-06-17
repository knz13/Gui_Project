#include "shader_asset.h"

ShaderAsset::ShaderAsset(entt::entity e) : AssetObjectSpecifier(e)
{
}

Shader& ShaderAsset::GetUnderlyingShader()
{
	if (!Storage().m_Shader) {
		throw std::runtime_error("Tried getting an invalid shader at " + GetPath());
	}

	return *Storage().m_Shader.Get();
}

std::vector<std::string> ShaderAsset::GetAssetExtensions()
{
	return {".shader"};
}

ecspp::ObjectHandle ShaderAsset::LoadFromPath(std::string path)
{
	if (m_LoadedShaders.find(path) != m_LoadedShaders.end()) {
		if (!m_LoadedShaders[path]) {
			m_LoadedShaders.erase(path);
			return {};
		}
		return m_LoadedShaders[path];
	}

	if (!std::filesystem::exists(path)) {
		return {};
	}

	if (!std::filesystem::is_directory(path)) {
		return {};
	}

	std::vector<std::pair<ShaderType, std::string>> sources;
	for (auto file : std::filesystem::directory_iterator(path)) {
		std::string fileName = file.path().filename().string();

		if (file.path().extension().string() == (".vert")) {
			std::string source = LoadFileContents(std::filesystem::absolute(path + "/" + fileName).string());
			sources.push_back(std::make_pair(ShaderType::Vertex, source));
		}
		if (file.path().extension().string() == (".frag")) {
			std::string source = LoadFileContents(std::filesystem::absolute(path + "/" + fileName).string());
			sources.push_back(std::make_pair(ShaderType::Fragment, source));
		}
	}

	ecspp::HelperClasses::PointerHolder<Shader> ptr;
	Shader& shader = ptr.HoldType<Shader>();
	ShaderCreationProperties prop = shader.CreateNew();

	for (auto& [type, source] : sources) {
		prop.AddShader(type, source);
	}

	if (!prop.Generate()) {
		return {};
	}

	ShaderAsset asset = ShaderAsset::CreateNew(path);
	asset.SetPath(path);

	asset.Storage().m_Shader = ptr;

	m_LoadedShaders[path] = asset;

	return asset;

}

void ShaderAsset::ReadFile()
{
	std::string path = this->GetPath();


}
