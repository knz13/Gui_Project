#pragma once
#include "../global.h"



class Movable : public EventReceiver,public Component {
    KV_CLASS
public:
    Movable(entt::entity e);
    Movable(const Movable& mov);

    

    void IncreaseRotationPerFrame(float x,float y,float z);
    void SetRotationIncreasePerFrame(float x,float y,float z);
    void Rotate(float x,float y,float z);
    /**
     * Sets the rotation as euler angles
     */
    void SetRotation(float pitch,float yall,float row);
    /**
     * Sets the rotation as radians
     */
    void SetRotation(glm::vec3 vec);

    void IncreaseMovementPerFrame(float x,float y,float z);
    void SetMovementIncreasePerFrame(float x,float y,float z);
    void Move(float x,float y,float z);
    void Move(glm::vec3 offset);

    void SetPosition(float x,float y,float z);
    void SetPosition(glm::vec3 pos);

    void IncreaseScalePerFrame(float x,float y,float z);
    void SetScaleChangePerFrame(float x,float y,float z);
    void SetScale(float x,float y,float z);
    void SetScale(glm::vec3 vec);
    void InstantScaleChange(float x,float y,float z);


    const glm::vec3& GetScale() const;
    glm::vec3 GetRotation();
    const glm::vec3& GetPosition();

    void Update(float deltaTime);
    void ShowProperties();
    glm::mat4 GetModelMatrix();
protected:
    std::function<nlohmann::json()> GetSerializerFunction();

private:

    glm::vec3 m_PositionChangePerFrame = glm::vec3(0,0,0);
    glm::vec3 m_RotationChangePerFrame = glm::vec3(0,0,0);
    glm::vec3 m_ScaleChangePerFrame = glm::vec3(0,0,0);

    glm::vec3 m_Position = glm::vec3(0,0,0);
    glm::vec3 m_Rotation = glm::vec3(0,0,0);
    glm::vec3 m_Scale = glm::vec3(1,1,1);

    

 
};