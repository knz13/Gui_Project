#pragma once
#include "../global.h"
#include "../opengl_wrappers/vertex_array.h"
#include "drawing_modes.h"
#include "../general/movable.h"
#include "../object/component.h"

class Shader;
class Drawable;


class Drawable : public EventReceiver,public Component{

    KV_CLASS

public:
    Drawable(entt::entity master);
    ~Drawable();

    bool SetShader(std::string shaderLocation);
    


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
    
    VertexArray* m_VAO=nullptr;
    std::string m_ShaderName = "";
    
    

    
    EventLauncher<void(Drawable&,Shader&)> m_PreDrawFuncs;
    EventLauncher<void(Drawable&)> m_PostDrawFuncs;
    EventLauncher<void(Drawable&)> m_DeletedFuncs;

    //static members



    

    


    friend class GuiLayer;
    friend class DrawableEvents;
};