#pragma once
#include "../global.h"

class Framebuffer;
class Object;
class Window;
class Camera : public DeriveFromComponent<Camera> {

    KV_CLASS
    
public: 
    Camera(CameraCreationProperties prop);
    Camera();
    ~Camera();

    

    void MoveInRelationToView(float rightLeft,float upDown,float frontBack);
    void LookAt(Object& obj);
    void SetLookAt(float x,float y,float z);
    void SetDirection(float x,float y,float z);
    
    
    glm::mat4 GetViewProjection(const Window& window);
    glm::mat4 GetView() const;
    glm::mat4 GetProjection() const;
    

    glm::vec4 GetViewPort() const;
    glm::vec3 GetLookDirection() const;
    glm::vec2 GetViewportSize() const;
    Framebuffer& GetRenderTarget();

    bool HasRenderTarget();
    void Render();
    
    void SetDrawingFunction(std::function<void(Camera&)> drawingFunc);
    void SetRenderTarget(std::shared_ptr<Framebuffer> framebuffer);
    void SetViewport(float x,float y,float width,float height);

    void Update(float deltaTime);
    void ShowProperties();

private:
    void Init() override;
    void Destroy() override;

    std::function<void(Camera&)> m_DrawingFunc;
    std::shared_ptr<Framebuffer> m_RenderTarget;
    glm::vec4 m_ViewPort = glm::vec4(0,0,0,0);
    float m_DrawNear = 0;
    float m_DrawDistance = 100.0f;
    float m_Fov = 45;
    
    
    //static members

   


};