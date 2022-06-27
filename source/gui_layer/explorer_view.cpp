#include "explorer_view.h"
#include "../kv.h"
#include "../assets/asset_object.h"

void GuiLayer::ExplorerView::Update(Window& win) {
    
    std::replace(m_CurrentPath.begin(),m_CurrentPath.end(),'\\','/');

    
    GuiLayer::SetupWindowStyle("Explorer",[&](ImGuiWindowFlags flags){
        ImGui::Begin("Explorer",0,flags );
    });
    
    
    


    ImGui::BeginChild("fileExplorer",ImVec2(0,-1),true,ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);
    
    int index = 0;
    
    
    ImGui::SetCursorPosX(10);

    if (ImGui::BeginTable(("ExplorerMain" + GuiLayer::GetImGuiID(&win)).c_str(), 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
        ImGui::TableSetupColumn("##ExplorerFolderSelection",ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel,0.3);
        
        
        ImGui::TableNextColumn();

        ShowAllSubFolders(m_InitialPath);


        
        ImGui::TableNextColumn();
        
        

        if (ImGui::BeginTable("Explorer View Outside", 1)) {
            


            ImGui::TableNextColumn();

            SetupFolderExplorerAboveFileExplorer();

            ImGui::TableNextColumn();

            int columns = ImGui::GetContentRegionAvail().x / m_WidgetSize.x;
            if (columns == 0) {
                columns = 1;
            }

            ImGuiTableFlags tableFlags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuter;

            int fileCount = 0;
            for (auto& file : std::filesystem::directory_iterator(m_CurrentPath)) {
                fileCount++;
            }
            if (fileCount <= columns) {
                tableFlags |= ImGuiTableFlags_SizingFixedFit;
            }
            
            ImGui::BeginChild("childWindowExplorer", ImGui::GetContentRegionAvail());
            
            bool explorerView;

            GuiLayer::SetupChildStyle([&]() {
                ImVec2 explorerSize = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 10);
                explorerView = ImGui::BeginTable(("Explorer View" + GuiLayer::GetImGuiID(&win)).c_str(), columns, tableFlags, explorerSize);
                });

            if (ImGui::BeginPopupContextWindow("ExplorerViewChildWindowPopup")) {
                SetupDefaultPopupMenuWidgets();
                ImGui::EndPopup();
            }
            


            if (explorerView) {
                for (auto& file : std::filesystem::directory_iterator(m_CurrentPath)) {
                    if (!AssetRegister::GetAssetForPath(file.path().string())) {
                        continue;
                    }
                    ImGui::TableNextColumn();
                    AssetObject asset = AssetRegister::GetAssetForPath(file.path().string()).GetAs<AssetObject>();

                    asset.ShowOnExplorer(ImVec2(m_WidgetSize.x - 10, m_WidgetSize.y));

                    index++;
                }

                if (m_CurrentCreatingObject) {
                    if (AssetRegister::IsAsset(m_CurrentCreatingObject.GetAsObject().GetType())) {
                        ImGui::TableNextColumn();
                        m_CurrentCreatingObject.GetAs<AssetObject>().ShowOnExplorer(ImVec2(m_WidgetSize.x - 10, m_WidgetSize.y));
                    }
                }

                ImGui::EndTable();
            }


            ImGui::EndChild();

            

            ImGui::EndTable();
        }
            
        
        ImGui::EndTable();
    }


    ImGui::EndChild();

    ImGui::End();

    //ImGui::ShowDemoWindow();
}

void GuiLayer::ExplorerView::Setup(Window& win)
{
    std::replace(m_CurrentPath.begin(), m_CurrentPath.end(), '\\', '/');
    if (!std::filesystem::exists("Assets")) {
        std::filesystem::create_directory("Assets");
    }

    SetCurrentPath(m_InitialPath);

    
    win.Events().FocusEvent().Connect([](Window& window, bool focused) {
        if (focused) {
            OnUpdatePathOrReload();
        }
    });

    GameObject camera = GameObject::CreateNew("Explorer Camera");
    camera.AddComponent<Camera>();
    camera.AddComponent<InternalUse>();

    m_ExplorerCamera = camera;
    
}

void GuiLayer::ExplorerView::Reload()
{
    OnUpdatePathOrReload();
}

void GuiLayer::ExplorerView::SetCurrentPath(std::string path)
{
    std::replace(path.begin(), path.end(), '\\', '/');
    if (path != m_CurrentPath) {
        m_CurrentPath = path;
        OnUpdatePathOrReload();
    }
}

void GuiLayer::ExplorerView::OnUpdatePathOrReload()
{
    std::vector<std::string> vec;
    for (auto& file : std::filesystem::directory_iterator(m_CurrentPath)) {
        if (!AssetRegister::GetAssetForPath(file.path().string())) {
            AssetObject asset = AssetRegister::LoadAssetForPath(file.path().string()).GetAs<AssetObject>();
            asset.InitializeFile();
            
            
        }
        vec.push_back(file.path().string());
    }

    

    for (auto& path : m_CurrentFilesByFolder[m_CurrentPath]) {
        if (auto it = std::find(vec.begin(), vec.end(), path); it == vec.end()) {
            if (!AssetRegister::PathHasRegisteredAsset(path)) {
                continue;
            }
            DEBUG_LOG("Unloading Asset " + path + " because it was not found in current directory!");
            AssetRegister::UnloadAsset(path);
        }
    }
    m_CurrentFilesByFolder[m_CurrentPath] = vec;
}

