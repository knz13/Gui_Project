#include "game_view.h"
#include "../kv.h"

std::shared_ptr<Framebuffer> GuiLayer::GameView::m_RaycastTexture;
ObjectHandle GuiLayer::GameView::m_EditorCamera;
bool GuiLayer::GameView::m_CanSelect = true;
RayCastHit GuiLayer::GameView::RayCast(ImVec2 pos) {
    uint32_t max = 0;
    max--;
    if (!m_RaycastTexture.get()->Status()) {
        return RayCastHit((entt::entity)max);
    }
    m_RaycastTexture.get()->Bind();
    
    
    //m_RaycastTexture.get()->Clear(Color::White);

    unsigned char data[4];
    GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    GL_CALL(glReadPixels(pos.x,pos.y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,data));
    
    //data[0] *= 255;
    //data[1] *= 255;
    //data[2] *= 255;

    uint32_t pickedID = data[0] + data[1] * 256 + data[2] * 256 * 256;
    pickedID--;
    
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

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    GuiLayer::SetupWindowStyle("Game View",[&](ImGuiWindowFlags flags){
            ImGui::Begin("Game View",0,flags);
    });
    
    ImGui::PopStyleVar();

    ImGui::BeginChild("GameRender");
    float windowPositionX = ImGui::GetWindowPos().x;
    float windowPositionY = ImGui::GetWindowPos().y;
    float windowSizeX = ImGui::GetWindowSize().x;
    float windowSizeY = ImGui::GetWindowSize().y;
        
    if(!initialized){
            
        m_RaycastTexture = std::make_shared<Framebuffer>(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
        m_EditorCamera.GetAsObject().GetComponent<Camera>().SetRenderTarget(std::make_shared<Framebuffer>(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y));
        m_EditorCamera.GetAsObject().GetComponent<Camera>().SetViewport(0,0, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
        initialized = true;
    }
    else{
        glm::vec4 currentSize = m_EditorCamera.GetAsObject().GetComponent<Camera>().GetViewPort();
        if(lastSize.x != currentSize.z || lastSize.y != currentSize.w) {
            m_RaycastTexture = std::make_shared<Framebuffer>(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
            m_EditorCamera.GetAsObject().GetComponent<Camera>().SetRenderTarget(std::make_shared<Framebuffer>(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y));
            m_EditorCamera.GetAsObject().GetComponent<Camera>().SetViewport(0, 0, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
        }
    }
        

        
        



    lastSize = ImGui::GetWindowSize();
    // Get the size of the child (i.e. the whole draw size of the windows).
    ImVec2 wsize = ImGui::GetWindowSize();
        
    //m_EditorCamera.GetAsObject().GetComponent<Camera>().GetRenderTarget()
    ImGui::Image((ImTextureID)m_EditorCamera.GetAsObject().GetComponent<Camera>().GetRenderTarget().GetAttachedTexture().GetID(), wsize, ImVec2(0, 1), ImVec2(1, 0));
        

    HandleSelectionGuizmo(win);
        
    HandleEditorCameraMovement(win);


    HandleObjectSelection(win);


    ImGui::EndChild();

    ImGui::End();


        
    m_RaycastTexture.get()->Clear();

       

}

void GuiLayer::GameView::Setup(Window& win) {
    GameObject obj = ObjectPropertyRegister::CreateNew<GameObject>("Editor Camera");
    obj.AddComponent<Camera>();
    
    obj.AddComponent<InternalUse>();
    obj.Transform().SetPosition(0,5,20);
    obj.GetComponent<Camera>().SetLookAt(0, 0, 0);


    m_EditorCamera = ObjectHandle(obj.ID());

    SetupEditorCameraDrawing();


    win.Events().ClosingEvent().Connect([&](Window& win){
        m_RaycastTexture.reset();
    });
}

ClickedObjectProperties& GuiLayer::GameView::AnyObjectSelected() {
    return m_IsObjectSelected;
}

void GuiLayer::GameView::SetupEditorCameraDrawing()
{

    m_EditorCamera.GetAsObject().GetComponent<Camera>().SetDrawingFunction([=](Camera& camera) {
        

        m_RaycastTexture.get()->GetAttachedTexture().Bind();
        GL_CALL(glBindImageTexture(3, m_RaycastTexture.get()->GetAttachedTexture().GetID(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F));

        auto view = Registry::Get().view<TransformComponent, Mesh>();
        for (auto entity : view) {


            auto& transform = view.get<TransformComponent>(entity);
            auto& drawable = view.get<Mesh>(entity);

            if (!drawable.ReadyToDraw()) {
                continue;
            }

            Shader& currentObjectShader = drawable.GetShader();

            glm::mat4 mvp = camera.GetProjection() * camera.GetView() * transform.GetModelMatrix();
            currentObjectShader.Bind();
            currentObjectShader.SetUniformMat4f("MVP", mvp);

            currentObjectShader.SetUniform1i("rayCastTexture", 3);

            uint32_t val = (uint32_t)entity;
            val++;

            float r = ((uint32_t)val & 0x000000FF) >> 0;
            float g = ((uint32_t)val & 0x0000FF00) >> 8;
            float b = ((uint32_t)val & 0x00FF0000) >> 16;

            currentObjectShader.SetUniform3f("UMyIdentifier", r/255.0f, g / 255.0f, b / 255.0f);

            


            if (GameObject(entity).GetHighlightState()) {

                GL_CALL(glStencilFunc(GL_ALWAYS, 1, 0xFF));
                GL_CALL(glStencilMask(0xFF));


                drawable.Draw(mvp);

                GL_CALL(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
                GL_CALL(glStencilMask(0x00));
                GL_CALL(glDisable(GL_DEPTH_TEST));

                transform.InstantScaleChange(0.1, 0.1, 0.1);
                bool result;
                std::string currentShaderName = drawable.GetShaderName();
                Shader& singleColorShader = Window::GetCurrentWindow().Create().CachedShader("default_Shaders/single_color_shader", &result);

                GameObject comp(entity);

                glm::mat4 mvpSecond = camera.GetProjection() * camera.GetView() * transform.GetModelMatrix();

                singleColorShader.Bind();
                singleColorShader.SetUniformMat4f("MVP", mvpSecond);
                singleColorShader.SetUniform3f("desiredColor", comp.GetHighlightColor().Normalized().x, comp.GetHighlightColor().Normalized().y, comp.GetHighlightColor().Normalized().z);

                drawable.Draw(mvpSecond);

                drawable.SetShader(currentShaderName);

                transform.InstantScaleChange(-0.1, -0.1, -0.1);

                GL_CALL(glStencilFunc(GL_ALWAYS, 1, 0xFF));
                GL_CALL(glStencilMask(0x00));
                GL_CALL(glEnable(GL_DEPTH_TEST));

            }
            else {
                drawable.Draw(mvp);
            }

            

        }
        GL_CALL(glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT));

        

    });
    
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
            m_EditorCamera.GetAsObject().GetComponent<TransformComponent>().Rotate(offset.y, offset.x, 0);
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
            
            m_EditorCamera.GetAsObject().GetComponent<Camera>().MoveInRelationToView(offset.x, offset.y, 0);
            
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
                
                m_EditorCamera.GetAsObject().GetComponent<Camera>().MoveInRelationToView(0, 0, -mouseWheelEventProperties.offset.y * window.GetDeltaTime() * 400);
                
            }
        });
        hasSetupWheelCallback = true; //making sure to only do it once
    }

}

void GuiLayer::GameView::HandleSelectionGuizmo(Window& win)
{
    static bool first = true;
    static int imguizmoMode = ImGuizmo::OPERATION::TRANSLATE;
    if (m_IsObjectSelected && m_EditorCamera) {

        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

        GameObject clickedObject(m_IsObjectSelected.objectID);
        glm::mat4 proj = m_EditorCamera.GetAsObject().GetComponent<Camera>().GetProjection();
        glm::mat4 view = m_EditorCamera.GetAsObject().GetComponent<Camera>().GetView();
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

    if(first){
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
        first = false;
    }



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
                GameObject(m_IsObjectSelected.objectID).SetHighlightState(false);
            }
            m_IsObjectSelected = ClickedObjectProperties(hit.hitObjectID);
            GameObject(m_IsObjectSelected.objectID).SetHighlightState(true);
        }

    }
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && ImGui::IsWindowHovered()) {
        if (m_IsObjectSelected) {
            GameObject(m_IsObjectSelected.objectID).SetHighlightState(false);
        }
        m_IsObjectSelected = ClickedObjectProperties();
    }

    m_CanSelect = true;
}



