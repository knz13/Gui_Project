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

	static void LoadAssetsForFolder(std::string folderPath);
	
	template<typename T>
	static bool IsPathRegisteredAs(std::string path) {
		if (!std::filesystem::exists(path)) {
			return false;
		}
		std::string ext = std::filesystem::path(path).extension().string();

		if (m_RegisteredClassByExtension.find(ext) != m_RegisteredClassByExtension.end()) {
			if (m_RegisteredClassByExtension[ext] == HelperFunctions::GetClassName<T>()) {
				return true;
			}
			return false;
		}
		return false;

	}

	static std::string GetRegisteredAssetForExtension(std::string extension);
	static bool CreateAssetAtFolder(std::string folder,std::string assetType);
	static ObjectHandle LoadAssetForPath(std::string path);
	static ObjectHandle GetAssetForPath(std::string path);
	static std::string GetPathFromAsset(ObjectHandle handle);
	static bool UnloadAsset(std::string path);
	static std::string GetExtensionForClass(std::string className);
	static bool IsAsset(std::string typeName);
	static bool PathHasRegisteredAsset(std::string path);
private:
	static void RegisterPath(entt::entity e,std::string path);
	static void UnregisterPath(entt::entity e);
	static void UnregisterPath(std::string path,bool shouldDelete=true);

	template<typename T>
	static void RegisterClassAsAsset() {
		entt::id_type hash = HelperFunctions::HashClassName<T>();
		entt::meta<T>().type(hash).template func<&ObjectPropertyRegister::CreateObjectAndReturnHandle<T>>(entt::hashed_string("Create"));
		entt::meta<T>().type(hash).template func<&CallOnExplorerUI<T>>(entt::hashed_string("Call Explorer UI"));
		entt::meta<T>().type(hash).template func<&CallOnRename<T>>(entt::hashed_string("Call Rename"));
		entt::meta<T>().type(hash).template func<&CallReadFile<T>>(entt::hashed_string("Call Read File"));
		entt::meta<T>().type(hash).template func<&CallSaveFile<T>>(entt::hashed_string("Call Save File"));
		entt::meta<T>().type(hash).template func<&CallRenameCreation<T>>(entt::hashed_string("Rename On Creation"));
		
		

		m_RegisteredAssetClasses.push_back(HelperFunctions::GetClassName<T>());

		auto vec = T::GetAssetExtensions();
		if(vec.size() != 0){	
			for(auto& ext : vec){
				m_RegisteredClassByExtension[ext] = HelperFunctions::GetClassName<T>();
			}
			m_RegisteredExtensionsByClass[HelperFunctions::GetClassName<T>()] = vec;
		}
	};

	template<typename T>
	static void CallRenameCreation(entt::entity e,std::string path) {
		T obj(e);

		((AssetObject*)(&obj))->SetRenameOnCreation(path);
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
	static void CallSaveFile(entt::entity e) {
		T obj(e);

		((AssetObject*)(&obj))->SaveFile();
	}

	template<typename T>
	static void CallReadFile(entt::entity e) {
		T obj(e);

		((AssetObject*)(&obj))->ReadFile();
	}

	
	
	static inline std::vector<std::string> m_RegisteredAssetClasses;
	static inline std::unordered_map<std::string, entt::entity> m_RegisteredAssetsByPath;
	static inline std::unordered_map<entt::entity,std::string> m_RegistererdPathsByAsset;
	static inline std::unordered_map<std::string, std::vector<std::string>> m_RegisteredExtensionsByClass;
	static inline std::unordered_map<std::string, std::string> m_RegisteredClassByExtension;


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
	virtual void SetRenameOnCreation(std::string) {};
	void SetPath(std::string path);
	virtual void ReadFile();
	virtual void SaveFile();
private:
	

	entt::entity m_Handle;

	friend class GuiLayer::ExplorerView;
	friend class AssetRegister;
	
};

class AssetObjectSpecifierStorage {
private:
	bool m_IsRenaming = false;
	std::string tempWord = "";
	bool m_DeleteIfNotRename = false;
	std::string m_CurrentDirectoryIfOneTimeOnly = "";

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
		return AssetRegister::GetPathFromAsset(ObjectHandle(this->ID()));
	}


protected:
	
	virtual void SetupExplorerIcon(ImVec2 size) {};
	virtual void OnDestroy() {};
	virtual void OnCreate() {};
	
	 
