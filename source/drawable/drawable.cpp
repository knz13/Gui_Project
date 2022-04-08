#include "drawable.h"
#include "kv.h"



bool Drawable::SetShader(std::string shaderLocation) {
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

bool Drawable::ReadyToDraw() {

    if(GetActiveState() && m_ShaderName == ""){
        LOG("Created Object has no shader attached, deactivating...");
        SetActiveState(false);
    }

    bool shaderValid = false;
    Window::GetCurrentWindow().Create().CachedShader(m_ShaderName,&shaderValid);
    return m_ShaderName != "" && shaderValid && GetActiveState();
}



VertexArray& Drawable::GetVertexArray() {
    return *m_VAO;
}




Drawable::Drawable(entt::entity master) : Component(master) {
    m_VAO = &Window::GetCurrentWindow().Create().NewVertexArray();
}

Drawable::~Drawable() {
    m_DeletedFuncs.EmitEvent(*this);

}


void Drawable::Draw() {
    m_VAO->Bind();
    if(m_VAO->HasIndexBuffer()){
        GL_CALL(glDrawElements(m_DrawingMode.GetDrawingType(),m_VAO->GetDrawCount(),GL_UNSIGNED_INT,nullptr));
    }
    else {
        GL_CALL(glDrawArrays(m_DrawingMode.GetDrawingType(),0,m_VAO->GetDrawCount()));
    }
}



void Drawable::SetDrawingMode(DrawingMode mode) {
    m_DrawingMode = mode;
}













FunctionSink<void(Drawable&,Shader&)> Drawable::PreDrawn() {
    return FunctionSink<void(Drawable&,Shader&)>(m_PreDrawFuncs);
}

FunctionSink<void(Drawable&)> Drawable::PostDrawn() {
    return FunctionSink<void(Drawable&)>(m_PostDrawFuncs);
}

void Drawable::Update(float deltaTime) {
    
}

void Drawable::ShowProperties() {
    
}


