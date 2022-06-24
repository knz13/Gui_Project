#pragma once
#include "../gui_layer/gui_useful_implementations.h"
#include "../gui_layer/gui_layer.h"
#include "../object/base_object.h"
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
	static ecspp::ObjectHandle LoadAssetForPath(std::string path);
	static ecspp::ObjectHandle GetAssetForPath(std::string path);
	static std::string GetPathFromAsset(ecspp::ObjectHandle handle);
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
		ecspp::meta<T>().template RegisterFunc<&CallOnExplorerUI<T>>("Call Explorer UI");
		ecspp::meta<T>().template RegisterFunc<&CallOnRename<T>>("Call Rename");
		ecspp::meta<T>().template RegisterFunc<&CallReadFile<T>>("Call Read File");
		ecspp::meta<T>().template RegisterFunc<&CallSaveFile<T>>("Call Save File");
		ecspp::meta<T>().template RegisterFunc<&CallRenameCreation<T>>("Rename On Creation");		
		ecspp::meta<T>().template RegisterFunc<&CallMoveToDir<T>>("Call Move To Dir");		


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

	template<typename T>
	static void CallMoveToDir(entt::entity e,std::string path) {
		T obj(e);

		((AssetObject*)(&obj))->MoveToDir(path);
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

class AssetObject {
public:
	AssetObject(entt::entity e);
	~AssetObject();

	void InitializeFile();
	void ShowOnExplorer(ImVec2 size);
	void MoveTo(std::string path);
	void Rename();
	std::string GetPath() {
		return AssetRegister::GetPathFromAsset(ecspp::ObjectHandle(m_Handle));
	}
protected:
	virtual void OnExplorerUI(ImVec2 size) {};
	virtual void OnRenameCall() {};
	virtual void SetRenameOnCreation(std::string) {};
	void SetPath(std::string path);
	virtual void ReadFile();
	virtual void SaveFile();
	virtual void MoveToDir(std::string path) {};
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
class AssetObjectSpecifier : 
	public ComponentlessBaseObject<Derived>,
	public ecspp::RegisterStorage<Derived,Storage>,
	public AssetObject {
public:
	AssetObjectSpecifier(entt::entity e) : ComponentlessBaseObject<Derived>(e), AssetObject(e), ecspp::RegisterStorage<Derived,Storage>(e) {
		(void)dummy;
	}

	//virtual void Serialize(ryml::NodeRef mainNode) = 0;
	//virtual void Deserialize(ryml::NodeRef mainNode) = 0;


protected:
	void ShowProperties() override {
		SetupExplorerIcon(ImVec2(60,60));
		ImGui::SameLine();
		ImGui::TextWrapped((std::filesystem::path(GetPath()).stem().string() + " (" + HelperFunctions::GetClassName<Derived>() + ")").c_str());

		ImGui::Separator();

		OnShowProperties();

	
	};
	virtual void SetupExplorerIcon(ImVec2 size) {};
	virtual void OnDestroy() {};
	virtual void OnCreate() {};
	virtual void OnShowProperties() {};
	 
private:

	void MoveToDir(std::string path) override {

		
		try {
			std::filesystem::rename(GetPath(), path);
		}
		catch (std::exception& e) {
			DEBUG_LOG(e.what());
			return;
		}

		if (GetPath() != "") {
			AssetRegister::UnregisterPath(GetPath(), false);
		}

		AssetRegister::RegisterPath(this->ID(), path);



	}

	void SucceedRenaming(std::string newPath)  {
		
		if (GetPath() != "") {
			AssetRegister::UnregisterPath(GetPath(), false);
		}

		AssetRegister::RegisterPath(this->ID(), newPath);
		this->SetName(GetPrivateStorage().tempWord);

		GetPrivateStorage().tempWord = "";
		GetPrivateStorage().m_IsRenaming = false;
		if (GuiLayer::ExplorerView::GetTempObject().ID() == this->ID()) {
			GuiLayer::ExplorerView::GetTempObject() = ecspp::ObjectHandle();
		}
	}

	void SetRename() {
		GetPrivateStorage().m_IsRenaming = true;
		GetPrivateStorage().tempWord = this->GetName();
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
		GetPrivateStorage().tempWord =	this->GetName();
	}

	AssetObjectSpecifierStorage& GetPrivateStorage() {
		return ecspp::Registry().get<AssetObjectSpecifierStorage>(this->ID());
	}

	void OnExplorerUI(ImVec2 size) final {

		const bool isSelected = GuiLayer::AnyObjectSelected() == this->ID();

		ImVec2 pos = ImGui::GetCursorPos();
		if (ImGui::Selectable(("##SelectableData" + to_string((uint32_t)this->ID())).c_str(), isSelected, ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowItemOverlap, size)) {
			
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && this->GetType() == "FolderAsset") {
				GuiLayer::ExplorerView::SetCurrentPath(GetPath());
			}

			GuiLayer::AnyObjectSelected() = ecspp::ObjectHandle(this->ID());
		}

		size = ImVec2(size.x, size.y - ImGui::CalcTextSize("A").y - 10);
		if (ImGui::BeginDragDropSource()) {
			SetupExplorerIcon(size);

			ImGui::Text(this->GetName().c_str());

			ImGui::SetDragDropPayload(this->GetType().c_str(), &this->ID(), sizeof(entt::entity));
			ImGui::SetDragDropPayload("AllAssets", &this->ID(), sizeof(entt::entity));

			ImGui::EndDragDropSource();

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

		SetupExplorerIcon(size);

		if (!GetPrivateStorage().m_IsRenaming) {
			ImGui::Text(GetMaxWord(this->GetName(), size.x).c_str());
		}
		else {
			//TODO: Make all this related to the explorerView and a method named GetRenamingObject() or something like that...
			ImGui::SetNextItemWidth(size.x);
			ImGui::SetKeyboardFocusHere();
			if (ImGui::InputText(GuiLayer::GetImGuiID(this).c_str(), &GetPrivateStorage().tempWord, ImGuiInputTextFlags_EnterReturnsTrue)) {
				if (GetPrivateStorage().tempWord == this->GetName()) {
					GetPrivateStorage().m_IsRenaming = false;
					return;
				}
				
				
				if (GetPrivateStorage().m_DeleteIfNotRename) {
					SucceedRenaming(GetPrivateStorage().m_CurrentDirectoryIfOneTimeOnly + "/" + GetPrivateStorage().tempWord + AssetRegister::GetExtensionForClass(this->GetType()));
					return;
				}

				std::string newPath;
				
				newPath = std::filesystem::path(GetPath()).parent_path().string() + "/" + GetPrivateStorage().tempWord + AssetRegister::GetExtensionForClass(this->GetType());
				
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
					ecspp::DeleteObject(ecspp::ObjectHandle(this->ID()));
				}

				
			}
		}
	};;

	

	void Init() final {
		ecspp::Registry().emplace<AssetObjectSpecifierStorage>(this->ID());
		OnCreate();
	}
	void Destroy() final {
		HelperFunctions::CallMetaFunction(this->GetType(),"Call Save File",this->ID());
		ecspp::Registry().erase<AssetObjectSpecifierStorage>(this->ID());
		AssetRegister::UnregisterPath(this->ID());
		OnDestroy();

	}

	static inline bool dummy = []() {
		AssetRegister::RegisterClassAsAsset<Derived>();
		return false;
	}();


};

