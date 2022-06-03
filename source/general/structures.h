#pragma once
#include "../includes.h"
#include "object/game_object.h"
#include "gui_layer/gui_useful_implementations.h"

using namespace std;




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
    glm::vec2 offset;
};










struct ShaderProperties {
    std::string source;
    unsigned int shaderType;


};

struct CameraCreationProperties {
    std::string cameraName = "camera";
    float fov = 60.0f;
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

struct EventReceiver;
struct EventReceiverAttachmentProperties {
    std::function<void(EventReceiver*)> m_CopyFunc;
    std::function<void(void*,size_t)> m_DeleteFunc;
};


template<typename T>
struct EventLauncher;
template<typename T>
struct FunctionSink;

struct EventReceiver {

    ~EventReceiver();

    EventReceiver& operator=(const EventReceiver& other);

private:
    std::map<void*,EventReceiverAttachmentProperties> m_SubscribedEvents;

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

    EventLauncher<R(Args...)>& operator=(const EventLauncher<R(Args...)>& other){
        return *this;
    }

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
        return ecspp::Registry().valid(hitObjectID);
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
            key->m_SubscribedEvents.erase((void*)m_Master);
            delete func;
        };

        auto func = new std::function<R(Args...)>([=](auto... args){
            windowFunc(key,args...);
        });
        m_Master->m_Receivers[hash] = std::shared_ptr<std::function<R(Args...)>>(func,deleter);
        
        EventReceiverAttachmentProperties prop;
        prop.m_CopyFunc = [=](EventReceiver* receiver){
            FunctionSink<R(Args...)>(*m_Master).Connect(receiver,windowFunc);
        };

        prop.m_DeleteFunc = [](void* ptr,size_t hash){
                ((EventLauncher<R(Args...)>*)(ptr))->DisconnectReceiver(hash);
        };

        key->m_SubscribedEvents[(void*)m_Master] = std::move(prop);
        
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











