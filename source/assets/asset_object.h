#pragma once
#include "../object/tagged_object.h"
#include "../gui_layer/gui_useful_implementations.h"
#include <filesystem>

namespace GuiLayer {
	class ExplorerView;
}



class AssetRegister {
public:

	static void CreateObjectsForFolder(std::string folderPath);
	static std::string GetRegisteredClassForExtension(std::string extension);
	static ObjectHandle CreateObjectForPath(std::string path);
	static ObjectHandle GetObjectForPath(std::string path);
	static std::string GetPathFromObject(ObjectHandle handle);
private:

	static bool IsRegistered(std::string path);
	static void RegisterPath(entt::entity e,std::string path);
	static void UnregisterPath(entt::entity e);
	static void UnregisterPath(std::string path);

	template<typename T>
	static void RegisterClassAsAsset() {
		entt::id_type hash = HelperFunctions::HashClassName<T>();
		entt::meta<T>().type(hash).template func<&ObjectPropertyRegister::CreateObjectAndReturnHandle<T>>(entt::hashed_string("Create"));
		auto vec = T::GetAssetExtensions();
		for (auto& extension : vec) {
			m_RegisteredClassesByExtension[extension] = HelperFunctions::GetClassName<T>();
		}
	};


	static inline std::unordered_map<std::string, entt::entity> m_RegisteredAssetsByPath;
	static inline std::unordered_map<entt::entity,std::string> m_RegistererdPathsByAsset;
	static inline std::unordered_map<std::string, std::string> m_RegisteredClassesByExtension;


	template<typename ,typename>
	friend class AssetObjectSpecifier;

	friend class AssetObject;

};

//Do Not Use!
template<typename Derived,typename Storage,typename ComponentName = ComponentHelpers::Null>
class AssetComponent : public ComponentSpecifier<ComponentName, AssetObjectSpecifier<Derived,Storage>> {

};

class AssetObject {
public:
	AssetObject(entt::entity e);
	~AssetObject();

	void InitializeFile();
protected:
	virtual void OnExplorerIconUI();
private:
	virtual void ReadFile();
	void SetPath(std::string path);
	void OnExplorerIcon();
	void OnExplorerName();
	void OnExplorerRename();
	entt::entity m_Handle;

	friend class GuiLayer::ExplorerView;
	friend class AssetRegister;
	
};

template<typename Derived,typename Storage>
class AssetObjectSpecifier : public TaggedObject<AssetObjectSpecifier<Derived,Storage>, AssetComponent<Derived,Storage>,Storage> ,public AssetObject {
public:
	AssetObjectSpecifier(entt::entity e) : TaggedObject<AssetObjectSpecifier<Derived,Storage>, AssetComponent<Derived,Storage>, Storage>(e),AssetObject(e) {
		(void)dummy;
	}

	//virtual void Serialize(ryml::NodeRef mainNode) = 0;
	//virtual void Deserialize(ryml::NodeRef mainNode) = 0;

	

	std::string GetPath() {
		return AssetRegister::GetPathFromObject(ObjectHandle(this->ID()));
	}
	 
private:
	
	void Init() final {
		
	}
	void Destroy() final {
		AssetRegister::UnregisterPath(this->ID());
	}

	static inline bool dummy = []() {
		AssetRegister::RegisterClassAsAsset<Derived>();
		return false;
	}();


};

