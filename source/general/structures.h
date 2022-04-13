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
#include "../vendor/entt/single_include/entt/entt.hpp"
#include "../vendor/glew/include/GL/glew.h"
#include "../vendor/glfw/include/GLFW/glfw3.h"
#include "../vendor/thread_pool/thread_pool.hpp"
#include "../vendor/glm/glm/gtc/type_ptr.hpp"
#include "../vendor/assimp/include/assimp/Importer.hpp"
#include "../vendor/assimp/include/assimp/scene.h"
#include "../vendor/assimp/include/assimp/postprocess.h"
#include "../vendor/assimp/include/assimp/matrix4x4.h"
#include "../vendor/assimp/include/assimp/cimport.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "gui_layer/gui_useful_implementations.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "../object/object.h"
#include "../object/component.h"
using namespace std;


#define GL_SIZEOF(x) TestSize(x)
#define KV_CLASS private: \
                    friend class Registry; friend class Window; friend class Object;
#define KV_DRAWING_MODE private: friend class Mesh;
#define KV_REGISTER_CLASS_AS_COMPONENT(className) void* __UNUSEDVARIABLE##className = [](){ Object::RegisterClassAsComponent<className>();return nullptr;}()

static unsigned int TestSize(unsigned int dataType) {
    switch (dataType) {
    case GL_FLOAT:
        return sizeof(float);
    case GL_UNSIGNED_INT:
        return sizeof(unsigned int);
    case GL_UNSIGNED_BYTE:
        return sizeof(unsigned char);
    case GL_BYTE:
        return sizeof(char);
    case GL_INT:
        return sizeof(int);
    default:
        return 0;
    }
}


#define LOG(x) cout << "LOG: " << x << endl <<  "At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl


#ifdef NDEBUG
    #define DEBUG_LOG(x)
    #define DEBUG_WARN(x)
    #define DEBUG_ERROR(x)
#else
    #define DEBUG_LOG(x) cout << "LOG: " << x << endl <<  "At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl
    #define DEBUG_WARN(x) cout << "WARNING: " << x << endl <<  "At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl
    #define DEBUG_ERROR(x) cout << "ERROR! -> " << x  << endl <<  "At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl; __debugbreak()
#endif

static void ClearGLErrors(){
    while(glGetError() != GL_NO_ERROR){
        ;
    }
}

static bool GetGLError(int line,std::string file){
    GLenum code = glGetError();
    if(code != GL_NO_ERROR){
        cout << "OpenGL error '" << gluErrorString(code) << "' \nAt line: " << line << " \nIn file: " << file << endl;
        return true;
    }
    return false;
}

#ifdef NDEBUG
    #define GL_CALL(x) x 
    #define GL_CALL_WITH_RESULT(var,x) var = x 
#else
    #define GL_CALL(x) ClearGLErrors(); x; if(GetGLError(__LINE__,__FILE__)) {;}
    #define GL_CALL_WITH_RESULT(var,x) ClearGLErrors(); var = x; if(GetGLError(__LINE__,__FILE__)) {;}
#endif



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

struct RandomDummyComponent : public Component<RandomDummyComponent> {
    void Update(float) {};
    void ShowProperties() {};
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
        std::cout << m_Receivers.size() << std::endl;
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
            key->m_SubscribedEvents.erase((void*)m_Master);
            delete func;
        };

        auto func = new std::function<R(Args...)>(std::bind(windowFunc,key,std::placeholders::_1,std::placeholders::_2));
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











