#include "asset_object.h"
#include "../kv.h"
#include "text_asset.h"
#include "folder_asset.h"


void AssetRegister::CreateObjectsForFolder(std::string folderPath)
{
	if (!std::filesystem::exists(folderPath)) {
		return;
	}
	for (auto file : std::filesystem::directory_iterator(folderPath)) {
		if (!IsRegistered(file.path().string())) {
			CreateObjectForPath(file.path().string());
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

std::string AssetRegister::GetRegisteredClassForExtension(std::string extension)
{
	if (m_RegisteredClassesByExtension.find(extension) != m_RegisteredClassesByExtension.end()) {
		return m_RegisteredClassesByExtension[extension];
	}
	return "";
}

ObjectHandle AssetRegister::CreateObjectForPath(std::string path)
{
	if (!GetObjectForPath(path)) {
		if (std::filesystem::is_directory(path)) {
			Object obj = ObjectPropertyRegister::CreateObjectFromType("FolderAsset", std::filesystem::path(path).filename().string());
			FolderAsset asset(obj.ID());
			asset.SetPath(path);
			return ObjectHandle(asset.ID());
		}
		if (std::string extClass = AssetRegister::GetRegisteredClassForExtension(std::filesystem::path(path).extension().string()); extClass != "") {
			Object obj = ObjectPropertyRegister::CreateObjectFromType(extClass, std::filesystem::path(path).filename().string());
			AssetObject asset(obj.ID());
			asset.SetPath(path);
			return ObjectHandle(obj.ID());
		}
		Object obj = ObjectPropertyRegister::CreateObjectFromType("TextAsset", std::filesystem::path(path).filename().string());
		AssetObject asset(obj.ID());
		asset.SetPath(path);
		return ObjectHandle(obj.ID());
		
	}
	return GetObjectForPath(path);
}

ObjectHandle AssetRegister::GetObjectForPath(std::string path)
{
	if (m_RegisteredAssetsByPath.find(path) != m_RegisteredAssetsByPath.end()) {
		return ObjectHandle(m_RegisteredAssetsByPath[path]);
	}
	else return {};
}

std::string AssetRegister::GetPathFromObject(ObjectHandle handle)
{
	if (handle) {
		return m_RegistererdPathsByAsset[handle.ID()];
	}
	return "";
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

AssetObject::~AssetObject()
{
	
}

void AssetObject::InitializeFile()
{
	this->ReadFile();
}

void AssetObject::OnExplorerIconUI()
{
	ImGui::Button(GuiLayer::GetImGuiID(&m_Handle).c_str(),ImVec2(30,30));
}

void AssetObject::ReadFile()
{
}

void AssetObject::SetPath(std::string path)
{
	AssetRegister::RegisterPath(m_Handle, path);
}

void AssetObject::OnExplorerIcon()
{
	this->OnExplorerIconUI();
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
