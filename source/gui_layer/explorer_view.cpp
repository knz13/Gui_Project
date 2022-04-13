#include "explorer_view.h"
#include "../kv.h"

void GuiLayer::ExplorerView::Update(Window& win) {
    static const std::string InitialPath = std::filesystem::current_path().string();
    static std::string currentPath = std::filesystem::current_path().string();
    std::replace(currentPath.begin(),currentPath.end(),'\\','/');

    GuiLayer::SetupWindowStyle([&](ImGuiWindowFlags flags){
        ImGui::Begin("Explorer",0,flags );
    });
    
    
    std::vector<std::string> vec = HelperFunctions::SplitString(currentPath,"/");
    if(vec.size() == 0){
        vec.push_back(currentPath);
    }
    ImGui::BeginChild("fileExplorerPathSelector",ImVec2(0,40),true,ImGuiWindowFlags_HorizontalScrollbar);
    int index = 0;
    for(auto& directory : vec) {
        

        
        if(ImGui::Button(directory.c_str())){
            std::string newPath = "";
            for(int i = 0;i < vec.size(); i++){
                newPath += vec[i];
                if(i==index){
                    break;
                }
                newPath += "/";

            }
            currentPath = newPath;
            break;
        }
        
        if(index != vec.size() - 1){
            ImGui::SameLine();
        }
        index++;
    }
    ImGui::EndChild();


    ImGui::BeginChild("fileExplorer",ImVec2(0,0),true);
    if(ImGui::BeginPopupContextWindow(("ResetPathMenu" + GuiLayer::GetImGuiID(&win)).c_str())){
        if(ImGui::MenuItem("Reset To Default Path")){
            currentPath = InitialPath;
        }
        ImGui::EndPopup();
    }

    if(*(currentPath.end() - 1) == ':'){
        currentPath.push_back('/');
    }

    if(ImGui::BeginTable(("Explorer View" + GuiLayer::GetImGuiID(&win)).c_str(),6,ImGuiTableFlags_NoKeepColumnsVisible)){
        for(auto& file : std::filesystem::directory_iterator(currentPath)){
            ImGui::TableNextColumn();
            if(ImGui::Button(file.path().filename().string().c_str(),ImVec2(-FLT_MIN,0))){
                if(std::filesystem::is_directory(file.path())){
                    currentPath = file.path().parent_path().string() + "/" + file.path().filename().string();
                }
            }
        }
        ImGui::EndTable();
    }


    ImGui::EndChild();

    ImGui::End();
}
