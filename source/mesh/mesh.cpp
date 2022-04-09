#include "mesh.h"
#include "kv.h"



bool Mesh::SetShader(std::string shaderLocation) {
    bool loadResult;
    Shader& shader = Window::GetCurrentWindow().Create().CachedShader(shaderLocation,&loadResult);
    if(loadResult){
        m_ShaderName = shaderLocation;
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




Mesh::Mesh(entt::entity e) : Component(e) {
    m_VAO = &Window::GetCurrentWindow().Create().NewVertexArray();
}

Mesh::~Mesh() {
    m_DeletedFuncs.EmitEvent(*this);

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



void Mesh::SetDrawingMode(DrawingMode mode) {
    m_DrawingMode = mode;
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
    
}


