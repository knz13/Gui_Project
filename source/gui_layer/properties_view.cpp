#include "properties_view.h"
#include "../kv.h"

void GuiLayer::PropertiesView::Update(Window& win) {
    GuiLayer::SetupWindowStyle([&](ImGuiWindowFlags flags){
        ImGui::Begin("Properties",0,flags );
    });
    static bool randomAddress = false;

    

    if(GameView::AnyObjectSelected()){
        GameObject selected(GameView::AnyObjectSelected().objectID);
        selected.ShowProperties();
        
        
        if(ImGui::BeginPopupContextWindow(GuiLayer::GetImGuiID(&randomAddress).c_str(),ImGuiPopupFlags_MouseButtonRight)){
            
            
            if(ImGui::BeginMenu("Add Component")){
                for(auto& componentName : Object::GetRegisteredClasses()){
                    if(!selected.HasComponent(componentName)){
                        if(ImGui::MenuItem(componentName.c_str())){
                            selected.AddComponentByName(componentName);
                        }
                    }
                }
                ImGui::EndMenu();
            }

            ImGui::EndPopup();
        }
    }

    ImGui::End();
}
