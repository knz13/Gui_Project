#include "asset_object.h"
#include "../kv.h"
#include "text_asset.h"



void AssetRegister::CreateObjectsForFolder(std::string folderPath)
{
	if (!std::filesystem::exists(folderPath)) {
		return;
	}
	for (auto file : std::filesystem::directory_iterator(folderPath)) {
		if (!IsRegistered(file.path().string())) {
			ObjectPropertyRegister::CreateObjectFromType("TextAsset",file.path().filename().string());
		}
	}


}


bool AssetRegister::IsRegistered(std::string path)
{
	if (m_RegisteredAssetsByPath.find(path) != m_RegisteredAssetsByPath.end()) {
		return true;
	}
	return false;
}

void AssetRegister::RegisterPath(entt::entity e, std::string path)
{
	
	UnregisterPath(path);

	m_RegisteredAssetsByPath[path] = e;
	m_RegistererdPathsByAsset[e] = path;

}

void AssetRegister::UnregisterPath(entt::entity e)
{
	if (m_RegistererdPathsByAsset.find(e) != m_RegistererdPathsByAsset.end()) {
		std::string path = m_RegistererdPathsByAsset[e];

		m_RegisteredAssetsByPath.erase(path);
		m_RegistererdPathsByAsset.erase(e);

		if (ObjectHandle(e)) {
			Registry::DeleteObject(Object(e));
		}

	}
}

void AssetRegister::UnregisterPath(std::string path)
{
	if (m_RegisteredAssetsByPath.find(path) != m_RegisteredAssetsByPath.end()) {
		entt::entity id = m_RegisteredAssetsByPath[path];
		m_RegisteredAssetsByPath.erase(path);
		m_RegistererdPathsByAsset.erase(id);

		if (ObjectHandle(id)) {
			Registry::DeleteObject(Object(id));
		}

	}
}

AssetObject::AssetObject(entt::entity e)
{
	m_Handle = e;
}

void AssetObject::OnExplorerName()
{
	if (!ObjectHandle(m_Handle)) {
		return;
	}
	ImGui::Text(Object(m_Handle).GetName().c_str());
}

void AssetObject::OnExplorerRename()
{
	if (!ObjectHandle(m_Handle)) {
		return;
	}
	std::string name = Object(m_Handle).GetName();

	ImGui::InputText(GuiLayer::GetImGuiID(&m_Handle).c_str(),&name);

	Object(m_Handle).Properties().SetName(name);

}
