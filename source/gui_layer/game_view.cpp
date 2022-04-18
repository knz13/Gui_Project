#include "game_view.h"
#include "../kv.h"

std::shared_ptr<Framebuffer> GuiLayer::GameView::m_RaycastTexture;
std::unique_ptr<Framebuffer> GuiLayer::GameView::m_Buffer;
bool GuiLayer::GameView::m_CanSelect = true;
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
    
    static bool initialized = false;
    
    static ImVec2 lastSize;

    GuiLayer::SetupWindowStyle([&](ImGuiWindowFlags flags){
            ImGui::Begin("Game View",0,flags);
        });

        
        ImGui::BeginChild("GameRender");

        float windowPositionX = ImGui::GetWindowPos().x - ImGui::GetMainViewport()->Pos.x;
        float windowPositionY = ImGui::GetWindowPos().y - ImGui::GetMainViewport()->Pos.y;

        if(!initialized){
            m_RaycastTexture = std::make_shared<Framebuffer>(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
            m_Buffer = std::make_unique<Framebuffer>(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
            int newW = ImGui::GetWindowSize().y * 4/3.0f;
            int left = (ImGui::GetWindowSize().x - newW) / 2.0f;
            win.SetViewPort(0,0, ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
            initialized = true;
        }
        else{
            if(lastSize.x != m_Buffer.get()->GetSize().x || lastSize.y != m_Buffer.get()->GetSize().y){
                m_RaycastTexture = std::make_shared<Framebuffer>(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
                m_Buffer = std::make_unique<Framebuffer>(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
                int newW = ImGui::GetWindowSize().y * 4/3.0f;
                int left = (ImGui::GetWindowSize().x - newW) / 2.0f;
                win.SetViewPort(0,0, ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
            }
        }
        m_Buffer.get()->Bind();
        lastSize = ImGui::GetWindowSize();
        // Get the size of the child (i.e. the whole draw size of the windows).
        ImVec2 wsize = ImGui::GetWindowSize();
        

        ImGui::Image((ImTextureID)m_Buffer.get()->GetAttachedTexture().GetID(),wsize, ImVec2(0, 1), ImVec2(1, 0));
        

        HandleSelectionGuizmo(win);
        
        HandleEditorCameraMovement(win);

        HandleObjectSelection(win);

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

void GuiLayer::GameView::HandleEditorCameraMovement(Window& win)
{
    static bool wasMouseDown = false;
    static bool wasMouseWheelDown = false;
    
    
        
    if ((ImGui::IsMouseDown(ImGuiMouseButton_Right)) || wasMouseDown) {
        static ImVec2 lastMousePos;
        m_CanSelect = false;

        if (!wasMouseDown) {
            lastMousePos = ImGui::GetMousePos();
            wasMouseDown = true;
        }
        else {
            glm::vec3 offset(lastMousePos.x - ImGui::GetMousePos().x, lastMousePos.y - ImGui::GetMousePos().y, 0);
            offset *= 0.2;
            Window::GetCurrentWindow().GetCurrentCamera().GetAsObject().GetComponent<TransformComponent>().Rotate(offset.y, offset.x, 0);
            lastMousePos = ImGui::GetMousePos();
        }
    }

    if ((ImGui::IsMouseDown(ImGuiMouseButton_Middle)) || wasMouseWheelDown) {
        static ImVec2 lastMouseWheelPos;
        m_CanSelect = false;

        if (!wasMouseWheelDown) {
            lastMouseWheelPos = ImGui::GetMousePos();
            wasMouseWheelDown = true;
        }
        else {
                
          
            glm::vec3 offset(lastMouseWheelPos.x - ImGui::GetMousePos().x, ImGui::GetMousePos().y - lastMouseWheelPos.y, 0);
            offset *= 0.03;
            if (Window::GetCurrentWindow().GetCurrentCamera()) {
                Window::GetCurrentWindow().GetCurrentCamera().GetAsObject().GetComponent<Camera>().MoveInRelationToView(offset.x, offset.y, 0);
            };
            lastMouseWheelPos = ImGui::GetMousePos();
        }
    }




    
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && wasMouseDown) {
        wasMouseDown = false;
    }
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle) && wasMouseWheelDown) {
        wasMouseWheelDown = false;
    }

    static ImVec2 gameViewRectMin;
    static ImVec2 gameViewRectMax;
    static bool hasSetupWheelCallback = false;

    m_ContentRectMin = ImGui::GetWindowPos();
    m_ContentRectMax = ImVec2(ImGui::GetWindowSize().x + m_ContentRectMin.x, ImGui::GetWindowSize().y + m_ContentRectMin.y);

    if (!hasSetupWheelCallback) {
        win.Events().MouseScrollEvent().Connect([&](Window& window, MouseScrollEventProperties mouseWheelEventProperties) {
            if (Math::IsPointInRect(m_ContentRectMin, m_ContentRectMax, ImGui::GetMousePos())) {
                if (window.GetCurrentCamera()) {
                    window.GetCurrentCamera().GetAsObject().GetComponent<Camera>().MoveInRelationToView(0, 0, -mouseWheelEventProperties.offset.y * window.GetDeltaTime() * 400);
                }
            }
        });
        hasSetupWheelCallback = true; //making sure to only do it once
    }

}

void GuiLayer::GameView::HandleSelectionGuizmo(Window& win)
{
    static int imguizmoMode = ImGuizmo::OPERATION::TRANSLATE;
    if (m_IsObjectSelected && win.GetCurrentCamera()) {

        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

        Object clickedObject(m_IsObjectSelected.objectID);
        glm::mat4 proj = win.GetCurrentCamera().GetAsObject().GetComponent<Camera>().GetProjection(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
        glm::mat4 view = win.GetCurrentCamera().GetAsObject().GetComponent<Camera>().GetView();
        TransformComponent& objectTransform = clickedObject.GetComponent<TransformComponent>();

        if (objectTransform.GetScale().x == 0) {
            objectTransform.SetScale(0.1, 0, 0);
        }
        if (objectTransform.GetScale().y == 0) {
            objectTransform.SetScale(0, 0.1, 0);
        }
        if (objectTransform.GetScale().z == 0) {
            objectTransform.SetScale(0, 0, 0.1);
        }

        glm::mat4 model = objectTransform.GetModelMatrix();

        float snap[5] = { 0,0,0,0,0.1 };
        ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj), (ImGuizmo::OPERATION)imguizmoMode, ImGuizmo::MODE::LOCAL, glm::value_ptr(model), 0, snap);




        if (ImGuizmo::IsUsing()) {
            m_CanSelect = false;
            objectTransform.SetFromModelMatrix(model);

        }



    }
    ImVec2 gameSize = ImGui::GetWindowSize();

    win.Events().KeyEvent().Connect([&](Window& window, KeyEventProperties keyEvent) {

        if (keyEvent.action == GLFW_PRESS && Math::IsPointInRect(m_ContentRectMin, m_ContentRectMax, ImGui::GetMousePos())) {
            if (keyEvent.key == GLFW_KEY_E) {
                imguizmoMode = ImGuizmo::OPERATION::TRANSLATE;
            }
            if (keyEvent.key == GLFW_KEY_R) {
                imguizmoMode = ImGuizmo::OPERATION::ROTATE;
            }
            if (keyEvent.key == GLFW_KEY_T) {
                imguizmoMode = ImGuizmo::OPERATION::SCALE;
            }
        }
    });



}

void GuiLayer::GameView::HandleObjectSelection(Window& win)
{
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered() && m_CanSelect) {
        ImVec2 pos;
        pos.x = ImGui::GetMousePos().x - ImGui::GetWindowPos().x;
        pos.y = ImGui::GetWindowSize().y - (ImGui::GetMousePos().y - ImGui::GetWindowPos().y);
        RayCastHit hit = RayCast(pos);
        if (hit) {
            if (m_IsObjectSelected) {
                Object(m_IsObjectSelected.objectID).Properties().SetHightlightState(false);
            }
            m_IsObjectSelected = ClickedObjectProperties(hit.hitObjectID);
            Object(m_IsObjectSelected.objectID).Properties().SetHightlightState(true);
        }

    }
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && ImGui::IsWindowHovered()) {
        if (m_IsObjectSelected) {
            Object(m_IsObjectSelected.objectID).Properties().SetHightlightState(false);
        }
        m_IsObjectSelected = ClickedObjectProperties();
    }

    m_CanSelect = true;
}



