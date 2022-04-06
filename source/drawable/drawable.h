#pragma once
#include "../global.h"
#include "../opengl_wrappers/vertex_array.h"
#include "drawing_modes.h"
#include "../general/movable.h"

class Shader;
class Drawable;


class Drawable : public EventReceiver{

    KV_CLASS

public:
    Drawable(Window* win=nullptr);
    ~Drawable();

    bool SetShader(std::string shaderLocation);
    void SetActive(Window& win);
    void SetActive();
    void SetInactive();


    void SetDrawingMode(DrawingMode mode);

    FunctionSink<void(Drawable&,Shader&)> PreDrawn();
    FunctionSink<void(Drawable&)> PostDrawn();

    VertexArray& GetVertexArray();

    void Update(float deltaTime);
    void ShowProperties();
protected:

    


private:
    
    void Draw();
    bool ReadyToDraw();
    

    DrawingMode m_DrawingMode;
    Window* m_CurrentWindow = nullptr;
    VertexArray* m_VAO=nullptr;
    std::string m_ShaderName = "";
    bool m_Active = true;
    unsigned int m_ID;

    
    EventLauncher<void(Drawable&,Shader&)> m_PreDrawFuncs;
    EventLauncher<void(Drawable&)> m_PostDrawFuncs;
    EventLauncher<void(Drawable&)> m_DeletedFuncs;

    //static members



    static void DestroyDrawableHandle(unsigned int id);
    static unsigned int CreateDrawableHandle(Drawable& dr);

    static std::unordered_map<unsigned int,Drawable*> m_DrawableObjects;


    friend class GuiLayer;
    friend class DrawableEvents;
};