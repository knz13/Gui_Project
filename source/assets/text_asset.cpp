#include "text_asset.h"
#include "text_asset.h"

TextAsset::TextAsset(entt::entity e) : AssetObjectSpecifier(e)
{
}

std::vector<std::string> TextAsset::GetAssetExtensions()
{
	return { "None" };
}

void TextAsset::ReadFile()
{
	std::string path = this->GetPath();

	std::fstream stream(path);

	std::string temp;
	while (!stream.good()) {
		stream >> temp;
		Storage().m_TextContents += temp;
	}

	
}

void TextAsset::ShowProperties()
{

}
