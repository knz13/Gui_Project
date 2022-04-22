#pragma once
#include "../global.h"



class TransformComponent : public EventReceiver,public GameComponent<TransformComponent,AddOnlyTo<TransformComponent,GameObject>> {
    KV_CLASS
public:
    TransformComponent();
    TransformComponent(const TransformComponent& mov);


    
    void Rotate(float x,float y,float z);
    /**
     * Sets the rotation as euler angles
     */
    void SetRotation(float pitch,float yall,float row);
    /**
     * Sets the rotation as radians
     */
    void SetRotation(glm::vec3 vec);

    
    void Move(float x,float y,float z);
    void Move(glm::vec3 offset);

    void SetPosition(float x,float y,float z);
    void SetPosition(glm::vec3 pos);
    

    
    void SetScale(float x,float y,float z);
    void SetScale(glm::vec3 vec);
    void InstantScaleChange(float x,float y,float z);


    const glm::vec3& GetScale() const;
    glm::vec3 GetRotation();
    const glm::vec3& GetRotationRadians();
    const glm::vec3& GetPosition();

    void Update(float deltaTime) override;
    void ShowProperties() override;
    void SetFromModelMatrix(glm::mat4 matrix);
    glm::mat4 GetModelMatrix();


private:
    void Init() override;
    
    glm::mat4 GetCumulativeMatrix(std::vector<glm::mat4>* outVec=nullptr);
    glm::mat4 CalculateModelMatrix(bool passScale = true);

    

    glm::vec3 m_Position = glm::vec3(0,0,0);
    glm::vec3 m_Rotation = glm::vec3(0,0,0);
    glm::vec3 m_Scale = glm::vec3(1,1,1);

    

 
};

