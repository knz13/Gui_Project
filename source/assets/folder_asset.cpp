#include "folder_asset.h"
#include "../../vendor/stb/stb_image.h"
#include "../kv.h"

FolderAsset::FolderAsset(entt::entity e) : AssetObjectSpecifier(e)
{

}

std::vector<std::string> FolderAsset::GetAssetExtensions()
{
	return {};
}

void FolderAsset::ReadFile()
{

	Storage().m_IconTexture = Texture<Type2D>([](Texture<Type2D>& texture){
		texture.Bind();

		int width, height, channels;

		unsigned char* data = stbi_load("defaults/images/folder2.png",&width,&height,&channels,0);


		GL_CALL(glTexParameteri(texture.GetType(), GL_TEXTURE_WRAP_S, GL_REPEAT));
		GL_CALL(glTexParameteri(texture.GetType(), GL_TEXTURE_WRAP_T, GL_REPEAT));
		GL_CALL(glTexParameteri(texture.GetType(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GL_CALL(glTexParameteri(texture.GetType(), GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		
		GL_CALL(glTexImage2D(texture.GetType(), 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
		
		stbi_image_free(data);

		texture.Unbind();
	});
	
	//HelperFunctions::LoadTextureFromFile<Type2D>("defaults/images/folder3.png");

}

void FolderAsset::SetupExplorerIcon(ImVec2 size)
{

	if (Storage().m_IconTexture) {
		Storage().m_IconTexture.Bind();
		unsigned int id = Storage().m_IconTexture.GetID();
		ImGui::Image((ImTextureID)id, ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));
		Storage().m_IconTexture.Unbind();
	}

}
