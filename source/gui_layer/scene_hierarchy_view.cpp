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

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(0,0));
    ImGui::InvisibleButton("InvisibleButton",ImVec2(ImGui::GetWindowSize().x - ImGui::GetCursorPosX() - ImGui::GetWindowSize().x/15.0f,ImGui::GetWindowSize().y - ImGui::GetCursorPosY() - ImGui::GetWindowSize().y/30.0f));
    ImGui::PopStyleVar();

    if(ImGui::BeginDragDropTarget()){

        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AddChildren",ImGuiDragDropFlags_AcceptNoDrawDefaultRect);

        if(payload){
            Object unParentTarget(*(entt::entity*)payload->Data);
            unParentTarget.Properties().ClearParent();
        }

        ImGui::EndDragDropTarget();
    }

    if(ImGui::BeginPopupContextItem("AddingObjectsAndPropertiesPopup", ImGuiPopupFlags_MouseButtonRight)){

        if(ImGui::BeginMenu("Create Object")){
            for(auto& [name,func] : m_DefaultObjects){
                if(ImGui::MenuItem(name.c_str())){
                    func();
                }
            }
            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }
    
    

    ImGui::End();
}

void GuiLayer::SceneHierarchyView::SetupObject(Object obj) {
    
    SceneHierarchyViewComponent& comp = obj.GetComponent<SceneHierarchyViewComponent>();
    bool& openRename = comp.m_IsChoosingName;
    static std::string nameToRename;
    if(openRename){
        ImGui::SetNextItemOpen(true);
        if(ImGui::TreeNodeEx((GuiLayer::GetImGuiID((void*)&obj.ID())).c_str())){
            ImGui::SameLine();

            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,10);
            if(ImGui::InputText("##",&nameToRename,ImGuiInputTextFlags_EnterReturnsTrue)){
                obj.Properties().SetName(nameToRename);
                openRename = false;
            }
            ImGui::PopStyleVar();
            if(ImGui::IsKeyPressed(ImGuiKey_Escape,false)){
                openRename = false;
            }
            ImGui::TreePop();
        }
    }
    else{
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
                    if(!maybeChildren.Properties().IsInChildren(obj)){
                        maybeChildren.Properties().ClearParent();
                        maybeChildren.Properties().SetParent(obj);
                    }
                }
                ImGui::EndDragDropTarget();
            }

            if(ImGui::BeginPopupContextItem(GuiLayer::GetImGuiID(&obj.Transform()).c_str())){
                
                if(ImGui::MenuItem("Duplicate")){
                    Registry::CopyEntity(obj);
                }
                if(ImGui::MenuItem("Rename")){
                    nameToRename = obj.Properties().GetName();
                    openRename = true;
                }

                if(ImGui::MenuItem("Delete")){
                    Registry::DeleteObject(obj);
                }

                if(obj.Properties().GetParent()){
                    if(ImGui::MenuItem("Make Independent")){
                        obj.Properties().ClearParent();
                    }
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
    

    
    
}

void GuiLayer::SceneHierarchyView::SetupDefaultObjects() {
    m_DefaultObjects["Cube"] = [](){
        MeshAttribute::Vertex vertices;

        vertices.positions = {
        // front
        -0.5, -0.5,  0.5,
        0.5, -0.5,  0.5,
        0.5,  0.5,  0.5,
        -0.5,  0.5,  0.5,
        // back
        -0.5, -0.5, -0.5,
        0.5, -0.5, -0.5,
        0.5,  0.5, -0.5,
        -0.5,  0.5, -0.5
        };

        vertices.indices = {
            // front
            0, 1, 2,
            2, 3, 0,
            // right
            1, 5, 6,
            6, 2, 1,
            // back
            7, 6, 5,
            5, 4, 7,
            // left
            4, 0, 3,
            3, 7, 4,
            // bottom
            4, 5, 1,
            1, 0, 4,
            // top
            3, 2, 6,
            6, 7, 3
        };

        Object obj = Registry::CreateObject("Cube");
        Mesh& mesh = obj.AddComponent<Mesh>();

        mesh.SetVertices(vertices);
        mesh.SetShader("default_shaders/base_shader");
        
    };

    m_DefaultObjects["Plane"] = [](){
        MeshAttribute::Vertex vertices;

        vertices.positions = {
            -0.5f,-0.5f,0.0f,
            0.5f,-0.5f,0.0f,
            0.5f,0.5f,0.0f,
            -0.5f,0.5f,0.0f
        };

        vertices.indices = {
            0,1,2,
            2,3,0
        };

        Object obj = Registry::CreateObject("Plane");
        Mesh& mesh = obj.AddComponent<Mesh>();

        mesh.SetVertices(vertices);
        mesh.SetShader("default_shaders/base_shader");
    };

    m_DefaultObjects["Camera"] = [](){
        Object obj = Registry::CreateObject("Camera");
        obj.AddComponent<Camera>();
    };


}
