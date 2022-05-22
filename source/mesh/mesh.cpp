#include "mesh.h"
#include "kv.h"



bool Mesh::SetShader(std::string shaderLocation) {
    bool loadResult;
    Shader& shader = Window::GetCurrentWindow().Create().CachedShader(shaderLocation,&loadResult);
    if(loadResult){
        m_ShaderName = shaderLocation;

        for(auto& [name,prop] : shader.GetUniformLocations()){
            if(name[0] != 'p'){ 
                MeshAttribute::ShaderUniformVariable var;
                if(name[1] != 'c'){
                    var.type = prop.type;
                }
                else{
                    var.type = "Color";
                }
                m_PublicShaderVariables[name] = var;
            }
        }


        return true;
    }
    else {
        return false;
    }
}

bool Mesh::ReadyToDraw() {

    if(GetActiveState() && m_ShaderName == ""){
        LOG("Created Object has no shader attached, deactivating...");
        SetActiveState(false);
    }

    bool shaderValid = false;
    Window::GetCurrentWindow().Create().CachedShader(m_ShaderName,&shaderValid);
    return m_ShaderName != "" && shaderValid && GetActiveState();
}

void Mesh::TrySetMesh(std::string path)
{

}



YAML::Node Mesh::Serialize()
{
    YAML::Node node;

    HelperFunctions::SerializeVariable("shader name", m_ShaderName, node);
    

    YAML::Node vertices = node["vertices"];

    HelperFunctions::SerializeVariable("positions", m_Vertices.positions, node);
    HelperFunctions::SerializeVariable("normals", m_Vertices.normals, node);
    HelperFunctions::SerializeVariable("tex coords", m_Vertices.texCoords, node);
    HelperFunctions::SerializeVariable("tangents", m_Vertices.tangents, node);
    HelperFunctions::SerializeVariable("indices", m_Vertices.indices, node);

    

    return node;
}

bool Mesh::Deserialize(YAML::Node& node)
{
    HelperFunctions::DeserializeVariable("shader name", m_ShaderName, node);
    

    YAML::Node vertices = node["vertices"];

    HelperFunctions::DeserializeVariable("positions", m_Vertices.positions, node);
    HelperFunctions::DeserializeVariable("normals", m_Vertices.normals, node);
    HelperFunctions::DeserializeVariable("tex coords", m_Vertices.texCoords, node);
    HelperFunctions::DeserializeVariable("tangents", m_Vertices.tangents, node);
    HelperFunctions::DeserializeVariable("indices", m_Vertices.indices, node);

    SetVertices(m_Vertices);

    return true;
}

VertexArray& Mesh::GetVertexArray() {
    return *m_VAO;
}



Mesh::Mesh() {
    
}

Mesh::~Mesh() {
    
    
}

bool MeshAttribute::Vertex::CheckValid() const {
    return (positions.size() == normals.size()) && (positions.size()/3 == texCoords.size()/2) && (positions.size() == tangents.size()) && (positions.size() != 0);
}

bool Mesh::SetVertices(MeshAttribute::Vertex vertexAttribute) {
    vertexAttribute.SetEqualSize();
    if(!vertexAttribute.CheckValid()){
        return false;
    }
    if (!m_VAO) {
        m_VAO = &Window::GetCurrentWindow().Create().NewVertexArray();
    }
    
    m_Vertices = vertexAttribute;
    GetVertexArray().Bind();
    GetVertexArray().Reset();
    GetVertexArray().Bind();
    GetVertexArray().CreateVertexBuffer((unsigned int)vertexAttribute.positions.size()/3)
        .AddAttribute(m_Vertices.positions,false)
        .AddAttribute(m_Vertices.normals,false)
        .AddAttribute(m_Vertices.texCoords,false)
        .AddAttribute(m_Vertices.tangents,false)
        .Generate();

    if(m_Vertices.indices.size() > 0){
        GetVertexArray().CreateIndexBuffer().SetIndices(m_Vertices.indices);
    }
   
    return true;
}

void Mesh::Draw(const glm::mat4& mvp) {

    GetShader().Bind();
    m_PreDrawFuncs.EmitEvent(*this, GetShader(), mvp);


    m_VAO->Bind();
    if(m_VAO->HasIndexBuffer()){
        GL_CALL(glDrawElements(GL_TRIANGLES,m_VAO->GetDrawCount(),GL_UNSIGNED_INT,nullptr));
    }
    else {
        GL_CALL(glDrawArrays(GL_TRIANGLES,0,m_VAO->GetDrawCount()));
    }

    m_PostDrawFuncs.EmitEvent(*this);
}





FunctionSink<void(Mesh&,Shader&,const glm::mat4&)> Mesh::PreDrawn() {
    return FunctionSink<void(Mesh&,Shader&,const glm::mat4&)>(m_PreDrawFuncs);
}

FunctionSink<void(Mesh&)> Mesh::PostDrawn() {
    return FunctionSink<void(Mesh&)>(m_PostDrawFuncs);
}

void Mesh::Update(float deltaTime) {
    
}

void Mesh::ShowProperties() {
   
    
}



void MeshAttribute::Vertex::SetEqualSize() {
    size_t largestAttribute = positions.size()/3;
    
    if(positions.size() == 0){
        return;
    }
    
    if(normals.size()/3 < largestAttribute){
        size_t oldSize = normals.size();
        normals.resize(normals.size() + largestAttribute*3);
        std::fill(normals.begin() + oldSize,normals.end(),0.0f);
    }
    if(texCoords.size()/2 < largestAttribute){
        size_t oldSize = texCoords.size();
        texCoords.resize(texCoords.size() + largestAttribute*2);
        std::fill(texCoords.begin() + oldSize,texCoords.end(),0.0f);
    }
    if(tangents.size()/3 < largestAttribute){
        size_t oldSize = tangents.size();
        tangents.resize(tangents.size() + largestAttribute*3);
        std::fill(tangents.begin() + oldSize,tangents.end(),0.0f);
    }

}





std::string Mesh::GetShaderName()
{
    return m_ShaderName;
}

Shader& Mesh::GetShader() {
    bool found = false;
    Shader& shader = Window::GetCurrentWindow().Create().CachedShader(m_ShaderName,&found);
    return shader;
}

void Mesh::Init() {
    
    m_VAO = &Window::GetCurrentWindow().Create().NewVertexArray();
    
    
}

void Mesh::Destroy() { 
    m_DeletedFuncs.EmitEvent(*this);
}
