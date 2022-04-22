#include "explorer_view.h"
#include "../kv.h"

void GuiLayer::ExplorerView::Update(Window& win) {
    static const std::string InitialPath = std::filesystem::current_path().string() + "/Assets";
    static std::string currentPath = std::filesystem::current_path().string() + "/Assets";
    std::replace(currentPath.begin(),currentPath.end(),'\\','/');

    GuiLayer::SetupWindowStyle([&](ImGuiWindowFlags flags){
        ImGui::Begin("Explorer",0,flags );
    });
    
    
    


    ImGui::BeginChild("fileExplorer",ImVec2(0,0),true);
    if(ImGui::BeginPopupContextWindow(("ResetPathMenu" + GuiLayer::GetImGuiID(&win)).c_str())){
        if(ImGui::MenuItem("Return to Assets")){
            currentPath = InitialPath;
        }
        if (ImGui::BeginMenu("Create")) {
            if (ImGui::MenuItem("Folder")) {
                
            }
        }
        ImGui::EndPopup();
    }

    if(ImGui::BeginTable(("Explorer View" + GuiLayer::GetImGuiID(&win)).c_str(),6,ImGuiTableFlags_NoKeepColumnsVisible)){
        for(auto& file : std::filesystem::directory_iterator(currentPath)){
            ImGui::TableNextColumn();
            if (std::hash<std::string>()(file.path().filename().string().c_str()) != m_CurrentRenamingFileHash) {
                
            }
        }
        ImGui::EndTable();
    }


    ImGui::EndChild();

    ImGui::End();
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