void GuiLayer::ExplorerView::ShowAllSubFolders(std::string current)
{


    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;

    if (std::filesystem::path(current) == std::filesystem::path(m_CurrentPath)) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    if (!std::filesystem::is_directory(current)) {
        return;
    }

    if (!AssetRegister::IsPathRegisteredAs<FolderAsset>(current)) {
        return;
    }
    
    if (!FolderHasFoldersInside(current)) {

        bool open = ImGui::TreeNodeEx((std::filesystem::path(current).stem().string() + GuiLayer::GetImGuiID(&m_CurrentFilesByFolder)).c_str(), flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanFullWidth);
        
        if (open) {
            ImGui::TreePop();
        }
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
            SetCurrentPath(current);
        }
        return;
    }
    else {
        
        if (ImGui::TreeNodeEx((std::filesystem::path(current).stem().string() + GuiLayer::GetImGuiID(&m_CurrentFilesByFolder)).c_str(), flags | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnArrow)) {
            if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && !ImGui::IsItemToggledOpen()) {
                SetCurrentPath(current);
            }
            for (auto& file : std::filesystem::directory_iterator(current)) {
                
                if (std::filesystem::path(current) == std::filesystem::current_path() && file.path().stem().string() != "Assets") {
                    continue;
                }
                ShowAllSubFolders(file.path().string());
            }
            ImGui::TreePop();
        }

        
        
    }



}

bool GuiLayer::ExplorerView::FolderHasFoldersInside(std::string folderPath)
{
    if (!std::filesystem::exists(folderPath)) {
        return false;
    }
    if (!std::filesystem::is_directory(folderPath)) {
        return false;
    }

    for (auto& file : std::filesystem::directory_iterator(folderPath)) {
        if (file.is_directory()) {
            return true;
        }

    }
    return false;
}

void GuiLayer::ExplorerView::SetupDefaultPopupMenuWidgets()
{
    if (ImGui::BeginMenu("Create")) {

        if (ImGui::MenuItem("Folder")) {
            AssetRegister::CreateAssetAtFolder(m_CurrentPath,"FolderAsset");
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Render Texture")) {
            AssetRegister::CreateAssetAtFolder(m_CurrentPath,"RenderTextureAsset");
        }
        if (ImGui::MenuItem("Shader")) {
            AssetRegister::CreateAssetAtFolder(m_CurrentPath,"ShaderAsset");
        }
        if (ImGui::MenuItem("Scene")) {
            AssetRegister::CreateAssetAtFolder(m_CurrentPath, "SceneAsset");
        }


        ImGui::EndMenu();
    }
}

GameObject GuiLayer::ExplorerView::GetExplorerCamera()
{
    return m_ExplorerCamera.GetAs<GameObject>();
}



ecspp::ObjectHandle& GuiLayer::ExplorerView::GetTempObject()
{
    return m_CurrentCreatingObject;
}

void GuiLayer::ExplorerView::SetupFolderExplorerAboveFileExplorer()
{
    std::string path = std::filesystem::path(m_CurrentPath).lexically_relative(std::filesystem::current_path()).string();
    auto vec = HelperFunctions::SplitString(path, "\\");

    auto sumItemsUntil = [&](std::vector<std::string>& vector, std::string limiter) {
        std::string path = std::filesystem::current_path().string();
        std::replace(path.begin(), path.end(), '\\', '/');
        path += "/";

        for (auto& file : vector) {

            if (file != limiter) {
                path += file;
                path += "/";

            }
            else {
                path += file;
                break;

            }
        }
        
        return path;
    };

    int index = 0;
    for (auto& folder : vec) {
        ImGui::Text(folder.c_str());

        if (ImGui::BeginDragDropTarget()) {
            const auto* payload = ImGui::AcceptDragDropPayload("AllAssets");

            if (payload) {
                entt::entity e = *(entt::entity*)payload->Data;
                if (ecspp::ObjectHandle(e)) {
                    AssetObject(e).MoveTo(sumItemsUntil(vec,"Assets") + "/" + std::filesystem::path(AssetObject(e).GetPath()).filename().string());
                }
            }

            ImGui::EndDragDropTarget();
        }

        if (ImGui::IsItemClicked()) {
            std::string finalPath = sumItemsUntil(vec,folder);
            SetCurrentPath(finalPath);
        }
        if (index != vec.size() - 1) {
            ImGui::SameLine();

            ImGui::SmallButton(">");

            if (ImGui::BeginPopupContextItem(("PopupFor" + folder).c_str(),ImGuiPopupFlags_MouseButtonLeft)) {
                for (auto& file : std::filesystem::directory_iterator(sumItemsUntil(vec, folder))) {
                    if (file.is_directory() && AssetRegister::IsPathRegisteredAs<FolderAsset>(file.path().string())) {
                        if (ImGui::MenuItem(file.path().stem().string().c_str(), "", std::filesystem::path(m_CurrentPath) == file.path())) {
                            SetCurrentPath(file.path().string());
                        }
                    }
                }

                ImGui::EndPopup();
            }
                
            ImGui::SameLine();
        }
        index++;
    }
}

/*
if (ImGui::Button(file.path().filename().string().c_str(), ImVec2(-FLT_MIN, 0))) {
    if (std::filesystem::is_directory(file.path())) {
        m_CurrentPath = file.path().parent_path().string() + "/" + file.path().filename().string();
    }
}
*/