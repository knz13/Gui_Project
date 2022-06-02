#include "scene_hierarchy_view.h"
#include "../kv.h"

void GuiLayer::SceneHierarchyView::Update(Window& win) {
    
    GuiLayer::SetupWindowStyle("Scene Hierarchy",[&](ImGuiWindowFlags flags){
        ImGui::Begin("Scene Hierarchy",0,flags );
    });

    
    
   

   
    if(ecspp::Registry::Get().alive() > 0){
        
        GameObject::ForEach([&](GameObject obj){
            /*
            if (obj.HasComponent<InternalUse>()) {
                return;
            }
            */
            if(obj.GetParent()){
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
            ecspp::Object unParentTarget(*(entt::entity*)payload->Data);
            unParentTarget.Properties().ClearParent();
        }

        ImGui::EndDragDropTarget();
    }

    if(ImGui::BeginPopupContextItem("AddingObjectsAndPropertiesPopup", ImGuiPopupFlags_MouseButtonRight)){

        if(ImGui::BeginMenu("Create ecspp::Object")){
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
    else {

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnArrow;
        if (GuiLayer::AnyObjectSelected()) {
            if (GuiLayer::AnyObjectSelected().ID() == obj.ID()) {
                flags |= ImGuiTreeNodeFlags_Selected;
            }
        }

        ImGui::SetNextItemOpen(true,ImGuiCond_Once);
        bool isOpen = false;

        GuiLayer::SetupStaticTreeNodeStyle([&]() {
            isOpen = ImGui::TreeNodeEx((obj.Properties().GetName() + GuiLayer::GetImGuiID((void*)&obj.ID())).c_str(), flags);
        });


        if (isOpen){
            
            if(ImGui::BeginDragDropSource()){

                ImGui::SetDragDropPayload("AddChildren",&obj.ID(),sizeof(entt::entity),ImGuiCond_Once);
                ImGui::Text(obj.Properties().GetName().c_str());
                ImGui::EndDragDropSource();
            }
            if(ImGui::BeginDragDropTarget()){

                
                const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AddChildren");
                if(payload){
                    ecspp::Object maybeChildren(*(entt::entity*)payload->Data);
                    if(!maybeChildren.Properties().IsInChildren(obj)){
                        maybeChildren.Properties().ClearParent();
                        maybeChildren.Properties().SetParent(obj);
                    }
                }
                ImGui::EndDragDropTarget();
            }

            if(ImGui::BeginPopupContextItem(GuiLayer::GetImGuiID(&obj.Transform()).c_str())){
                
                if(ImGui::MenuItem("Duplicate")){
                    ObjectPropertyRegister::CopyObject(obj);
                }
                if(ImGui::MenuItem("Rename")){
                    nameToRename = obj.Properties().GetName();
                    openRename = true;
                }

                if(ImGui::MenuItem("Delete")){
                    ObjectPropertyRegister::DeleteObject(obj);
                }

                if(obj.Properties().GetParent()){
                    if(ImGui::MenuItem("Make Independent")){
                        obj.Properties().ClearParent();
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
