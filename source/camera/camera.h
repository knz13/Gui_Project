#pragma once
#include "../global.h"
#include "../general/movable.h"

class Window;
class Camera : public Movable {

    KV_CLASS
    
public: 
    Camera(Window* win=nullptr);
    ~Camera();

    void LookAt(Movable& mov);
    void SetLookAt(float x,float y,float z);
    void SetDirection(float x,float y,float z);
    
    
    glm::mat4 GetViewProjection(const Window& window);

    glm::vec4 GetViewPort();

protected:

    virtual void Update(float deltaTime);

private:

    
    Window* m_CurrentWindow = nullptr;
    float m_DrawNear = 0;
    float m_DrawDistance = 100.0f;
    float m_Fov = 45;
    glm::vec4 m_ViewPort = glm::vec4(0,0,1,1);
    
    //static members

    static Camera& GeneratePerspectiveCamera(CameraCreationProperties prop,Window& windowToSetCurrentOn);

    static std::unordered_map<std::string,Camera> m_Cameras;


};