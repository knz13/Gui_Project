#pragma once
#include "../global.h"
#include "../opengl_wrappers/vertex_array.h"
#include "drawing_modes.h"
#include "../general/transform.h"
#include "../object/component.h"

class Shader;
class Mesh;

namespace MeshAttribute {

    struct Vertex {
        bool CheckValid() const;
       
        void SetEqualSize();

        std::vector<float> positions;
        std::vector<float> normals;
        std::vector<float> texCoords;
        std::vector<float> tangents;
        std::vector<unsigned int> indices;
    };

    struct ShaderUniformVariable {
        std::string type;
    };
};

class Mesh : public EventReceiver,public Component<Mesh>{

public:
    Mesh(entt::entity e);
    
    ~Mesh();

    bool SetShader(std::string shaderLocation);
    bool SetVertices(MeshAttribute::Vertex vertexAttribute);
    

    void SetDrawingMode(std::string mode);

    FunctionSink<void(Mesh&,Shader&)> PreDrawn();
    FunctionSink<void(Mesh&)> PostDrawn();


    void Update(float deltaTime);
    void ShowProperties();

    Mesh& operator=(const Mesh& other);
    bool ReadyToDraw();
private:
    VertexArray& GetVertexArray();
    
    void Draw();
    
    Color myColor = Color::White;
    MeshAttribute::Vertex m_Vertices;
    std::shared_ptr<DrawingMode> m_DrawingMode;
    std::map<std::string,MeshAttribute::ShaderUniformVariable> m_PublicShaderVariables;
    int m_DrawingModeComboItem = 0;
    VertexArray* m_VAO=nullptr;
    std::string m_ShaderName = "";

    
    EventLauncher<void(Mesh&,Shader&)> m_PreDrawFuncs;
    EventLauncher<void(Mesh&)> m_PostDrawFuncs;
    EventLauncher<void(Mesh&)> m_DeletedFuncs;

    //static members



    

    

    friend class Window;
    friend class DrawableEvents;
};