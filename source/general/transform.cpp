#include "transform.h"
#include "../kv.h"

void TransformComponent::IncreaseRotationPerFrame(float x, float y, float z) {
    m_RotationChangePerFrame += glm::radians(glm::vec3(x,y,z));
}

void TransformComponent::SetRotationIncreasePerFrame(float x, float y, float z) {
    m_RotationChangePerFrame = glm::radians(glm::vec3(x,y,z));
}

void TransformComponent::Rotate(float x, float y, float z) {
    
    m_Rotation += glm::radians(glm::vec3(x,y,z));
    
}

void TransformComponent::SetRotation(glm::vec3 vec) {
    m_Rotation = vec;
}

void TransformComponent::SetRotation(float x,float y,float z) {
    m_Rotation = glm::radians(glm::vec3(x,y,z));
}

void TransformComponent::IncreaseMovementPerFrame(float x, float y, float z) {
    m_PositionChangePerFrame += glm::vec3(x,y,z);
}

void TransformComponent::SetMovementIncreasePerFrame(float x, float y, float z) {
    m_PositionChangePerFrame = glm::vec3(x,y,z);
}

void TransformComponent::Move(float x, float y, float z) {
    m_Position += glm::vec3(x,y,z);
    
}

void TransformComponent::Move(glm::vec3 offset) {
    m_Position += offset;
}

void TransformComponent::SetPosition(float x, float y, float z) {
    
    m_Position = glm::vec3(x,y,z);
}

void TransformComponent::SetPosition(glm::vec3 pos) {
    m_Position = pos;
}

void TransformComponent::IncreaseScalePerFrame(float x, float y, float z) {
    m_ScaleChangePerFrame += glm::vec3(x,y,z);
}

void TransformComponent::SetScaleChangePerFrame(float x, float y, float z) {
    m_ScaleChangePerFrame = glm::vec3(x,y,z);
    
}

void TransformComponent::SetScale(float x, float y, float z) {
    
    m_Scale = glm::vec3(x,y,z);
    
}

void TransformComponent::InstantScaleChange(float x, float y, float z) {
   
    m_Scale += glm::vec3(x,y,z);

    
}



glm::mat4 TransformComponent::GetModelMatrix() {
    // getting the parent transforms
    glm::mat4 finalMatrix = this->CalculateModelMatrix();
    bool foundFinalMatrix = !GetMasterObject().Properties().GetParent();
    Object current = GetMasterObject();
    while (!foundFinalMatrix){
        if(current.Properties().GetParent()){
            current = Object(GetMasterObject().Properties().GetParent().Handle());
        }
        else{
            foundFinalMatrix = true;
        }
        finalMatrix *= current.Transform().CalculateModelMatrix();
        
    }

    return finalMatrix;
    
}

glm::mat4 TransformComponent::CalculateModelMatrix() {
    return glm::translate(glm::mat4(1.0f),m_Position) * glm::toMat4(glm::quat(m_Rotation)) * glm::scale(glm::mat4(1.0f),m_Scale);
}

glm::vec3 TransformComponent::GetRotation() {
    return glm::degrees(glm::vec3(m_Rotation.x,m_Rotation.y,m_Rotation.z));
}

const glm::vec3& TransformComponent::GetPosition() {
    return m_Position;
}

void TransformComponent::SetScale(glm::vec3 vec) {
    m_Scale = vec;
}

void TransformComponent::ShowProperties() {
    
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

void TransformComponent::Update(float deltaTime) {

    m_Position += m_PositionChangePerFrame * deltaTime;
    m_Rotation += m_RotationChangePerFrame * deltaTime;
    m_Scale += m_ScaleChangePerFrame * deltaTime;
}


    


const glm::vec3& TransformComponent::GetScale() const {
    return m_Scale;
}

TransformComponent::TransformComponent(entt::entity e) : Component(e) {

};

TransformComponent::TransformComponent(const TransformComponent& mov) : Component(this->GetMasterObject().ID()) {
    m_Position = mov.m_Position;
    m_Rotation = mov.m_Rotation;
    m_Scale = mov.m_Scale;

    m_PositionChangePerFrame = mov.m_PositionChangePerFrame;
    m_RotationChangePerFrame = mov.m_RotationChangePerFrame;
    m_ScaleChangePerFrame = mov.m_ScaleChangePerFrame;
}