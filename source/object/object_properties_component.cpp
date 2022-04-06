#include "object_properties_component.h"




void ObjectPropertiesComponent::SetActiveState(bool state) {
    active = state;
}

bool ObjectPropertiesComponent::IsActive() {
    return active;
}

void ObjectPropertiesComponent::HandleShowPropertiesFunction(entt::id_type type,std::function<void()> func) {
    m_ShowPropertiesFunctions[type] = func;
}

void ObjectPropertiesComponent::EraseUpdateFunction(entt::id_type type) {
    if(m_UpdateFunctions.find(type) != m_UpdateFunctions.end()){
        m_UpdateFunctions.erase(type);
    }
}

void ObjectPropertiesComponent::EraseShowPropertiesFunction(entt::id_type type) {
    if(m_ShowPropertiesFunctions.find(type) != m_ShowPropertiesFunctions.end()){
        m_ShowPropertiesFunctions.erase(type);
    }
}

void ObjectPropertiesComponent::HandleUpdateFunction(entt::id_type type,std::function<void(float)> func) {
    m_UpdateFunctions[type] = func;
}

void ObjectPropertiesComponent::CallUpdateFunctions(float deltaTime) {
    for(auto& [handle,func] : m_UpdateFunctions){
        func(deltaTime);
    }
}

void ObjectPropertiesComponent::CallShowPropertiesFunctions() {
    int i = 0;
    for(auto& [handle,func] : m_ShowPropertiesFunctions){
        
        ImGui::BeginChild(("ObjectProperty" + std::to_string(i)).c_str(),ImVec2(ImGui::GetWindowSize().x-23,200),true);
        
        func();
        ImGui::EndChild();
        i++;
    }
}

std::string ObjectPropertiesComponent::GetName() {
    return m_Name;
}

void ObjectPropertiesComponent::SetHightlightState(bool state) {
    m_ShouldHighlight = state;
    
}

void ObjectPropertiesComponent::SetHighlightColor(Color color) {
    m_HighlightColor = color;
}
