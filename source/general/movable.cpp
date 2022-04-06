#include "movable.h"

void Movable::IncreaseRotationPerFrame(float x, float y, float z) {
    m_RotationChangePerFrame += glm::radians(glm::vec3(x,y,z));
}

void Movable::SetRotationIncreasePerFrame(float x, float y, float z) {
    m_RotationChangePerFrame = glm::radians(glm::vec3(x,y,z));
}

void Movable::Rotate(float x, float y, float z) {
    if(x != 0 || y != 0 || z != 0){
        m_RotatedFuncs.EmitEvent(*this);
    }
    m_Rotation += glm::radians(glm::vec3(x,y,z));
    m_RotatedFuncs.EmitEvent(*this);
}

void Movable::SetRotation(float x, float y, float z) {
    if(m_Rotation != glm::radians(glm::vec3(x,y,z))){
        m_RotatedFuncs.EmitEvent(*this);
    }

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
    m_MovedFuncs.EmitEvent(*this);
}

void Movable::SetPosition(float x, float y, float z) {
    if(m_Position !=  glm::vec3(x,y,z)){
        m_MovedFuncs.EmitEvent(*this);
    }
    m_Position = glm::vec3(x,y,z);
}

void Movable::IncreaseScalePerFrame(float x, float y, float z) {
    m_ScaleChangePerFrame += glm::vec3(x,y,z);
}

void Movable::SetScaleChangePerFrame(float x, float y, float z) {
    m_ScaleChangePerFrame = glm::vec3(x,y,z);
    
}

void Movable::SetScale(float x, float y, float z) {
    if(m_Scale != glm::vec3(x,y,z)){
        m_ScaledFuncs.EmitEvent(*this);
    }
    m_Scale = glm::vec3(x,y,z);
    
}

void Movable::InstantScaleChange(float x, float y, float z) {
    if(x != 0 || y != 0 || z != 0){
        m_ScaledFuncs.EmitEvent(*this);
    }

    m_Scale += glm::vec3(x,y,z);

    
}



glm::mat4 Movable::GetModelMatrix() {
    return glm::translate(glm::mat4(1.0f),m_Position) * glm::toMat4(glm::quat(m_Rotation)) * glm::scale(glm::mat4(1.0f),m_Scale);
}

glm::vec3 Movable::GetRotation() {
    return glm::degrees(glm::vec3(m_Rotation.x,-m_Rotation.y,m_Rotation.z));
}

const glm::vec3& Movable::GetPosition() {
    return m_Position;
}

FunctionSink<void(Movable&)> Movable::Moved() {
    return FunctionSink<void(Movable&)>(m_MovedFuncs);
}

FunctionSink<void(Movable&)> Movable::Rotated() {
    return FunctionSink<void(Movable&)>(m_RotatedFuncs);
}

FunctionSink<void(Movable&)> Movable::Scaled() {
    return FunctionSink<void(Movable&)>(m_ScaledFuncs);
}

FunctionSink<void(Movable&)> Movable::Deleted() {
    return FunctionSink<void(Movable&)>(m_DeletedFuncs);
}

void Movable::ShowProperties() {
    
    ImGui::InputFloat3("Position",(float*)&m_Position);
    ImGui::InputFloat3("Rotation",(float*)&m_Rotation);
    ImGui::InputFloat3("Scale",(float*)&m_Scale);


    
}

void Movable::Update(float deltaTime) {
    if(m_PositionChangePerFrame != glm::vec3(0)){
        m_MovedFuncs.EmitEvent(*this);
    }
    if(m_RotationChangePerFrame != glm::vec3(0)){
        m_RotatedFuncs.EmitEvent(*this);
    }
    if(m_ScaleChangePerFrame != glm::vec3(0)){
        m_ScaledFuncs.EmitEvent(*this);
    }

    m_Position += m_PositionChangePerFrame * deltaTime;
    m_Rotation += m_RotationChangePerFrame * deltaTime;
    m_Scale += m_ScaleChangePerFrame * deltaTime;
}
