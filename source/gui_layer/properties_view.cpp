#include "properties_view.h"
#include "../kv.h"

void GuiLayer::PropertiesView::Update(Window& win) {
    GuiLayer::SetupWindowStyle([&](ImGuiWindowFlags flags){
        ImGui::Begin("Properties",0,flags );
    });
    static bool randomAddress = false;

    

    if(GameView::AnyObjectSelected()){
        Object selected(GameView::AnyObjectSelected().objectID);
        selected.Properties().CallShowPropertiesFunctions();
        
        
        if(ImGui::BeginPopupContextWindow(GuiLayer::GetImGuiID(&randomAddress).c_str(),ImGuiPopupFlags_MouseButtonRight)){
                
            
            if(ImGui::BeginMenu("Add Component")){
                for(auto& componentName : Object::GetRegisteredClasses()){
                    if(!selected.HasComponent(componentName)){
                        if(ImGui::MenuItem(componentName.c_str())){
                            selected.TryAddComponent(componentName);
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
