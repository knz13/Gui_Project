#include "explorer_view.h"
#include "../kv.h"
#include "../assets/asset_object.h"

void GuiLayer::ExplorerView::Update(Window& win) {
    
    std::replace(m_CurrentPath.begin(),m_CurrentPath.end(),'\\','/');

    GuiLayer::SetupWindowStyle("Explorer",[&](ImGuiWindowFlags flags){
        ImGui::Begin("Explorer",0,flags );
    });
    
    
    


    ImGui::BeginChild("fileExplorer",ImVec2(0,0),true);
    if(ImGui::BeginPopupContextWindow(("ResetPathMenu" + GuiLayer::GetImGuiID(&win)).c_str())){
        if(ImGui::MenuItem("Return to Assets")){
            m_CurrentPath = m_InitialPath;
        }
        ImGui::EndPopup();
    }
    int index = 0;
    
    int columns = ImGui::GetWindowSize().x / m_WidgetSize.x;
    if (columns == 0) {
        columns = 1;
    }
    
    ImGuiTableFlags tableFlags =  ImGuiTableFlags_NoHostExtendX;

    int fileCount = 0;
    for (auto& file : std::filesystem::directory_iterator(m_CurrentPath)) {
        fileCount++;
    }
    if (fileCount <= columns) {
        tableFlags |= ImGuiTableFlags_SizingFixedFit;
    }
    ImGui::SetCursorPosX(10);
    if(ImGui::BeginTable(("Explorer View" + GuiLayer::GetImGuiID(&win)).c_str(), columns, tableFlags)) {
        
        for(auto& file : std::filesystem::directory_iterator(m_CurrentPath)){
            if (!AssetRegister::GetAssetForPath(file.path().string())) {
                continue;
            }
            ImGui::TableNextColumn();
            AssetObject asset = AssetRegister::GetAssetForPath(file.path().string()).GetAs<AssetObject>();
            
            asset.ShowOnExplorer(ImVec2(m_WidgetSize.x-10,m_WidgetSize.y));
            
            index++;
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

    for (auto& file : std::filesystem::directory_iterator(m_CurrentPath)) {
        if (!AssetRegister::GetAssetForPath(file.path().string())) {
            AssetObject asset = AssetRegister::LoadAssetForPath(file.path().string()).GetAs<AssetObject>();
            asset.InitializeFile();
            m_CurrentFilesByFolder[m_CurrentPath].push_back(file.path().string());
        }
    }

    
    win.Events().FocusEvent().Connect([](Window& window, bool focused) {
        if (focused) {
            OnUpdatePathOrReload();
        }
    });
    
}

void GuiLayer::ExplorerView::SetCurrentPath(std::string path)
{
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
            
            continue;
        }
        vec.push_back(file.path().string());
    }
    for (auto& path : m_CurrentFilesByFolder[m_CurrentPath]) {
        if (auto it = std::find(vec.begin(), vec.end(), path); it == vec.end()) {
            DEBUG_LOG("Unloading Asset " + *it + " because it was not found in current directory!");
            AssetRegister::UnloadAsset(*it);
        }
    }
    m_CurrentFilesByFolder[m_CurrentPath] = vec;
}

/*
if (ImGui::Button(file.path().filename().string().c_str(), ImVec2(-FLT_MIN, 0))) {
    if (std::filesystem::is_directory(file.path())) {
        m_CurrentPath = file.path().parent_path().string() + "/" + file.path().filename().string();
    }
}
*/