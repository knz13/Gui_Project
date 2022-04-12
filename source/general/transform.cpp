#include "transform.h"
#include "../kv.h"


void TransformComponent::Rotate(float x, float y, float z) {
    
    m_Rotation += glm::radians(glm::vec3(x,y,z));
    
}

void TransformComponent::SetRotation(glm::vec3 vec) {
    m_Rotation = vec;
}

void TransformComponent::SetRotation(float x,float y,float z) {
    m_Rotation = glm::radians(glm::vec3(x,y,z));
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



void TransformComponent::SetScale(float x, float y, float z) {
    
    m_Scale = glm::vec3(x,y,z);
    
}

void TransformComponent::InstantScaleChange(float x, float y, float z) {
   
    m_Scale += glm::vec3(x,y,z);

    
}
void TransformComponent::SetFromModelMatrix(glm::mat4 matrix) {
    
    Object current = GetMasterObject();
    
    std::vector<glm::mat4> vec;
    GetCumulativeMatrix(&vec);
    vec.erase(vec.begin());
    
    for(auto it = vec.rbegin();it != vec.rend();it++){
        matrix = glm::inverse(*it) * matrix;
    }

    glm::vec3 position,scale;
    glm::vec3 rotation;
    
    Math::DecomposeMatrix(matrix,position,rotation,scale);
    

    SetPosition(position);
    SetRotation(rotation);
    SetScale(scale);
}

glm::mat4 TransformComponent::GetModelMatrix() {
    return this->GetCumulativeMatrix();
    
}


glm::mat4 TransformComponent::GetCumulativeMatrix(std::vector<glm::mat4>* outVec){
    std::vector<glm::mat4> matrices {this->CalculateModelMatrix()};
    bool foundFinalMatrix = !GetMasterObject().Properties().GetParent();
    Object current = GetMasterObject();
    while (!foundFinalMatrix){
        if(current.Properties().GetParent()){
            current = current.Properties().GetParent().GetAsObject();
        }
        else{
            foundFinalMatrix = true;
        }
        matrices.push_back(current.Transform().CalculateModelMatrix(false));
        
    }
    if(outVec){
        *outVec = matrices;
    }
    glm::mat4 finalMat(1.0f);
    for(auto it = matrices.begin();it != matrices.end();it++){
        finalMat = *it * finalMat;
    }

    return finalMat;
};

glm::mat4 TransformComponent::CalculateModelMatrix(bool passScale) {
    if(passScale){
        return Math::CalculateModelMatrix(&m_Position,&m_Rotation,&m_Scale);
    }
    else{
        return Math::CalculateModelMatrix(&m_Position,&m_Rotation,nullptr);
    }
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

    
}


    


const glm::vec3& TransformComponent::GetScale() const {
    return m_Scale;
}

TransformComponent::TransformComponent(entt::entity e) : Component(e) {
    MakeRemovable(false);
};

TransformComponent::TransformComponent(const TransformComponent& mov) : Component(this->GetMasterObject().ID()) {
    m_Position = mov.m_Position;
    m_Rotation = mov.m_Rotation;
    m_Scale = mov.m_Scale;
}
const glm::vec3& TransformComponent::GetRotationRadians() {
    return m_Rotation;
}
