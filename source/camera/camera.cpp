#include "camera.h"
#include "kv.h"


std::unordered_map<std::string,Camera> Camera::m_Cameras;

void Camera::SetLookAt(float x, float y, float z) {
    glm::mat4 lookat = glm::lookAt(m_Position,glm::vec3(x,y,z),glm::vec3(0,1,0));
    glm::mat4 modelMat = glm::inverse(lookat);
    modelMat[3] = glm::vec4(0,0,0,1);
    m_Rotation = glm::eulerAngles(glm::quat(modelMat));
}

void Camera::SetDirection(float x, float y, float z) {
    m_Rotation = glm::eulerAngles(glm::quatLookAt(glm::vec3(x,y,z),glm::vec3(0,1,0)));
}

glm::mat4 Camera::GetViewProjection(const Window& window) {
    return glm::perspective(m_Fov,(float)window.Properties().width/window.Properties().height,m_DrawNear,m_DrawDistance) * glm::inverse(glm::translate(glm::mat4(1.0f),m_Position)* glm::toMat4(glm::quat(m_Rotation)));
}

Camera::Camera(Window* win) : m_CurrentWindow(win) {
    
}

glm::vec4 Camera::GetViewPort() {
    return m_ViewPort;
}

Camera& Camera::GeneratePerspectiveCamera(CameraCreationProperties prop, Window& windowToSetCurrentOn) {
    Camera camera(&windowToSetCurrentOn);

    camera.m_Fov = prop.fov;
    camera.m_DrawNear = prop.drawingNearCutoff;
    camera.m_DrawDistance = prop.drawDistance;
    camera.m_Position = prop.initialPos;
    camera.m_Rotation = prop.initialRotationRadians;
    camera.m_ViewPort = prop.viewPort;
    

    if(Camera::m_Cameras.find(prop.cameraName) != Camera::m_Cameras.end()){
        prop.cameraName += "_1";
        int index = 1;
        while(Camera::m_Cameras.find(prop.cameraName) != Camera::m_Cameras.end()){
            prop.cameraName = prop.cameraName.substr(0,prop.cameraName.size()-3) + "_" + std::to_string(index);
            index++;
        }
    }

    Camera::m_Cameras[prop.cameraName] = std::move(camera);

    windowToSetCurrentOn.SetCamera(Camera::m_Cameras[prop.cameraName]);

    return Camera::m_Cameras[prop.cameraName];
}

void Camera::Update(float deltaTime) {
    
}


Camera::~Camera() {
    
}

void Camera::LookAt(Movable& mov) {
    this->SetLookAt(mov.GetPosition().x,mov.GetPosition().y,mov.GetPosition().z);
}