private:

	void SucceedRenaming(std::string newPath) {
		if (GetPath() != "") {
			AssetRegister::UnregisterPath(GetPath(), false);
		}
		

		AssetRegister::RegisterPath(this->ID(), newPath);
		HelperFunctions::CallMetaFunction(this->GetType(),"Call Read File",this->ID());
		this->Properties().SetName(GetPrivateStorage().tempWord);
		GetPrivateStorage().tempWord = "";
		GetPrivateStorage().m_IsRenaming = false;
		if (GuiLayer::ExplorerView::GetTempObject().ID() == this->ID()) {
			GuiLayer::ExplorerView::GetTempObject() = ObjectHandle();
		}
	}

	void SetRename() {
		GetPrivateStorage().m_IsRenaming = true;
		GetPrivateStorage().tempWord = this->Properties().GetName();
	}
	void SetRenameOnCreation(std::string currDir) final {
		SetRename();
		GetPrivateStorage().m_DeleteIfNotRename = true;
		GetPrivateStorage().m_CurrentDirectoryIfOneTimeOnly = currDir;
	}

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
		if (ImGui::Selectable(("##SelectableData" + to_string((uint32_t)this->ID())).c_str(), isSelected, ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowItemOverlap, size)) {
			
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && this->GetType() == "FolderAsset") {
				GuiLayer::ExplorerView::SetCurrentPath(GetPath());
			}

			GuiLayer::AnyObjectSelected() = ObjectHandle(this->ID());
		}

		if(ImGui::BeginPopupContextItem(GuiLayer::GetImGuiID(&GetPrivateStorage()).c_str())) {
			GuiLayer::ExplorerView::SetupDefaultPopupMenuWidgets();
			ImGui::Separator();
			if (ImGui::MenuItem("Rename")) {
				SetRename();
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Delete")) {
				AssetRegister::UnloadAsset(GetPath());
				std::filesystem::remove_all(GetPath());
			}

			ImGui::EndPopup();
		}

		ImGui::SetCursorPos(ImVec2(pos.x,pos.y + 4));


		size = ImVec2(size.x, size.y - ImGui::CalcTextSize("A").y - 10);

		SetupExplorerIcon(size);

		if (!GetPrivateStorage().m_IsRenaming) {
			ImGui::Text(GetMaxWord(this->Properties().GetName(), size.x).c_str());
		}
		else {
			//TODO: Make all this related to the explorerView and a method named GetRenamingObject() or something like that...
			ImGui::SetNextItemWidth(size.x);
			ImGui::SetKeyboardFocusHere();
			if (ImGui::InputText(GuiLayer::GetImGuiID(this).c_str(), &GetPrivateStorage().tempWord, ImGuiInputTextFlags_EnterReturnsTrue)) {
				if (GetPrivateStorage().tempWord == this->Properties().GetName()) {
					GetPrivateStorage().m_IsRenaming = false;
					return;
				}
				
				
				if (GetPrivateStorage().m_DeleteIfNotRename) {
					if (this->GetType() == "FolderAsset") {
						std::filesystem::create_directory(GetPrivateStorage().m_CurrentDirectoryIfOneTimeOnly + "/" + GetPrivateStorage().tempWord);
						this->SucceedRenaming(GetPrivateStorage().m_CurrentDirectoryIfOneTimeOnly + "/" + GetPrivateStorage().tempWord);
						return;
					}

					
					SucceedRenaming(GetPrivateStorage().m_CurrentDirectoryIfOneTimeOnly + "/" + GetPrivateStorage().tempWord + AssetRegister::GetExtensionForClass(this->GetType()));
					
					return;
				}

				std::string newPath;
				if (std::filesystem::is_directory(GetPath())) {
					newPath = std::filesystem::path(GetPath()).parent_path().string() + "/" + GetPrivateStorage().tempWord;
					
				}
				else {
					newPath = std::filesystem::path(GetPath()).parent_path().string() + "/" + GetPrivateStorage().tempWord + AssetRegister::GetExtensionForClass(this->GetType());
				}
				try {
					std::filesystem::rename(GetPath(), newPath);
				}
				catch (std::exception& e) {
					DEBUG_LOG(e.what());
					GetPrivateStorage().tempWord = "";
					GetPrivateStorage().m_IsRenaming = false;
					//TODO: fix
					return;
				}
				SucceedRenaming(newPath);
			}
			if (ImGui::IsKeyPressed(ImGuiKey_Escape, false)) {
				if (GetPrivateStorage().m_DeleteIfNotRename) {
					ObjectPropertyRegister::DeleteObject({this->ID()});
				}

				
			}
		}
	};;

	

	void Init() final {
		Registry::Get().emplace<AssetObjectSpecifierStorage>(this->ID());
		OnCreate();
	}
	void Destroy() final {
		HelperFunctions::CallMetaFunction(this->GetType(),"Call Save File",this->ID());
		Registry::Get().erase<AssetObjectSpecifierStorage>(this->ID());
		AssetRegister::UnregisterPath(this->ID());
		OnDestroy();

	}

	static inline bool dummy = []() {
		AssetRegister::RegisterClassAsAsset<Derived>();
		return false;
	}();


};

