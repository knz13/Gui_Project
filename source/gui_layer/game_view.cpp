#include "game_view.h"
#include "../kv.h"

std::shared_ptr<Framebuffer> GuiLayer::GameView::m_RaycastTexture;
std::unique_ptr<Framebuffer> GuiLayer::GameView::m_Buffer;
RayCastHit GuiLayer::GameView::RayCast(ImVec2 pos) {
    m_RaycastTexture.get()->Bind();
    unsigned char data[4];
    GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    GL_CALL(glReadPixels(pos.x,pos.y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,data));
    
    uint32_t pickedID = data[0] + data[1]*256 + data[2]*256*256;
    pickedID--;
    
    uint32_t max = 0;
    max--;
    if(pickedID == max){
        return RayCastHit((entt::entity)max);
    }

    m_RaycastTexture.get()->Unbind();
    if(Registry::Get().valid((entt::entity)(pickedID))){
        return RayCastHit((entt::entity)pickedID);
    }
    else{
        return RayCastHit((entt::entity)max);
    }
}

void GuiLayer::GameView::Update(Window& win) {
    static int imguizmoMode = ImGuizmo::OPERATION::TRANSLATE;
    static bool initialized = false;
    static ImVec2 lastSize;

    GuiLayer::SetupWindowStyle([&](ImGuiWindowFlags flags){
            ImGui::Begin("Game View",0,flags);
        });

        ImGui::BeginChild("GameRender");


        if(!initialized){
            m_RaycastTexture = std::make_shared<Framebuffer>(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
            m_Buffer = std::make_unique<Framebuffer>(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
            GL_CALL(glViewport(0,0,ImGui::GetWindowSize().x,ImGui::GetWindowSize().y));
            initialized = true;
        }
        else{
            if(lastSize.x != m_Buffer.get()->GetSize().x || lastSize.y != m_Buffer.get()->GetSize().y){
                m_RaycastTexture = std::make_shared<Framebuffer>(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
                m_Buffer = std::make_unique<Framebuffer>(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
                GL_CALL(glViewport(0,0,ImGui::GetWindowSize().x,ImGui::GetWindowSize().y));
            }
        }
        m_Buffer.get()->Bind();
        lastSize = ImGui::GetWindowSize();
        // Get the size of the child (i.e. the whole draw size of the windows).
        ImVec2 wsize = ImGui::GetWindowSize();

        if(ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()){
            ImVec2 pos;
            pos.x = ImGui::GetMousePos().x - ImGui::GetWindowPos().x;
            pos.y = wsize.y - (ImGui::GetMousePos().y - ImGui::GetWindowPos().y);
            RayCastHit hit = RayCast(pos);
            if(hit){
                if(m_IsObjectSelected){
                    Object(m_IsObjectSelected.objectID).Properties().SetHightlightState(false);
                }
                m_IsObjectSelected = ClickedObjectProperties(hit.hitObjectID);
                Object(m_IsObjectSelected.objectID).Properties().SetHightlightState(true);
            }
           
        }
        if(ImGui::IsMouseDown(ImGuiMouseButton_Right) && ImGui::IsWindowHovered()){
            if(m_IsObjectSelected){
                Object(m_IsObjectSelected.objectID).Properties().SetHightlightState(false);
            }
            m_IsObjectSelected = ClickedObjectProperties();
        }
        

        ImGui::Image((ImTextureID)m_Buffer.get()->GetAttachedTexture().GetID(),wsize, ImVec2(0, 1), ImVec2(1, 0));
        if(m_IsObjectSelected && win.GetCurrentCamera()){

            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y,ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);

            Object clickedObject(m_IsObjectSelected.objectID);
            glm::mat4 proj = win.GetCurrentCamera().GetAsObject().GetComponent<Camera>().GetProjection(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
            glm::mat4 view = win.GetCurrentCamera().GetAsObject().GetComponent<Camera>().GetView();
            TransformComponent& objectTransform = clickedObject.GetComponent<TransformComponent>();
            
            if(objectTransform.GetScale().x == 0){
                objectTransform.SetScale(0.1,0,0);
            }
            if(objectTransform.GetScale().y == 0){
                objectTransform.SetScale(0,0.1,0);
            }
            if(objectTransform.GetScale().z == 0){
                objectTransform.SetScale(0,0,0.1);
            }

            glm::mat4 model = objectTransform.GetModelMatrix();

            float snap[5] = {0,0,0,0,0.1};
            ImGuizmo::Manipulate(glm::value_ptr(view),glm::value_ptr(proj),(ImGuizmo::OPERATION)imguizmoMode,ImGuizmo::MODE::LOCAL,glm::value_ptr(model),0,snap);


            objectTransform.SetFromModelMatrix(model);
            /*
            if(ImGuizmo::IsUsing()){
                glm::vec3 position,scale;
                glm::quat rotation;
                glm::vec3 skew;
                glm::vec4 perspective;
                glm::decompose(model,scale,rotation,position,skew,perspective);

                objectTransform.SetPosition(position);
                objectTransform.SetRotation(glm::eulerAngles(rotation));
                objectTransform.SetScale(scale);

            }
            */
            

        }
        ImVec2 gameSize = ImGui::GetWindowSize();

        if(ImGui::IsKeyPressed(ImGuiKey_E)){
            imguizmoMode = ImGuizmo::OPERATION::TRANSLATE;
        }
        if(ImGui::IsKeyPressed(ImGuiKey_R)){
            imguizmoMode = ImGuizmo::OPERATION::ROTATE;
        }
        if(ImGui::IsKeyPressed(ImGuiKey_T)){
            imguizmoMode = ImGuizmo::OPERATION::SCALE;
        }

        static bool wasMouseDown = false;
        if((ImGui::IsKeyDown(ImGuiKey_LeftShift) && ImGui::IsMouseDown(ImGuiMouseButton_Left)) || wasMouseDown){
            static ImVec2 lastMousePos;


            if(!wasMouseDown){
                lastMousePos = ImGui::GetMousePos();
                wasMouseDown = true;
            }
            else{
                glm::vec3 offset(lastMousePos.x - ImGui::GetMousePos().x,ImGui::GetMousePos().y - lastMousePos.y,0);
                offset *= 0.03;
                Window::GetCurrentWindow().GetCurrentCamera().GetAsObject().GetComponent<Camera>().MoveInRelationToView(offset.x,offset.y,offset.z);
                lastMousePos = ImGui::GetMousePos();
            }
        }
        if(ImGui::IsMouseReleased(ImGuiMouseButton_Left) && wasMouseDown){
            wasMouseDown = false;
        }

        
        ImGui::EndChild();



        ImGui::End();


        m_RaycastTexture.get()->Bind();

        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        m_RaycastTexture.get()->Unbind();

        if(m_Buffer.get()->Status()){

            m_Buffer.get()->Bind();
        }


        m_RaycastTexture.get()->GetAttachedTexture().Bind();
        GL_CALL(glBindImageTexture(3,m_RaycastTexture.get()->GetAttachedTexture().GetID(),0,GL_FALSE,0,GL_WRITE_ONLY,GL_RGBA32F));

}

void GuiLayer::GameView::Setup(Window& win) {

    win.Events().PostDrawingLoopEvent().Connect([&](Window& window){
        GL_CALL(glMemoryBarrier(GL_ALL_BARRIER_BITS));
        m_Buffer.get()->Unbind();
    });

    win.Events().ClosingEvent().Connect([&](Window& win){
        m_RaycastTexture.reset();
        m_Buffer.reset();
    });
}

ClickedObjectProperties& GuiLayer::GameView::AnyObjectSelected() {
    return m_IsObjectSelected;
}
