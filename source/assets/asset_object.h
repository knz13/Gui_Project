#pragma once
#include "../object/tagged_object.h"
#include "../gui_layer/gui_useful_implementations.h"
#include <filesystem>

namespace GuiLayer {
	class ExplorerView;
}



class AssetRegister {

	static void CreateObjectsForFolder(std::string folderPath);

private:

	static bool IsRegistered(std::string path);
	static void RegisterPath(entt::entity e,std::string path);
	static void UnregisterPath(entt::entity e);
	static void UnregisterPath(std::string path);

	template<typename T>
	static void RegisterClassAsAsset() {
		entt::id_type hash = HelperFunctions::HashClassName<T>();
		entt::meta<T>().type(hash).template func<&ObjectPropertyRegister::CreateObjectAndReturnHandle<T>>(entt::hashed_string("Create"));
		m_RegisteredClassesByExtension[T::GetAssetExtension()] = HelperFunctions::GetClassName<T>();
	};


	static inline std::unordered_map<std::string, entt::entity> m_RegisteredAssetsByPath;
	static inline std::unordered_map<entt::entity,std::string> m_RegistererdPathsByAsset;
	static inline std::unordered_map<std::string, std::string> m_RegisteredClassesByExtension;


	template<typename ,typename>
	friend class AssetObjectSpecifier;

};

//Do Not Use!
template<typename Derived,typename Storage,typename ComponentName = ComponentHelpers::Null>
class AssetComponent : public ComponentSpecifier<ComponentName, AssetObjectSpecifier<Derived,Storage>> {

};

class AssetObject {
public:
	AssetObject(entt::entity e);

protected:
	virtual void OnExplorerIconUI() {};
private:
	void OnExplorerName();
	void OnExplorerRename();
	entt::entity m_Handle;

	friend class GuiLayer::ExplorerView;
	
};

template<typename Derived,typename Storage>
class AssetObjectSpecifier : public TaggedObject<AssetObjectSpecifier<Derived,Storage>, AssetComponent<Derived,Storage>,Storage> ,public AssetObject {
public:
	AssetObjectSpecifier(entt::entity e) : TaggedObject<AssetObjectSpecifier<Derived,Storage>, AssetComponent<Derived,Storage>, Storage>(e),AssetObject(e) {
		(void)dummy;
	}


	~AssetObjectSpecifier() {
		AssetRegister::UnregisterPath(this->ID());
	}

	void SetPath(std::string path) {
		AssetRegister::RegisterPath(this->ID(), path);
	}

private:
	static inline bool dummy = []() {
		AssetRegister::RegisterClassAsAsset<Derived>();
		return false;
	}();


};

