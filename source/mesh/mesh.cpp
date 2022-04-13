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



VertexArray& Mesh::GetVertexArray() {
    return *m_VAO;
}



Mesh::Mesh() {
    
}

Mesh::~Mesh() {
    m_DeletedFuncs.EmitEvent(*this);
    
}

bool MeshAttribute::Vertex::CheckValid() const {
    return (positions.size() == normals.size()) && (positions.size()/3 == texCoords.size()/2) && (positions.size() == tangents.size()) && (positions.size() != 0);
}

bool Mesh::SetVertices(MeshAttribute::Vertex vertexAttribute) {
    vertexAttribute.SetEqualSize();
    if(!vertexAttribute.CheckValid()){
        return false;
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

void Mesh::Draw() {
    m_VAO->Bind();
    if(m_VAO->HasIndexBuffer()){
        GL_CALL(glDrawElements(m_DrawingMode.GetDrawingType(),m_VAO->GetDrawCount(),GL_UNSIGNED_INT,nullptr));
    }
    else {
        GL_CALL(glDrawArrays(m_DrawingMode.GetDrawingType(),0,m_VAO->GetDrawCount()));
    }
}



void Mesh::SetDrawingMode(std::string mode) {
    if(mode == "Triangles"){
        
        DrawingModeType::Triangles(m_DrawingMode);
    }
    if(mode == "Lines"){
        DrawingModeType::Lines(m_DrawingMode);
    }
    if(mode == "Points"){
        DrawingModeType::Points(m_DrawingMode);
    }
}




FunctionSink<void(Mesh&,Shader&)> Mesh::PreDrawn() {
    return FunctionSink<void(Mesh&,Shader&)>(m_PreDrawFuncs);
}

FunctionSink<void(Mesh&)> Mesh::PostDrawn() {
    return FunctionSink<void(Mesh&)>(m_PostDrawFuncs);
}

void Mesh::Update(float deltaTime) {
    
}

void Mesh::ShowProperties() {
    ImGui::BulletText("Mode:");
    ImGui::SameLine();

    std::vector<const char*> items = {"Triangles","Lines","Points"};
    const char* currentItem = items[m_DrawingModeComboItem];

    ImGui::PushItemWidth(120);
    if(ImGui::BeginCombo((GuiLayer::GetImGuiID(this) + "Combo").c_str(),currentItem)){
        int index = 0;
        for(auto& item : items){
            const bool isSelected = (m_DrawingModeComboItem == index);
            if(ImGui::Selectable(item,isSelected)){
                SetDrawingMode(item);
                m_DrawingModeComboItem = index;
            }

            if(isSelected){
                ImGui::SetItemDefaultFocus();
            }
            index++;
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();

    m_DrawingMode.m_ShowPropertiesFunc();
    
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

Mesh& Mesh::operator=(const Mesh& other) {
    Component::operator=(other);
    m_VAO = &Window::GetCurrentWindow().Create().NewVertexArray();

    if(other.m_Vertices.CheckValid()){
        m_Vertices = other.m_Vertices;
        this->SetVertices(m_Vertices);
    }

    m_DrawingMode = other.m_DrawingMode;
    m_ShaderName = other.m_ShaderName;
    return *this;
}

Shader& Mesh::GetShader() {
    bool found = false;
    Shader& shader = Window::GetCurrentWindow().Create().CachedShader(m_ShaderName,&found);
    return shader;
}

void Mesh::Init() {
    m_DrawingMode.SetMaster(this->GetMasterObject());
    m_VAO = &Window::GetCurrentWindow().Create().NewVertexArray();
    SetDrawingMode("Triangles");
    
    this->PreDrawn().Connect([&](Mesh& mesh, Shader& sh){
        sh.SetUniform3f("MyColor",myColor.Normalized().x,myColor.Normalized().y,myColor.Normalized().z);
    });
}

void Mesh::Destroy() {
    m_DrawingMode.m_DeleteFunc();    
}
