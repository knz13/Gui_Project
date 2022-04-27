#include "explorer_view.h"
#include "../kv.h"
#include "../assets/asset_object.h"

void GuiLayer::ExplorerView::Update(Window& win) {
    static const std::string InitialPath = std::filesystem::current_path().string() + "/Assets";
    static std::string currentPath = std::filesystem::current_path().string() + "/Assets";
    std::replace(currentPath.begin(),currentPath.end(),'\\','/');

    GuiLayer::SetupWindowStyle("Explorer",[&](ImGuiWindowFlags flags){
        ImGui::Begin("Explorer",0,flags );
    });
    
    
    


    ImGui::BeginChild("fileExplorer",ImVec2(0,0),true);
    if(ImGui::BeginPopupContextWindow(("ResetPathMenu" + GuiLayer::GetImGuiID(&win)).c_str())){
        if(ImGui::MenuItem("Return to Assets")){
            currentPath = InitialPath;
        }
        ImGui::EndPopup();
    }
    int index = 0;
    
    int columns = ImGui::GetWindowSize().x / m_WidgetSize.x;
    if (columns == 0) {
        columns = 1;
    }
    
    ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersOuter | ImGuiTableFlags_NoHostExtendX;

    int fileCount = 0;
    for (auto& file : std::filesystem::directory_iterator(currentPath)) {
        fileCount++;
    }
    if (fileCount <= columns) {
        tableFlags |= ImGuiTableFlags_SizingFixedFit;
    }

    if(ImGui::BeginTable(("Explorer View" + GuiLayer::GetImGuiID(&win)).c_str(), columns, tableFlags)) {
        
        for(auto& file : std::filesystem::directory_iterator(currentPath)){
            ImGui::TableNextColumn();
            if (!AssetRegister::GetObjectForPath(file.path().string())) {
                AssetObject asset = AssetRegister::CreateObjectForPath(file.path().string()).GetAs<AssetObject>();
                asset.InitializeFile();
            }

            AssetObject asset = AssetRegister::GetObjectForPath(file.path().string()).GetAs<AssetObject>();
            
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
    if (!std::filesystem::exists("Assets")) {
        std::filesystem::create_directory("Assets");
    }
}

/*
if (ImGui::Button(file.path().filename().string().c_str(), ImVec2(-FLT_MIN, 0))) {
    if (std::filesystem::is_directory(file.path())) {
        currentPath = file.path().parent_path().string() + "/" + file.path().filename().string();
    }
}
*/