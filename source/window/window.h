#pragma once 
#include "../global.h"
#include <functional>

class Camera;
class Framebuffer;
class Mesh;
class Texture;
class Window;
class VertexArray;
class Shader;
struct WindowEvents {

    WindowEvents(Window& win) : m_Master(win) {};


    FunctionSink<void(Window&,MouseScrollEventProperties)> MouseScrollEvent();
    FunctionSink<void(Window&,MouseButtonEventProperties)> MouseButtonEvent();
    FunctionSink<void(Window&,MouseEventProperties)> MouseEnteredWindowEvent();
    FunctionSink<void(Window&,MouseEventProperties)> MouseLeftWindowEvent();
    FunctionSink<void(Window&,KeyEventProperties)> KeyEvent();
    FunctionSink<void(Window&,MouseEventProperties)> MouseEvent();
    FunctionSink<void(Window&)> PreDrawingLoopEvent();
    FunctionSink<void(Window&)> PostDrawingLoopEvent();
    FunctionSink<void(Window&)> ClosingEvent();
    FunctionSink<void(Window&,WindowResizedEventProperties)> ResizedEvent();

private:
    Window& m_Master;

};



class WindowCreators {
public:
    WindowCreators(Window& wid) : m_Master(wid){}

    VertexArray& NewVertexArray();
    Shader& CachedShader(std::string shaderRelativePath,bool* loadResult=nullptr);
    

private:
    Window& m_Master;


};

class ObjectHandle;
class Window {

    KV_CLASS

public:

    Window(WindowCreationProperties prop);
    
    ~Window();

    bool IsOpen();
    
    WindowEvents Events();

    WindowCreators Create();
    

    ObjectHandle GetCurrentCamera();
    void SetClearColor(Color color);
    
    void DisableCamera();
    void SetCamera(Object obj);

    const WindowCreationProperties& Properties() const;
    GLFWwindow* GetContextPointer();
    float GetDeltaTime();

    void DrawingLoop();
    
    
    static Window& GetCurrentWindow();
    static Window* GetWindow(GLFWwindow* win);
    static FunctionSink<void(Window&)> WindowCreationEvent();
    
    RayCastHit RayCast(glm::vec2 screenPos);
    

protected:

    friend class Mesh;

private:

    void BeginDrawState();
    void EndDrawState();
    void PostDrawOperations();
    
    reactphysics3d::PhysicsWorld* m_PhysicsWorld;
    bool m_IsOpen = true;
    WindowCreationProperties m_Properties;
    GLFWwindow* m_ContextPointer=nullptr;

    entt::entity m_MainCamera = entt::null;
    


    float m_DeltaTime = 0;


    std::vector<std::unique_ptr<VertexArray>> m_CreatedVertexArrays;
    std::map<std::string,std::unique_ptr<Shader>> m_CreatedShaders;

    
    EventLauncher<void(Window&,WindowResizedEventProperties)> m_WindowResizedEventFuncs;
    EventLauncher<void(Window&,MouseEventProperties)> m_MouseMovedFuncs;
    EventLauncher<void(Window&,MouseEventProperties)> m_MouseEnteredWindowFuncs;
    EventLauncher<void(Window&,MouseEventProperties)> m_MouseLeftWindowFuncs;
    EventLauncher<void(Window&,MouseButtonEventProperties)> m_MouseButtonFuncs;
    EventLauncher<void(Window&,MouseScrollEventProperties)> m_MouseScrollFuncs;
    EventLauncher<void(Window&,KeyEventProperties)> m_KeyEventFuncs;
    EventLauncher<void(Window&)> m_PreDrawingLoopFuncs;
    EventLauncher<void(Window&)> m_PostDrawingLoopFuncs;
    EventLauncher<void(Window&)> m_ClosingCallbackFuncs;
    Color m_ClearColor = Color::Black;



    //static members

    static Window* m_MainWindow;
    static EventLauncher<void(Window&)> m_StartWindowFuncs;

    static std::map<GLFWwindow*,Window*> m_CurrentWindows;

    friend struct WindowCreators;
    friend struct WindowEvents;


};