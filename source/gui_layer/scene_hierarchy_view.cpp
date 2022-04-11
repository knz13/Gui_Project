#include "scene_hierarchy_view.h"
#include "../kv.h"

void GuiLayer::SceneHierarchyView::Update(Window& win) {
    
    GuiLayer::SetupWindowStyle([&](ImGuiWindowFlags flags){
        ImGui::Begin("Scene Hierarchy",0,flags );
    });

    


   
    if(Registry::Get().alive() > 0){
        
        Registry::Get().each([&](const entt::entity e){
            Object obj(e);
            if(obj.Properties().GetParent()){
                return;
            }
            SceneHierarchyView::SetupObject(obj);
            
        });

    }
    

    ImGui::End();
}

void GuiLayer::SceneHierarchyView::SetupObject(Object obj) {
    ImGui::SetNextItemOpen(true);
    if(ImGui::TreeNodeEx((obj.Properties().GetName() + GuiLayer::GetImGuiID((void*)&obj.ID())).c_str(),ImGuiTreeNodeFlags_SpanFullWidth)){
        
        if(ImGui::BeginDragDropSource()){

            ImGui::SetDragDropPayload("AddChildren",&obj.ID(),sizeof(entt::entity),ImGuiCond_Once);
            ImGui::Text(obj.Properties().GetName().c_str());
            ImGui::EndDragDropSource();
        }
        if(ImGui::BeginDragDropTarget()){

            
            const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AddChildren");
            if(payload){
                Object maybeChildren(*(entt::entity*)payload->Data);
                if(maybeChildren.Properties().IsInChildren(obj)){
                    maybeChildren.Properties().ClearParent();
                }
                else{
                    maybeChildren.Properties().SetParent(obj);
                }
            }
            ImGui::EndDragDropTarget();
        }

        if(ImGui::BeginPopupContextItem(GuiLayer::GetImGuiID(&obj.Transform()).c_str())){
            
            if(ImGui::MenuItem("Duplicate","Ctrl+E")){
                Registry::CopyEntity(obj);
            }
            ImGui::EndPopup();
        }
        if(ImGui::IsItemClicked(ImGuiMouseButton_Left)){
            if(GameView::AnyObjectSelected()){
                Object(GameView::AnyObjectSelected().objectID).Properties().SetHightlightState(false);
            }
            GameView::AnyObjectSelected() = ClickedObjectProperties(obj.ID());
            Object(GameView::AnyObjectSelected().objectID).Properties().SetHightlightState(true);
        }

        for(auto& id : obj.Properties().GetChildren()){
            SetupObject(Object(id));
        }

        ImGui::TreePop();
    }
}
