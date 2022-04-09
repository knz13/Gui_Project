#pragma once
#include "../global.h"
#include "../opengl_wrappers/vertex_array.h"
#include "drawing_modes.h"
#include "../general/movable.h"
#include "../object/component.h"

class Shader;
class Mesh;

namespace MeshAttribute {

    struct Vertex {
        bool CheckValid();
       
        void SetEqualSize();

        std::vector<float> positions;
        std::vector<float> normals;
        std::vector<float> texCoords;
        std::vector<float> tangents;
        std::vector<unsigned int> indices;
    };
};

class Mesh : public EventReceiver,public Component{

    KV_CLASS

public:
    Mesh(entt::entity e);
   
    ~Mesh();

    bool SetShader(std::string shaderLocation);
    bool SetVertices(MeshAttribute::Vertex vertexAttribute);
    

    void SetDrawingMode(DrawingMode mode);

    FunctionSink<void(Mesh&,Shader&)> PreDrawn();
    FunctionSink<void(Mesh&)> PostDrawn();


    void Update(float deltaTime);
    void ShowProperties();

private:
    VertexArray& GetVertexArray();
    
    void Draw();
    bool ReadyToDraw();
    

    MeshAttribute::Vertex m_Vertices;
    DrawingMode m_DrawingMode;
    
    VertexArray* m_VAO=nullptr;
    std::string m_ShaderName = "";

    
    EventLauncher<void(Mesh&,Shader&)> m_PreDrawFuncs;
    EventLauncher<void(Mesh&)> m_PostDrawFuncs;
    EventLauncher<void(Mesh&)> m_DeletedFuncs;

    //static members



    

    

    
    friend class DrawableEvents;
};