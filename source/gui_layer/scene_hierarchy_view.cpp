#include "scene_hierarchy_view.h"
#include "../kv.h"

void GuiLayer::SceneHierarchyView::Setup(Window& win) {
    GuiLayer::SetupWindowStyle([&](ImGuiWindowFlags flags){
        ImGui::Begin("Scene Hierarchy",0,flags );
    });
    
    if(Registry::Get().alive() > 0){
        
        Registry::Get().each([&](const entt::entity e){
            Object obj(e);
            ImGui::SetNextItemOpen(true);
            if(ImGui::TreeNodeEx(obj.Properties().GetName().c_str())){
                
                if(ImGui::IsItemClicked(ImGuiMouseButton_Left)){
                    if(GameView::AnyObjectSelected()){
                        Object(GameView::AnyObjectSelected().objectID).Properties().SetHightlightState(false);
                    }
                    GameView::AnyObjectSelected() = ClickedObjectProperties(e);
                    Object(GameView::AnyObjectSelected().objectID).Properties().SetHightlightState(true);
                }
                ImGui::TreePop();
            }
            
        });

    }

    ImGui::End();
}
