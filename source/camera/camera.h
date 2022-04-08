#pragma once
#include "../global.h"

class Object;
class Window;
class Camera : public Component {

    KV_CLASS
    
public: 
    Camera(CameraCreationProperties prop,entt::entity ent);
    Camera(entt::entity ent);
    ~Camera();


    void MoveInRelationToView(float rightLeft,float upDown,float frontBack);
    void LookAt(Object& obj);
    void SetLookAt(float x,float y,float z);
    void SetDirection(float x,float y,float z);
    
    
    glm::mat4 GetViewProjection(const Window& window);
    glm::mat4 GetView();
    glm::mat4 GetProjection(float viewPortWidth,float viewPortHeight);

    glm::vec4 GetViewPort();
    glm::vec3 GetLookDirection();

    void Update(float deltaTime);
    void ShowProperties();

private:
    float m_DrawNear = 0;
    float m_DrawDistance = 100.0f;
    float m_Fov = 45;
    glm::vec4 m_ViewPort = glm::vec4(0,0,1,1);
    
    //static members

    

    static std::unordered_map<std::string,Camera> m_Cameras;


};