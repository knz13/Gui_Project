#include "scene_hierarchy_view.h"
#include "../kv.h"

void GuiLayer::SceneHierarchyView::Update(Window& win) {
    
    GuiLayer::SetupWindowStyle("Scene Hierarchy",[&](ImGuiWindowFlags flags){
        ImGui::Begin("Scene Hierarchy",0,flags );
    });

    
#ifndef NDEBUG
    ImGui::ShowDemoWindow();

#endif 
   

   
    if(ecspp::Registry().alive() > 0){
        
        GameObject::ForEach([&](GameObject obj){
            if(obj.GetParent()){
                return;
            }
            
#ifdef NDEBUG
            if (obj.HasComponent<InternalUse>()) {
                return;
            }
            SceneHierarchyView::SetupObject(obj);
#else 
            if (obj.HasComponent<InternalUse>()) {
                ImGui::PushStyleColor(ImGuiCol_Header,Color::Red.AsImVec4());
                ImGui::PushStyleColor(ImGuiCol_HeaderActive,Color::Red.AsImVec4());
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered,Color::Red.AsImVec4());
            }

            SceneHierarchyView::SetupObject(obj);

            if (obj.HasComponent<InternalUse>()) {
                ImGui::PopStyleColor(3);
            }
#endif

            
        });

    }

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(0,0));
    ImGui::InvisibleButton("InvisibleButton",ImVec2(ImGui::GetWindowSize().x - ImGui::GetCursorPosX() - ImGui::GetWindowSize().x/15.0f,ImGui::GetWindowSize().y - ImGui::GetCursorPosY() - ImGui::GetWindowSize().y/30.0f));
    ImGui::PopStyleVar();

    if(ImGui::BeginDragDropTarget()){

        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AddChildren",ImGuiDragDropFlags_AcceptNoDrawDefaultRect);

        if(payload){
            ecspp::Object unParentTarget(*(entt::entity*)payload->Data);
            unParentTarget.ClearParent();
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

void GuiLayer::SceneHierarchyView::SetupObject(GameObject obj) {
    
    SceneHierarchyViewComponent& comp = obj.GetComponent<SceneHierarchyViewComponent>();
    bool& openRename = comp.m_IsChoosingName;
    static std::string nameToRename;
    if(openRename){
        ImGui::SetNextItemOpen(true);
        if(ImGui::TreeNodeEx((GuiLayer::GetImGuiID((void*)&obj.ID())).c_str())){
            ImGui::SameLine();

            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,10);
            ImGui::SetKeyboardFocusHere();
            if(ImGui::InputText("##",&nameToRename,ImGuiInputTextFlags_EnterReturnsTrue)){
                obj.SetName(nameToRename);
                openRename = false;
            }
            ImGui::PopStyleVar();
            if(ImGui::IsKeyPressed(ImGuiKey_Escape,false)){
                openRename = false;
            }
            ImGui::TreePop();
        }
    }
    else {

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Framed;
        if (GuiLayer::AnyObjectSelected()) {
            if (GuiLayer::AnyObjectSelected().ID() == obj.ID()) {
                flags |= ImGuiTreeNodeFlags_Selected;
            }
        }

        ImGui::SetNextItemOpen(true,ImGuiCond_Once);
        bool isOpen = false;

        
        isOpen = ImGui::TreeNodeEx((obj.GetName() + GuiLayer::GetImGuiID((void*)&obj.ID())).c_str(), flags);


        if (isOpen){
            
            if(ImGui::BeginDragDropSource()){

                ImGui::SetDragDropPayload("AddChildren",&obj.ID(),sizeof(entt::entity),ImGuiCond_Once);
                ImGui::Text(obj.GetName().c_str());
                ImGui::EndDragDropSource();
            }
            if(ImGui::BeginDragDropTarget()){

                
                const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AddChildren");
                if(payload){
                    ecspp::Object maybeChildren(*(entt::entity*)payload->Data);
                    if(!maybeChildren.IsInChildren(obj)){
                        maybeChildren.ClearParent();
                        maybeChildren.SetParent(obj);
                    }
                }
                ImGui::EndDragDropTarget();
            }

            if(ImGui::BeginPopupContextItem(GuiLayer::GetImGuiID(&obj.Transform()).c_str())){
                
                if(ImGui::MenuItem("Duplicate")){
                    ecspp::CopyObject(obj);
                }
                if(ImGui::MenuItem("Rename")){
                    nameToRename = obj.GetName();
                    openRename = true;
                }

                if(ImGui::MenuItem("Delete")){
                    ecspp::DeleteObject(obj);
                }

                if(obj.GetParent()){
                    if(ImGui::MenuItem("Make Independent")){
                        obj.ClearParent();
                    }
                }

                ImGui::EndPopup();
            }
            
            
                    


            if(ImGui::IsItemClicked(ImGuiMouseButton_Left)){
                if(GuiLayer::AnyObjectSelected()){
                    if (GuiLayer::AnyObjectSelected().IsType(HelperFunctions::HashClassName<GameObject>())) {
                        GuiLayer::AnyObjectSelected().GetAs<GameObject>().SetHighlightState(false);
                    }
                }
                GuiLayer::AnyObjectSelected() = ecspp::ObjectHandle(obj.ID());
                GuiLayer::AnyObjectSelected().GetAs<GameObject>().SetHighlightState(true);
            }

            for(auto id : obj.GetChildren()){
                SetupObject(id.GetAs<GameObject>());
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

        GameObject obj = GameObject::CreateNew("Cube");
        Mesh& mesh = obj.AddComponent<Mesh>();

        mesh.SetVertices(vertices);
        mesh.SetShader("defaults/default_shaders/base_shader");
        
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

        GameObject obj = GameObject::CreateNew("Plane");
        Mesh& mesh = obj.AddComponent<Mesh>();

        mesh.SetVertices(vertices);
        mesh.SetShader("defaults/default_shaders/base_shader");
    };

    m_DefaultObjects["Camera"] = [](){
        GameObject obj = GameObject::CreateNew("Camera");
        obj.AddComponent<Camera>();
    };


}

void GuiLayer::SceneHierarchyViewComponent::Init()
{
    this->HideInEditor(true);
}
