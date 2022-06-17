#pragma once
#include "../global.h"
#include "../opengl_wrappers/vertex_array.h"
#include "../general/transform.h"

class Shader;
class Mesh;

namespace DrawingModeHelpers {
    class Null;

};

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

    struct LoadResult {
       

    };
};

class Mesh : public yael::event_receiver,public ecspp::DefineComponent<Mesh, GameComponent> {
public:
    Mesh();
    
    ~Mesh();


    bool SetShader(std::string shaderLocation);
    bool SetVertices(MeshAttribute::Vertex vertexAttribute);
    
    std::string GetShaderName();
    ecspp::ObjectHandle GetShader();
    
    yael::event_sink<void(Mesh&,Shader&,const glm::mat4&)> PreDrawn();
    yael::event_sink<void(Mesh&)> PostDrawn();


    void Update(float deltaTime) override;

   
    

    bool ReadyToDraw();

    bool TrySetMesh(std::string path);
    void Draw(const glm::mat4& mvp);

private:
    

    VertexArray& GetVertexArray();
    void Init() override;
    void Destroy() override;

    
    
    MeshAttribute::Vertex m_Vertices;
    std::map<std::string,MeshAttribute::ShaderUniformVariable> m_PublicShaderVariables;
    
    VertexArray* m_VAO=nullptr;
    std::string m_ShaderName = "";
    
    
    yael::event_launcher<void(Mesh&,Shader&,const glm::mat4&)> m_PreDrawFuncs;
    yael::event_launcher<void(Mesh&)> m_PostDrawFuncs;
    yael::event_launcher<void(Mesh&)> m_DeletedFuncs;

    //static members



    

    

    friend class Window;
    friend class DrawableEvents;
};