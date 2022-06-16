#include "camera.h"
#include "../kv.h"




void Camera::SetLookAt(float x, float y, float z) {
    
    glm::mat4 lookat = glm::lookAt(GetMasterObject().GetAsObject().GetComponent<TransformComponent>().GetPosition(),glm::vec3(x,y,z),glm::vec3(0,1,0));
    glm::mat4 modelMat = glm::inverse(lookat);
    modelMat[3] = glm::vec4(0,0,0,1);
    GetMasterObject().GetAsObject().GetComponent<TransformComponent>().SetRotation(glm::eulerAngles(glm::quat(modelMat)));
}

void Camera::SetDirection(float x, float y, float z) {
    GetMasterObject().GetAsObject().GetComponent<TransformComponent>().SetRotation(glm::eulerAngles(glm::quatLookAt(glm::vec3(x,y,z),glm::vec3(0,1,0))));
}

glm::mat4 Camera::GetViewProjection(const Window& window) {
    return glm::perspective(glm::radians(m_Fov),(float)window.Properties().width/window.Properties().height,m_DrawNear,m_DrawDistance) * glm::inverse(GetMasterObject().GetAsObject().GetComponent<TransformComponent>().GetModelMatrix());
}

glm::vec4 Camera::GetViewPort() const {
    return m_ViewPort;
}


void Camera::Update(float deltaTime) {
    
}


Camera::~Camera() {
    
}

void Camera::Init()
{
    m_ViewPort.z = Window::GetCurrentWindow().Properties().width;
    m_ViewPort.w = Window::GetCurrentWindow().Properties().height;
    this->SetDrawingFunction([](Camera& camera) {

        auto view = ecspp::Registry().view<TransformComponent, Mesh>();
        for (auto entity : view) {

            auto& transform = view.get<TransformComponent>(entity);
            auto& drawable = view.get<Mesh>(entity);

            if (!drawable.ReadyToDraw()) {
                continue;
            }

            Shader& currentObjectShader = drawable.GetShader();
            glm::mat4 mvp = camera.GetProjection() * camera.GetView() * transform.GetModelMatrix();;

            currentObjectShader.Bind();
            currentObjectShader.SetUniformMat4f("MVP", mvp);

            drawable.Draw(mvp);
        }

    });
}

void Camera::Destroy()
{
    m_RenderTarget.reset();
}

void Camera::LookAt(ecspp::Object& obj) {
    this->SetLookAt(obj.GetComponent<TransformComponent>().GetPosition().x,obj.GetComponent<TransformComponent>().GetPosition().y,obj.GetComponent<TransformComponent>().GetPosition().z);
}

glm::mat4 Camera::GetProjection() const{
    float aspect = (m_ViewPort.z - m_ViewPort.x) / (m_ViewPort.w - m_ViewPort.y);
    return glm::perspective(glm::radians(m_Fov),aspect,m_DrawNear,m_DrawDistance);
}

glm::mat4 Camera::GetView() const{
    return glm::inverse(GetMasterObject().GetAsObject().GetComponent<TransformComponent>().GetModelMatrix());
}


Camera::Camera(CameraCreationProperties prop) {
    m_Fov = prop.fov;
    m_DrawNear = prop.drawingNearCutoff;
    m_DrawDistance = prop.drawDistance;
    
    m_ViewPort = prop.viewPort;

}


Camera::Camera() {
    CameraCreationProperties prop;

    m_Fov = prop.fov;
    m_DrawNear = prop.drawingNearCutoff;
    m_DrawDistance = prop.drawDistance;
    m_ViewPort = prop.viewPort;
}




void Camera::MoveInRelationToView(float rightLeft, float upDown, float frontBack) {
    glm::vec3 right,up,look;
    glm::mat4 modelView = GetView();

    right = glm::vec3(modelView[0][0],modelView[1][0],modelView[2][0]);
    right = glm::normalize(right);
    up = glm::vec3(modelView[0][1],modelView[1][1],modelView[2][1]);
    up = glm::normalize(up);
    look  = glm::vec3(modelView[0][2],modelView[1][2],modelView[2][2]);
    look = glm::normalize(look);

    GetMasterObject().GetAsObject().GetComponent<TransformComponent>().Move(right * rightLeft);
    GetMasterObject().GetAsObject().GetComponent<TransformComponent>().Move(up * upDown);
    GetMasterObject().GetAsObject().GetComponent<TransformComponent>().Move(look * frontBack);

}

glm::vec3 Camera::GetLookDirection() const{
    glm::mat4 modelView = GetView() * GetMasterObject().GetAsObject().GetComponent<TransformComponent>().GetModelMatrix();
    glm::vec3 look  = glm::vec3(modelView[0][2],modelView[1][2],modelView[2][2]);
    look = glm::normalize(look);
    return look;
}

glm::vec2 Camera::GetViewportSize() const
{
    if (m_RenderTarget) {
        return glm::vec2(m_RenderTarget.get()->GetSize());
    }
    return glm::vec2(Window::GetCurrentWindow().Properties().width, Window::GetCurrentWindow().Properties().height);
}

Framebuffer& Camera::GetRenderTarget()
{
    //TODO: make safer...
    return *m_RenderTarget.get();
}

bool Camera::HasRenderTarget()
{
    return m_RenderTarget.operator bool();
}

void Camera::Render()
{
    if (GetMasterObject().operator bool()) {
        if (HasRenderTarget()) {
            m_RenderTarget.get()->Clear();
            m_RenderTarget.get()->Bind();
        }
        GL_CALL(glViewport(m_ViewPort.x, m_ViewPort.y, m_ViewPort.z, m_ViewPort.w));
        m_DrawingFunc(*this);
        if (HasRenderTarget()) {
            m_RenderTarget.get()->Unbind();
        }
    }
    
}

void Camera::SetDrawingFunction(std::function<void(Camera&)> drawingFunc)
{
    m_DrawingFunc = drawingFunc;
}



void Camera::SetRenderTarget(std::shared_ptr<Framebuffer> framebuffer)
{
    m_RenderTarget = framebuffer;
}

void Camera::SetViewport(float x, float y, float width, float height)
{
    m_ViewPort = glm::vec4(x,y,width,height);
    
}




