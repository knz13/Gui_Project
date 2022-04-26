#pragma once
#include "../object/tagged_object.h"
#include "../gui_layer/gui_useful_implementations.h"
#include "../gui_layer/gui_layer.h"
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
		entt::meta<T>().type(hash).template func<&CallOnExplorerUI<T>>(entt::hashed_string("Call Explorer UI"));
		entt::meta<T>().type(hash).template func<&CallOnRename<T>>(entt::hashed_string("Call Rename"));
		entt::meta<T>().type(hash).template func<&CallReadFile<T>>(entt::hashed_string("Call Read File"));
		auto vec = T::GetAssetExtensions();
		for (auto& extension : vec) {
			m_RegisteredClassesByExtension[extension] = HelperFunctions::GetClassName<T>();
		}
	};

	template<typename T>
	static void CallOnExplorerUI(entt::entity e,ImVec2 size) {
		T obj(e);

		((AssetObject*)(&obj))->OnExplorerUI(size);
	}

	template<typename T>
	static void CallOnRename(entt::entity e) {
		T obj(e);

		((AssetObject*)(&obj))->OnRenameCall();
	}

	template<typename T>
	static void CallReadFile(entt::entity e) {
		T obj(e);

		((AssetObject*)(&obj))->ReadFile();
	}

	
	

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
	void ShowOnExplorer(ImVec2 size);
	void Rename();
protected:
	virtual void OnExplorerUI(ImVec2 size) {};
	virtual void OnRenameCall() {};
	
private:
	virtual void ReadFile();
	void SetPath(std::string path);
	
	entt::entity m_Handle;

	friend class GuiLayer::ExplorerView;
	friend class AssetRegister;
	
};

class AssetObjectSpecifierStorage {
private:
	bool m_IsRenaming = false;
	std::string tempWord = "";

	template<typename,typename>
	friend class AssetObjectSpecifier;
};

template<typename Derived,typename Storage>
class AssetObjectSpecifier : public TaggedObject<Derived, AssetComponent<Derived,Storage>,Storage> ,public AssetObject {
public:
	AssetObjectSpecifier(entt::entity e) : TaggedObject<Derived, AssetComponent<Derived,Storage>, Storage>(e),AssetObject(e) {
		(void)dummy;
	}

	//virtual void Serialize(ryml::NodeRef mainNode) = 0;
	//virtual void Deserialize(ryml::NodeRef mainNode) = 0;

	

	std::string GetPath() {
		return AssetRegister::GetPathFromObject(ObjectHandle(this->ID()));
	}
	 
private:
	std::string GetMaxWord(std::string word,int widthMax) {
		while (ImGui::CalcTextSize(word.c_str()).x > widthMax) {
			HelperFunctions::EraseWordFromString(word, "...");
			word = word.substr(0,word.size()-2) + "...";
		}
		return word;
	}

	void OnRenameCall() final {
		GetPrivateStorage().m_IsRenaming = true;
		GetPrivateStorage().tempWord = Object::Properties().GetName();
	}

	AssetObjectSpecifierStorage& GetPrivateStorage() {
		return Registry::Get().get<AssetObjectSpecifierStorage>(this->ID());
	}

	void OnExplorerUI(ImVec2 size) final {

		const bool isSelected = GuiLayer::AnyObjectSelected() == this->ID();

		ImVec2 pos = ImGui::GetCursorPos();
		if (ImGui::Selectable(("##SelectableData" + to_string((uint32_t)this->ID())).c_str(), isSelected, 0, size)) {
			GuiLayer::AnyObjectSelected() = ObjectHandle(this->ID());
		}

		ImGui::SetCursorPos(pos);

		if (!GetPrivateStorage().m_IsRenaming) {
			ImGui::Text(GetMaxWord(this->Properties().GetName(), size.x).c_str());
		}
		else {

		}
	};;

	void Init() final {
		Registry::Get().emplace<AssetObjectSpecifierStorage>(this->ID());
	}
	void Destroy() final {
		AssetRegister::UnregisterPath(this->ID());

	}

	static inline bool dummy = []() {
		AssetRegister::RegisterClassAsAsset<Derived>();
		return false;
	}();


};

