#pragma once
#include "../object/tagged_object.h"
#include "../gui_layer/gui_useful_implementations.h"
#include <filesystem>

namespace GuiLayer {
	class ExplorerView;
}

template<typename>
class AssetObjectSpecifier;

class AssetRegister {

	static void CreateObjectsForFolder(std::string folderPath);

private:

	static bool IsRegistered(std::string path);
	static void RegisterPath(entt::entity e,std::string path);
	static void UnregisterPath(entt::entity e);
	static void UnregisterPath(std::string path);

	static inline std::unordered_map<std::string, entt::entity> m_RegisteredAssetsByPath;
	static inline std::unordered_map<entt::entity,std::string> m_RegistererdPathsByAsset;

	template<typename>
	friend class AssetObjectSpecifier;

};

//Do Not Use!
template<typename Storage,typename ComponentName = ComponentHelpers::Null>
class AssetComponent : public ComponentSpecifier<ComponentName, AssetObjectSpecifier<Storage>> {

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

template<typename Storage>
class AssetObjectSpecifier : public TaggedObject<AssetObjectSpecifier<Storage>, AssetComponent<Storage>,Storage> ,public AssetObject {
public:
	AssetObjectSpecifier(entt::entity e,std::string path) : TaggedObject<AssetObjectSpecifier<Storage>, AssetComponent<Storage>, Storage>(e) {
		AssetRegister::RegisterPath(e,path);
	}


	~AssetObjectSpecifier() {
		AssetRegister::UnregisterPath(this->ID());
	}




};

