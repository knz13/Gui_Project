#pragma once
#include "../global.h"
#include "../opengl_wrappers/vertex_array.h"
#include "drawing_modes.h"
#include "../general/movable.h"
#include "../object/component.h"

class Shader;
class Mesh;


class Mesh : public EventReceiver,public Component{

    KV_CLASS

public:
    Mesh(entt::entity e);
   
    ~Mesh();

    bool SetShader(std::string shaderLocation);
    


    void SetDrawingMode(DrawingMode mode);

    FunctionSink<void(Mesh&,Shader&)> PreDrawn();
    FunctionSink<void(Mesh&)> PostDrawn();

    VertexArray& GetVertexArray();

    void Update(float deltaTime);
    void ShowProperties();

private:
    
    void Draw();
    bool ReadyToDraw();
    

    DrawingMode m_DrawingMode;
    
    VertexArray* m_VAO=nullptr;
    std::string m_ShaderName = "";

    
    EventLauncher<void(Mesh&,Shader&)> m_PreDrawFuncs;
    EventLauncher<void(Mesh&)> m_PostDrawFuncs;
    EventLauncher<void(Mesh&)> m_DeletedFuncs;

    //static members



    

    


    friend class DrawableEvents;
};