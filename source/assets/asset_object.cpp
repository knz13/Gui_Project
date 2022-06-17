#include "asset_object.h"
#include "../kv.h"
#include "text_asset.h"
#include "folder_asset.h"


void AssetRegister::LoadAssetsForFolder(std::string folderPath)
{
	if (!std::filesystem::exists(folderPath)) {
		return;
	}
	for (auto& file : std::filesystem::directory_iterator(folderPath)) {
		if (!PathHasRegisteredAsset(file.path().string())) {
			LoadAssetForPath(file.path().string());
		}
	}


}


std::string AssetRegister::GetExtensionForClass(std::string className)
{
	if (m_RegisteredExtensionsByClass.find(className) != m_RegisteredExtensionsByClass.end()) {
		return m_RegisteredExtensionsByClass[className][0];
	}
	return "";
}

bool AssetRegister::IsAsset(std::string typeName)
{
	return std::find(m_RegisteredAssetClasses.begin(), m_RegisteredAssetClasses.end(), typeName) != m_RegisteredAssetClasses.end();
}

bool AssetRegister::PathHasRegisteredAsset(std::string path)
{
	if (m_RegisteredAssetsByPath.find(path) != m_RegisteredAssetsByPath.end()) {
		return true;
	}
	return false;
}

std::string AssetRegister::GetRegisteredAssetForExtension(std::string extension)
{
	if (m_RegisteredClassByExtension.find(extension) != m_RegisteredClassByExtension.end()) {
		return m_RegisteredClassByExtension[extension];
	}
	return "";
}

bool AssetRegister::CreateAssetAtFolder(std::string folder, std::string assetType)
{
	if (!std::filesystem::exists(folder)) {
		DEBUG_LOG("Trying to create asset at folder " + folder + " which does not exist!");
		return false;
	}
	if (!ecspp::ObjectPropertyRegister::IsClassRegistered(assetType)) {
		DEBUG_LOG("Could not create asset at folder " + folder + "!\nClass " + assetType + " is not registered!");
		return false;
	}
	if (!IsAsset(assetType)) {
		DEBUG_LOG("Class " + assetType + " is not an asset type!");
		return false;
	}

	ecspp::ObjectHandle handle = ecspp::CreateNewObject(assetType, "tempName" + std::to_string(std::hash<std::string>()(folder + assetType)));
	if (!handle) {
		return false;
	}

	handle.GetAsObject().SetName("");

	AssetObject obj(handle.ID());

	auto result = HelperFunctions::CallMetaFunction(assetType,"Rename On Creation",handle.ID(),folder);

	if (!result) {
		DEBUG_LOG("Could not call meta function!");
		ecspp::DeleteObject(handle.GetAsObject());
		return false;
	}

	GuiLayer::ExplorerView::GetTempObject() = handle;
	GuiLayer::AnyObjectSelected() = handle;


	return true;



}

ecspp::ObjectHandle AssetRegister::LoadAssetForPath(std::string path)
{
	if (!GetAssetForPath(path)) {
		if (std::string extClass = AssetRegister::GetRegisteredAssetForExtension(std::filesystem::path(path).extension().string()); extClass != "") {
			ecspp::ObjectHandle obj = ecspp::CreateNewObject(extClass, std::filesystem::path(path).stem().string());
			AssetObject asset(obj.ID());
			
			asset.SetPath(path);
			return ecspp::ObjectHandle(obj.ID());
		}
		if (std::filesystem::is_directory(path)) {
			ecspp::ObjectHandle obj = ecspp::CreateNewObject("FolderAsset", std::filesystem::path(path).stem().string());
			FolderAsset asset(obj.ID());
			asset.SetPath(path);
			return ecspp::ObjectHandle(asset.ID());
		}
		ecspp::ObjectHandle obj = ecspp::CreateNewObject("TextAsset", std::filesystem::path(path).stem().string());
		AssetObject asset(obj.ID());
		asset.SetPath(path);
		return ecspp::ObjectHandle(obj.ID());
		
	}
	return GetAssetForPath(path);
}

ecspp::ObjectHandle AssetRegister::GetAssetForPath(std::string path)
{
	path = std::filesystem::path(path).lexically_normal().string();
	if (m_RegisteredAssetsByPath.find(path) != m_RegisteredAssetsByPath.end()) {
		return ecspp::ObjectHandle(m_RegisteredAssetsByPath[path]);
	}
	else return {};
}

std::string AssetRegister::GetPathFromAsset(ecspp::ObjectHandle handle)
{
	if (handle) {
		if (m_RegistererdPathsByAsset.find(handle.ID()) != m_RegistererdPathsByAsset.end()) {
			return m_RegistererdPathsByAsset[handle.ID()];
		}
		return "";
	}
	return "";
}

bool AssetRegister::UnloadAsset(std::string path)
{
	path = std::filesystem::path(path).lexically_normal().string();
	if (m_RegisteredAssetsByPath.find(path) != m_RegisteredAssetsByPath.end()) {
		ecspp::DeleteObject(m_RegisteredAssetsByPath[path]);
		return true;
	}
	DEBUG_LOG("Could not unload asset at path " + path + " because the asset could not be found!");
	return false;
}



void AssetRegister::RegisterPath(entt::entity e, std::string path)
{
	path = std::filesystem::path(path).lexically_normal().string();

	if (!std::filesystem::exists(path) && ecspp::Object(e).GetType() != "FolderAsset") {
		std::fstream stream;

		stream.open(path,std::fstream::out);



		stream.close();
	}

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

		if (ecspp::ObjectHandle(e)) {
			ecspp::DeleteObject(ecspp::Object(e));
		}

	}
}

void AssetRegister::UnregisterPath(std::string path,bool shouldDelete)
{
	path = std::filesystem::path(path).lexically_normal().string();
	if (m_RegisteredAssetsByPath.find(path) != m_RegisteredAssetsByPath.end()) {
		entt::entity id = m_RegisteredAssetsByPath[path];
		m_RegisteredAssetsByPath.erase(path);
		m_RegistererdPathsByAsset.erase(id);

		if (ecspp::ObjectHandle(id) && shouldDelete) {
			ecspp::DeleteObject(ecspp::Object(id));
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
	HelperFunctions::CallMetaFunction(ecspp::ObjectPropertyRegister::GetClassNameByID(ecspp::Object(m_Handle).GetTypeID()), "Call Read File",m_Handle);
	
}

void AssetObject::ShowOnExplorer(ImVec2 size)
{
	HelperFunctions::CallMetaFunction(ecspp::ObjectPropertyRegister::GetClassNameByID(ecspp::Object(m_Handle).GetTypeID()), "Call Explorer UI",m_Handle,size);

}

void AssetObject::Rename()
{
	HelperFunctions::CallMetaFunction(ecspp::ObjectPropertyRegister::GetClassNameByID(ecspp::Object(m_Handle).GetTypeID()), "Call Rename",m_Handle);

}






void AssetObject::SaveFile()
{
}



void AssetObject::ReadFile()
{
}

void AssetObject::SetPath(std::string path)
{
	AssetRegister::RegisterPath(m_Handle, path);
}


