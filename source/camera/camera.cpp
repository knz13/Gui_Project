#include "camera.h"
#include "kv.h"


std::unordered_map<std::string,Camera> Camera::m_Cameras;

void Camera::SetLookAt(float x, float y, float z) {
    glm::mat4 lookat = glm::lookAt(GetMasterObject().GetComponent<TransformComponent>().GetPosition(),glm::vec3(x,y,z),glm::vec3(0,1,0));
    glm::mat4 modelMat = glm::inverse(lookat);
    modelMat[3] = glm::vec4(0,0,0,1);
    GetMasterObject().GetComponent<TransformComponent>().SetRotation(glm::eulerAngles(glm::quat(modelMat)));
}

void Camera::SetDirection(float x, float y, float z) {
    GetMasterObject().GetComponent<TransformComponent>().SetRotation(glm::eulerAngles(glm::quatLookAt(glm::vec3(x,y,z),glm::vec3(0,1,0))));
}

glm::mat4 Camera::GetViewProjection(const Window& window) {
    return glm::perspective(glm::radians(m_Fov),(float)window.Properties().width/window.Properties().height,m_DrawNear,m_DrawDistance) * glm::inverse(GetMasterObject().GetComponent<TransformComponent>().GetModelMatrix());
}

glm::vec4 Camera::GetViewPort() {
    return m_ViewPort;
}

void Camera::Update(float deltaTime) {
    
}


Camera::~Camera() {
    
}

void Camera::LookAt(Object& obj) {
    this->SetLookAt(obj.GetComponent<TransformComponent>().GetPosition().x,obj.GetComponent<TransformComponent>().GetPosition().y,obj.GetComponent<TransformComponent>().GetPosition().z);
}

glm::mat4 Camera::GetProjection(float viewPortWidth, float viewPortHeight) {
    return glm::perspective(glm::radians(m_Fov),viewPortWidth/viewPortHeight,m_DrawNear,m_DrawDistance);
}

glm::mat4 Camera::GetView() {
    return glm::inverse(GetMasterObject().GetComponent<TransformComponent>().GetModelMatrix());
}

void Camera::ShowProperties() {
    static std::string fovRand = ("##" + std::to_string(Registry::GenerateRandomNumber()));
    bool isCurrent;
    if(Window::GetCurrentWindow().GetCurrentCamera()){
        isCurrent = (Window::GetCurrentWindow().GetCurrentCamera().GetAsObject().ID() == this->GetMasterObject().ID());
    }
    else{
        isCurrent = false;
    }
    bool stateHolder = isCurrent;

    ImGui::Checkbox("Set Current",&isCurrent);

    if(stateHolder && !isCurrent){
        Window::GetCurrentWindow().DisableCamera();
    }

    if(!stateHolder && isCurrent){
        Window::GetCurrentWindow().SetCamera(GetMasterObject());
    }


    ImGui::BulletText("Fov");
    ImGui::DragFloat(GuiLayer::GetImGuiID(&m_Fov).c_str(),&m_Fov,0.1,0,180);
    ImGui::BulletText("Render Distance");
    ImGui::DragFloat(GuiLayer::GetImGuiID(&m_DrawDistance).c_str(),&m_DrawDistance,0.1,0);
    ImGui::BulletText("Render Cuttoff");
    ImGui::DragFloat(GuiLayer::GetImGuiID(&m_DrawNear).c_str(),&m_DrawNear,0.1,0);


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

    GetMasterObject().GetComponent<TransformComponent>().Move(right * rightLeft);
    GetMasterObject().GetComponent<TransformComponent>().Move(up * upDown);
    GetMasterObject().GetComponent<TransformComponent>().Move(look * frontBack);

}

glm::vec3 Camera::GetLookDirection() {
    glm::mat4 modelView = GetView() * GetMasterObject().GetComponent<TransformComponent>().GetModelMatrix();
    glm::vec3 look  = glm::vec3(modelView[0][2],modelView[1][2],modelView[2][2]);
    look = glm::normalize(look);
    return look;
}
