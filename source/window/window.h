#pragma once 
#include "../global.h"
#include <functional>

class Camera;
class Framebuffer;
class Mesh;
class Window;
class VertexArray;
class Shader;
struct WindowEvents {

    WindowEvents(Window& win) : m_Master(win) {};

    FunctionSink<void(Window&, bool)> FocusEvent();
    FunctionSink<void(Window&,SDL_MouseWheelEvent)> MouseScrollEvent();
    FunctionSink<void(Window&,SDL_MouseButtonEvent)> MouseButtonEvent();
    FunctionSink<void(Window&,MouseEventProperties)> MouseEnteredWindowEvent();
    FunctionSink<void(Window&,MouseEventProperties)> MouseLeftWindowEvent();
    FunctionSink<void(Window&,SDL_KeyboardEvent)> KeyEvent();
    FunctionSink<void(Window&,SDL_MouseMotionEvent)> MouseMovedEvent();
    FunctionSink<void(Window&)> PreDrawingLoopEvent();
    FunctionSink<void(Window&)> PostDrawingLoopEvent();
    FunctionSink<void(Window&)> ClosingEvent();
    FunctionSink<void(Window&,WindowResizedEventProperties)> ResizedEvent();
    FunctionSink<void(Window&, SDL_Event&)> AllEvents();

private:
    Window& m_Master;

};





class Window {

    KV_CLASS

public:

    Window(WindowCreationProperties prop);
    
    ~Window();

    bool IsOpen();
    
    WindowEvents Events();

   
    
    ecspp::NamedObjectHandle<GameObject> GetCurrentCamera();
    void SetClearColor(Color color);
    
    void DisableCamera();
    void SetCamera(GameObject obj);
    
    void SetViewPort(int x,int y,int width,int height);

    const WindowCreationProperties& Properties() const;
    SDL_GLContext& GetContext();
    float GetDeltaTime();
    

    void DrawingLoop();
    
    
    static Window& GetCurrentWindow();
   
    static FunctionSink<void(Window&)> WindowCreationEvent();
    
    RayCastHit RayCast(glm::vec2 screenPos);
    

    SDL_Window* GetWindowPointer();

    glm::vec2 GetMousePos();

protected:

    friend class Mesh;

private:

    void BeginDrawState();
    void EndDrawState();
    void PostDrawOperations();
    
    
    bool m_IsOpen = true;
    WindowCreationProperties m_Properties;
    //GLFWwindow* m_ContextPointer=nullptr;
    SDL_Window* m_WindowPointer;
    SDL_GLContext m_Context;

    
    entt::entity m_MainCamera = entt::null;
    
    

    float m_DeltaTime = 0;

    EventLauncher<void(Window&, bool)> m_FocusEventFuncs;
    EventLauncher<void(Window&,WindowResizedEventProperties)> m_WindowResizedEventFuncs;
    EventLauncher<void(Window&,SDL_MouseMotionEvent)> m_MouseMovedFuncs;
    EventLauncher<void(Window&,MouseEventProperties)> m_MouseEnteredWindowFuncs;
    EventLauncher<void(Window&,MouseEventProperties)> m_MouseLeftWindowFuncs;
    EventLauncher<void(Window&,SDL_MouseButtonEvent)> m_MouseButtonFuncs;
    EventLauncher<void(Window&,SDL_MouseWheelEvent)> m_MouseScrollFuncs;
    EventLauncher<void(Window&,SDL_KeyboardEvent)> m_KeyEventFuncs;
    EventLauncher<void(Window&, SDL_Event&)> m_EventCallback;
    EventLauncher<void(Window&)> m_PreDrawingLoopFuncs;
    EventLauncher<void(Window&)> m_PostDrawingLoopFuncs;
    EventLauncher<void(Window&)> m_ClosingCallbackFuncs;
    Color m_ClearColor = Color::Black;



    //static members

    static Window* m_MainWindow;
    static EventLauncher<void(Window&)> m_StartWindowFuncs;

   

    friend struct WindowCreators;
    friend struct WindowEvents;


};