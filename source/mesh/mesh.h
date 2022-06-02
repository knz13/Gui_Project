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

class Mesh : public EventReceiver,public GameComponent<Mesh>{
    KV_CLASS
public:
    Mesh();
    
    ~Mesh();



    bool SetShader(std::string shaderLocation);
    bool SetVertices(MeshAttribute::Vertex vertexAttribute);
    
    std::string GetShaderName();
    Shader& GetShader();
    

    FunctionSink<void(Mesh&,Shader&,const glm::mat4&)> PreDrawn();
    FunctionSink<void(Mesh&)> PostDrawn();


    void Update(float deltaTime) override;
    void ShowProperties() override;

   
    

    bool ReadyToDraw();

    void TrySetMesh(std::string path);
    void Draw(const glm::mat4& mvp);

private:
    YAML::Node Serialize() override;
    bool Deserialize(YAML::Node& node) override;

    VertexArray& GetVertexArray();
    void Init() override;
    void Destroy() override;

    
    
    MeshAttribute::Vertex m_Vertices;
    std::map<std::string,MeshAttribute::ShaderUniformVariable> m_PublicShaderVariables;
    
    VertexArray* m_VAO=nullptr;
    std::string m_ShaderName = "";
    
    
    EventLauncher<void(Mesh&,Shader&,const glm::mat4&)> m_PreDrawFuncs;
    EventLauncher<void(Mesh&)> m_PostDrawFuncs;
    EventLauncher<void(Mesh&)> m_DeletedFuncs;

    //static members



    

    

    friend class Window;
    friend class DrawableEvents;
};