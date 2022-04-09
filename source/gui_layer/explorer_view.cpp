#include "explorer_view.h"
#include "../kv.h"

void GuiLayer::ExplorerView::Setup(Window& win) {
    static std::string currentPath = std::filesystem::current_path().string();
    std::replace(currentPath.begin(),currentPath.end(),'\\','/');

    GuiLayer::SetupWindowStyle([&](ImGuiWindowFlags flags){
        ImGui::Begin("Explorer",0,flags );
    });
    
    std::vector<std::string> vec = HelperFunctions::SplitString(currentPath,"/");

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

    ImGui::BeginChild("fileExplorer",ImVec2(0,0),true);



    ImGui::EndChild();

    ImGui::End();
}
