#include "scene_hierarchy_view.h"
#include "../kv.h"

void GuiLayer::SceneHierarchyView::Update(Window& win) {
    static bool vPressed = false;
    GuiLayer::SetupWindowStyle([&](ImGuiWindowFlags flags){
        ImGui::Begin("Scene Hierarchy",0,flags );
    });

    std::string clipboardText = ImGui::GetClipboardText();
    if(clipboardText.size() > 5 && clipboardText.substr(0,4) == "Copy" && ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_V) && !vPressed){
        entt::entity id = (entt::entity)std::stoll(clipboardText.substr(5));
        Registry::CopyEntity(Object(id));
        vPressed = true;
    }



    if(ImGui::IsKeyReleased(ImGuiKey_V)){
        vPressed= false;
    }
    
    if(Registry::Get().alive() > 0){
        
        Registry::Get().each([&](const entt::entity e){
            Object obj(e);
            ImGui::SetNextItemOpen(true);
            if(ImGui::TreeNodeEx(obj.Properties().GetName().c_str(),ImGuiTreeNodeFlags_SpanFullWidth)){
                
                if(ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_C) && ImGui::IsItemHovered()){
                        ImGui::SetClipboardText(("Copy " + std::to_string((uint32_t)e)).c_str());
                }
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
