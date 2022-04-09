#include "movable.h"
#include "../kv.h"

void Movable::IncreaseRotationPerFrame(float x, float y, float z) {
    m_RotationChangePerFrame += glm::radians(glm::vec3(x,y,z));
}

void Movable::SetRotationIncreasePerFrame(float x, float y, float z) {
    m_RotationChangePerFrame = glm::radians(glm::vec3(x,y,z));
}

void Movable::Rotate(float x, float y, float z) {
    
    m_Rotation += glm::radians(glm::vec3(x,y,z));
    
}

void Movable::SetRotation(glm::vec3 vec) {
    m_Rotation = vec;
}

void Movable::SetRotation(float x,float y,float z) {
    m_Rotation = glm::radians(glm::vec3(x,y,z));
}

void Movable::IncreaseMovementPerFrame(float x, float y, float z) {
    m_PositionChangePerFrame += glm::vec3(x,y,z);
}

void Movable::SetMovementIncreasePerFrame(float x, float y, float z) {
    m_PositionChangePerFrame = glm::vec3(x,y,z);
}

void Movable::Move(float x, float y, float z) {
    m_Position += glm::vec3(x,y,z);
    
}

void Movable::Move(glm::vec3 offset) {
    m_Position += offset;
}

void Movable::SetPosition(float x, float y, float z) {
    
    m_Position = glm::vec3(x,y,z);
}

void Movable::SetPosition(glm::vec3 pos) {
    m_Position = pos;
}

void Movable::IncreaseScalePerFrame(float x, float y, float z) {
    m_ScaleChangePerFrame += glm::vec3(x,y,z);
}

void Movable::SetScaleChangePerFrame(float x, float y, float z) {
    m_ScaleChangePerFrame = glm::vec3(x,y,z);
    
}

void Movable::SetScale(float x, float y, float z) {
    
    m_Scale = glm::vec3(x,y,z);
    
}

void Movable::InstantScaleChange(float x, float y, float z) {
   
    m_Scale += glm::vec3(x,y,z);

    
}



glm::mat4 Movable::GetModelMatrix() {
    return glm::translate(glm::mat4(1.0f),m_Position) * glm::toMat4(glm::quat(m_Rotation)) * glm::scale(glm::mat4(1.0f),m_Scale);
}

glm::vec3 Movable::GetRotation() {
    return glm::degrees(glm::vec3(m_Rotation.x,m_Rotation.y,m_Rotation.z));
}

const glm::vec3& Movable::GetPosition() {
    return m_Position;
}

void Movable::SetScale(glm::vec3 vec) {
    m_Scale = vec;
}

void Movable::ShowProperties() {
    
    ImGui::BulletText("Position");
    ImGui::DragFloat3(GuiLayer::GetImGuiID(&m_Position).c_str(),(float*)&m_Position,0.1);
    glm::vec3 rotationAsAngles = GetRotation();
    

    
    ImGui::BulletText("Rotation");
    ImGui::DragFloat3(GuiLayer::GetImGuiID(&m_Rotation).c_str(),(float*)&rotationAsAngles,0.1);
    

    glm::vec3 rotationAsRadians = glm::radians(rotationAsAngles);

    m_Rotation += rotationAsRadians - m_Rotation;
    
    ImGui::BulletText("Scale");
    ImGui::DragFloat3(GuiLayer::GetImGuiID(&m_Scale).c_str(),(float*)&m_Scale,0.01f,0);
    

    
}

void Movable::Update(float deltaTime) {

    m_Position += m_PositionChangePerFrame * deltaTime;
    m_Rotation += m_RotationChangePerFrame * deltaTime;
    m_Scale += m_ScaleChangePerFrame * deltaTime;
}


    


const glm::vec3& Movable::GetScale() const {
    return m_Scale;
}

Movable::Movable(entt::entity e) : Component(e) {

};

Movable::Movable(const Movable& mov) : Component(this->GetMasterObject().ID()) {
    m_Position = mov.m_Position;
    m_Rotation = mov.m_Rotation;
    m_Scale = mov.m_Scale;

    m_PositionChangePerFrame = mov.m_PositionChangePerFrame;
    m_RotationChangePerFrame = mov.m_RotationChangePerFrame;
    m_ScaleChangePerFrame = mov.m_ScaleChangePerFrame;
}