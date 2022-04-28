#include "asset_object.h"
#include "../kv.h"
#include "text_asset.h"
#include "folder_asset.h"


void AssetRegister::LoadAssetsForFolder(std::string folderPath)
{
	if (!std::filesystem::exists(folderPath)) {
		return;
	}
	for (auto file : std::filesystem::directory_iterator(folderPath)) {
		if (!IsRegistered(file.path().string())) {
			LoadAssetForPath(file.path().string());
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

std::string AssetRegister::GetRegisteredAssetForExtension(std::string extension)
{
	if (m_RegisteredClassesByExtension.find(extension) != m_RegisteredClassesByExtension.end()) {
		return m_RegisteredClassesByExtension[extension];
	}
	return "";
}

ObjectHandle AssetRegister::LoadAssetForPath(std::string path)
{
	if (!GetAssetForPath(path)) {
		if (std::filesystem::is_directory(path)) {
			ObjectHandle obj = ObjectPropertyRegister::CreateObjectFromType("FolderAsset", std::filesystem::path(path).stem().string());
			FolderAsset asset(obj.ID());
			asset.SetPath(path);
			return ObjectHandle(asset.ID());
		}
		if (std::string extClass = AssetRegister::GetRegisteredAssetForExtension(std::filesystem::path(path).extension().string()); extClass != "") {
			ObjectHandle obj = ObjectPropertyRegister::CreateObjectFromType(extClass, std::filesystem::path(path).stem().string());
			AssetObject asset(obj.ID());
			asset.SetPath(path);
			return ObjectHandle(obj.ID());
		}
		ObjectHandle obj = ObjectPropertyRegister::CreateObjectFromType("TextAsset", std::filesystem::path(path).stem().string());
		AssetObject asset(obj.ID());
		asset.SetPath(path);
		return ObjectHandle(obj.ID());
		
	}
	return GetAssetForPath(path);
}

ObjectHandle AssetRegister::GetAssetForPath(std::string path)
{
	if (m_RegisteredAssetsByPath.find(path) != m_RegisteredAssetsByPath.end()) {
		return ObjectHandle(m_RegisteredAssetsByPath[path]);
	}
	else return {};
}

std::string AssetRegister::GetPathFromAsset(ObjectHandle handle)
{
	if (handle) {
		return m_RegistererdPathsByAsset[handle.ID()];
	}
	return "";
}

bool AssetRegister::UnloadAsset(std::string path)
{
	if (m_RegisteredAssetsByPath.find(path) != m_RegisteredAssetsByPath.end()) {
		ObjectPropertyRegister::DeleteObject(m_RegisteredAssetsByPath[path]);
		return true;
	}
	DEBUG_LOG("Could not unload asset at path " + path + " because the asset could not be found!");
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
			ObjectPropertyRegister::DeleteObject(Object(e));
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
			ObjectPropertyRegister::DeleteObject(Object(id));
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
	HelperFunctions::CallMetaFunction(ObjectPropertyRegister::GetClassNameByID(Object(m_Handle).GetTypeOfObject()), "Call Read File",m_Handle);
	
}

void AssetObject::ShowOnExplorer(ImVec2 size)
{
	HelperFunctions::CallMetaFunction(ObjectPropertyRegister::GetClassNameByID(Object(m_Handle).GetTypeOfObject()), "Call Explorer UI",m_Handle,size);

}

void AssetObject::Rename()
{
	HelperFunctions::CallMetaFunction(ObjectPropertyRegister::GetClassNameByID(Object(m_Handle).GetTypeOfObject()), "Call Rename",m_Handle);

}






void AssetObject::ReadFile()
{
}

void AssetObject::SetPath(std::string path)
{
	AssetRegister::RegisterPath(m_Handle, path);
}


