#include "text_asset.h"
#include "text_asset.h"

TextAsset::TextAsset(entt::entity e) : AssetObjectSpecifier(e)
{
}

std::string TextAsset::GetAssetExtension()
{
	return "All";
}

void TextAsset::ShowProperties()
{

}
