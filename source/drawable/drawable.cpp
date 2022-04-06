#include "drawable.h"
#include "kv.h"


std::unordered_map<unsigned int,Drawable*> Drawable::m_DrawableObjects;


bool Drawable::SetShader(std::string shaderLocation) {
    bool loadResult;
    Shader& shader = m_CurrentWindow->Create().CachedShader(shaderLocation,&loadResult);
    if(loadResult){
        m_ShaderName = shaderLocation;
        return true;
    }
    else {
        return false;
    }
}

bool Drawable::ReadyToDraw() {

    if(m_Active && m_ShaderName == ""){
        LOG("Created Object has no shader attached, deactivating...");
        m_Active = false;
    }

    bool shaderValid = false;
    m_CurrentWindow->Create().CachedShader(m_ShaderName,&shaderValid);
    return m_ShaderName != "" && shaderValid && m_Active;
}



VertexArray& Drawable::GetVertexArray() {
    return *m_VAO;
}

void Drawable::SetActive(Window& win) {
    if(m_CurrentWindow != nullptr){
        m_CurrentWindow->RemoveFromDrawingQueue(m_ID);
        m_CurrentWindow = &win;
    }
    else{
        m_CurrentWindow = &win;
        m_VAO = &m_CurrentWindow->Create().NewVertexArray();
    }

    m_CurrentWindow->AddToDrawingQueue(m_ID);
    m_Active = true;
}

void Drawable::SetActive(){
    if(m_CurrentWindow != nullptr){
        m_CurrentWindow->AddToDrawingQueue(m_ID);
    }

    m_Active= true;
}

void Drawable::SetInactive() {
    if(m_CurrentWindow != nullptr){
        m_CurrentWindow->RemoveFromDrawingQueue(m_ID);
    }
    
    m_Active = false;
}

Drawable::Drawable(Window* win){
    if(win){
        m_ID = Drawable::CreateDrawableHandle(*this);
        this->SetActive(*win);
    }

}

Drawable::~Drawable() {

    m_DeletedFuncs.EmitEvent(*this);
    SetInactive();
    Drawable::DestroyDrawableHandle(m_ID);

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







void Drawable::DestroyDrawableHandle(unsigned int id) {
    if(m_DrawableObjects.find(id) != m_DrawableObjects.end()){
        m_DrawableObjects.erase(id);
    }
    else{
        DEBUG_WARN("Trying to delete object with id " + std::to_string(id) + " without it being in the drawable objects map.");
    }
}

unsigned int Drawable::CreateDrawableHandle(Drawable& dr) {
    static unsigned int id = 0;

    id++;

    m_DrawableObjects[id] = &dr;

    return id;
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
