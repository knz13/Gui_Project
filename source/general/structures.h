#pragma once
#include <iostream>
#include <map>
#include <unordered_map>
#include <functional>
#include "../vendor/glm/glm/gtc/type_ptr.hpp"
#include "../vendor/glm/glm/glm.hpp"
#include "../vendor/glm/glm/gtc/quaternion.hpp"
#include "../vendor/glm/glm/gtx/quaternion.hpp"
#include "color.h"
#include "../vendor/entt/single_include/entt/entt.hpp"
#include "../object/registry.h"

enum WindowFlag {

    None = 0,
    NotResizeable = 1,
    InitiallyMinimized = 2,
    NotFocused = 4,
    OpenGLDebugContext = 8,
    NotDecorated=16,
    FullScreen=32,
    CoreProfile=64,
    FocusOnShow=128
};

enum ShaderType {
    Vertex=0,
    Fragment,
};

struct KeyEventProperties {
    int key;
    int scancode;
    int action;
    int mods;
};

struct MouseEventProperties {
    glm::vec2 position;
};

struct MouseButtonEventProperties {
    int button;
    int action;
    int mods;
    glm::vec2 position;
};

struct WindowResizedEventProperties {
    int width;
    int height;
};

struct MouseScrollEventProperties {
    glm::vec2 offsetFromTopLeft;
};

struct Transform {
    glm::vec3 position = glm::vec3(0,0,0);
    glm::quat rotation = glm::vec3(0,0,0);
    glm::vec3 scale = glm::vec3(1,1,1);

    void Rotate(glm::vec3 eulerAnglesInDegrees) {
        rotation *= glm::quat(eulerAnglesInDegrees);
    };

    void SetLookDirection(glm::vec3 direction){
        rotation = glm::quatLookAt(direction,glm::vec3(0,1,0));
    }

    glm::vec3 Right(){
        return glm::quat(glm::vec3(0,glm::radians(90.0f),0)) * Forward();
    };

    glm::vec3 Forward() {
        return rotation * glm::vec3(0,0,-1);
    };

    glm::mat4 AsMatrix() {
        return glm::translate(glm::mat4(1.0f),position) * glm::toMat4(rotation) * glm::scale(glm::mat4(1.0f),scale);
    };

};

struct CameraProperties {

    
private:

    glm::mat4 AsProjection() {return glm::mat4(1.0f);};
    glm::mat4 AsView(Transform& transform) {
        return glm::inverse(transform.AsMatrix());
    };
    glm::mat4 AsViewProjection(Transform& transform) {
        return AsProjection() * AsView(transform);
    };

    friend class Camera;

};

struct OrthographicCameraProperties : public CameraProperties {

    float width;
    float height;

private:

    glm::mat4 AsProjection() {
        return glm::ortho(0.0f,width,0.0f,height);
    };

    friend class Camera;

};

struct PerspectiveCameraProperties : public CameraProperties {

    float fov = 65;
    float aspectRatio = 4.0f/3.0f;
    float maxRenderDistance = 1000.0f;
    float minRenderDistance = 0.1f;

private:

    glm::mat4 AsProjection() {
        return glm::perspective(glm::radians(fov),aspectRatio,minRenderDistance,maxRenderDistance);
    };

    friend class Camera;

};


struct ClickedObjectProperties {
    
    ClickedObjectProperties(entt::entity ent = entt::null){
        objectID = ent;
    };

    operator bool() const {
        return Registry::Get().valid(objectID);
    }

    entt::entity objectID = entt::null;
};




struct ShaderProperties {
    std::string source;
    unsigned int shaderType;


};

struct CameraCreationProperties {
    std::string cameraName = "camera";
    float fov = 45.0f;
    float drawDistance = 100.0f;
    float drawingNearCutoff = 0.1f;
    glm::vec3 initialPos = glm::vec3(0,0,0);
    glm::vec3 initialRotationRadians = glm::vec3(0,0,0);
    glm::vec4 viewPort = glm::vec4(0,0,1,1);
};

struct WindowCreationProperties {
    int width=640;
    int height=480;
    std::string title="Base Title";
    uint32_t windowFlags = WindowFlag::None;
    int openGLVersionMajor = 4;
    int openGLVersionMinor = 0;
};




template<typename T>
struct EventLauncher;
template<typename T>
struct FunctionSink;

struct EventReceiver {

    ~EventReceiver();

private:
    std::map<size_t,std::function<void()>> m_SubscribedEvents;

    template<typename T>
    friend struct FunctionSink;
    template<typename T>
    friend struct EventLauncher;
};


template<typename R,typename... Args>
struct EventLauncher<R(Args...)> {

    EventLauncher(){
    };

    ~EventLauncher() { 
    }

    
    bool DisconnectReceiver(size_t hash) {
        if(m_Receivers.find(hash) != m_Receivers.end()){
            m_Receivers.erase(hash);
            return true;
        }
        return false;
    };


    void EmitEvent(Args... args) {
        
        for(auto& [handle,func] : m_Receivers){
            if(func){
                (*func.get())(args...);
            }
        }
    
    };
private:
    std::unordered_map<size_t,std::shared_ptr<std::function<R(Args...)>>> m_Receivers;
    size_t m_MyHash = std::hash<void*>()((void*)this);
    
    template<typename T>
    friend struct FunctionSink;

};

struct RayCastHit {
    RayCastHit(entt::entity ent){
        hitObjectID = ent;
    }
    
    operator bool() const {
        return Registry::Get().valid(hitObjectID);
    };

    entt::entity hitObjectID = entt::null;

};





template<typename R,typename... Args>
struct FunctionSink<R(Args...)> {
    FunctionSink(EventLauncher<R(Args...)>& sink): m_Master(&sink){};

    size_t Connect(std::function<R(Args...)> windowFunc){
        static int count = 1;
        size_t hash = std::hash<int>()(count);
        count++;

        std::function<R(Args...)>* func = new std::function<R(Args...)>(windowFunc);
        m_Master->m_Receivers[hash] = std::make_shared<std::function<R(Args...)>>(*func);
        return hash;

    }

    void Connect(EventReceiver* key,std::function<R(EventReceiver*,Args...)> windowFunc) {
        
        size_t hash = std::hash<void*>()((void*)key);
        auto deleter = [=](std::function<R(Args...)>* func){
            key->m_SubscribedEvents.erase(m_Master->m_MyHash);
            delete func;
        };

        auto func = new std::function<R(Args...)>(std::bind(windowFunc,key,std::placeholders::_1,std::placeholders::_2));
        m_Master->m_Receivers[hash] = std::shared_ptr<std::function<R(Args...)>>(func,deleter);
        key->m_SubscribedEvents[m_Master->m_MyHash] = [=](){
            m_Master->DisconnectReceiver(hash);
        };
        
    };

    bool Disconnect(size_t hashKey){
        return m_Master->DisconnectReceiver(hashKey);
    }

    bool Disconnect(EventReceiver* key) {
        if(key == nullptr){
            return false;
        }
        size_t hash = std::hash<void*>()((void*)key);
        if(m_Master->m_Receivers.find(hash) != m_Master->m_Receivers.end()){
            m_Master->DisconnectReceiver(hash);
        }
        else {
            return false;
        }
        return true;
    }

private:

   

    EventLauncher<R(Args...)>* m_Master;
    
    
};











