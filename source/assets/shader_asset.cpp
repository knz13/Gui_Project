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
	std::vector<std::string> shaderTypes;
	for (auto file : std::filesystem::directory_iterator(path)) {
		std::string fileName = file.path().filename().string();

		if (file.path().extension().string() == (".vert")) {
			shaderTypes.push_back("Vertex Shader");
			std::string source = LoadFileContents(std::filesystem::absolute(path + "/" + fileName).string());
			sources.push_back(std::make_pair(ShaderType::Vertex, source));
		}
		if (file.path().extension().string() == (".frag")) {
			shaderTypes.push_back("Fragment Shader");
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
	asset.Storage().m_Stages = shaderTypes;
	asset.Storage().m_Shader = ptr;

	m_LoadedShaders[path] = asset;

	return asset;

}

void ShaderAsset::OnCreate()
{
	Storage().m_Texture = HelperFunctions::LoadTextureFromFile<Type2D>("defaults/images/shader_logo.png");
}

void ShaderAsset::SetupExplorerIcon(ImVec2 size)
{
	if (Storage().m_Texture) {
		ImGui::Image((ImTextureID)Storage().m_Texture.GetID(), size);
	}
}

void ShaderAsset::CreateShaderAtLocationFromType(std::string path)
{
	std::filesystem::path chosenPath(path);

	if (!std::filesystem::exists(chosenPath.parent_path())) {
		LOG("Could not create shader at " << path << " because the path is not valid!");
		return;
	}

	std::ofstream stream;

	if (chosenPath.extension().string() == ".vert") {
		stream.open(path);

		stream << R"(#version 430 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent;

uniform mat4 MVP //model-view-projection matrix;

void main() {

	// write your shader code here!

};


)";

		stream.close();

		Storage().m_Stages.push_back("Vertex Shader");

		return;
	}
	if (chosenPath.extension().string() == ".frag") {
		stream.open(path);

		stream << R"(#version 430 core

out vec4 mainColor;

void main() {

	// write your frament shader code here!
	
	mainColor = vec4(1.0f,1.0f,1.0f,1.0f); // this is the output color!

};

)";
		stream.close();

		Storage().m_Stages.push_back("Fragment Shader");

		return;

	}

	

}

void ShaderAsset::OnShowProperties()
{
	ImGui::Text("Current Stages:");
	for (auto& text : Storage().m_Stages) {
		ImGui::Text(text.c_str());
	}

	ImGui::Button("+");

	if (ImGui::BeginPopupContextItem(0,ImGuiPopupFlags_MouseButtonLeft)) {

		for (auto& [text,extension] : m_ShaderTypes) {
			if (std::find(Storage().m_Stages.begin(), Storage().m_Stages.end(), text) != Storage().m_Stages.end()) {
				continue;
			}
			if (ImGui::MenuItem(text.c_str())) {
				CreateShaderAtLocationFromType(GetPath() + "/shader" + extension);
			}
		}


		ImGui::EndPopup();
	}
	

}

void ShaderAsset::ReadFile()
{

	std::string path = GetPath();

	if (!std::filesystem::exists(path)) {
		std::filesystem::create_directory(path);
	}
}
