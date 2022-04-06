#pragma once
#include "../global.h"



class Movable : public EventReceiver {
    KV_CLASS
public:

    FunctionSink<void(Movable&)> Moved();
    FunctionSink<void(Movable&)> Rotated();
    FunctionSink<void(Movable&)> Scaled();
    FunctionSink<void(Movable&)> Deleted();

    void IncreaseRotationPerFrame(float x,float y,float z);
    void SetRotationIncreasePerFrame(float x,float y,float z);
    void Rotate(float x,float y,float z);
    void SetRotation(float x,float y,float z);

    void IncreaseMovementPerFrame(float x,float y,float z);
    void SetMovementIncreasePerFrame(float x,float y,float z);
    void Move(float x,float y,float z);
    void SetPosition(float x,float y,float z);

    void IncreaseScalePerFrame(float x,float y,float z);
    void SetScaleChangePerFrame(float x,float y,float z);
    void SetScale(float x,float y,float z);
    void InstantScaleChange(float x,float y,float z);

    glm::vec3 GetRotation();
    const glm::vec3& GetPosition();

    void Update(float deltaTime);
    void ShowProperties();
    glm::mat4 GetModelMatrix();
protected:



    glm::vec3 m_PositionChangePerFrame = glm::vec3(0,0,0);
    glm::vec3 m_RotationChangePerFrame = glm::vec3(0,0,0);
    glm::vec3 m_ScaleChangePerFrame = glm::vec3(0,0,0);

    glm::vec3 m_Position = glm::vec3(0,0,0);
    glm::vec3 m_Rotation = glm::vec3(0,0,0);
    glm::vec3 m_Scale = glm::vec3(1,1,1);

    EventLauncher<void(Movable&)> m_DeletedFuncs;
    EventLauncher<void(Movable&)> m_MovedFuncs;
    EventLauncher<void(Movable&)> m_RotatedFuncs;
    EventLauncher<void(Movable&)> m_ScaledFuncs;

    friend class GuiLayer;
};