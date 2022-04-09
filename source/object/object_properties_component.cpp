#include "object_properties_component.h"
#include "../kv.h"



void ObjectPropertiesComponent::SetActiveState(bool state) {
    active = state;
}

bool ObjectPropertiesComponent::IsActive() const{
    return active;
}



void ObjectPropertiesComponent::CallUpdateFunctions(float deltaTime) {
    for(auto& [handle,prop] : m_AttachedComponentsProperties){
        prop.m_UpdateFunc(deltaTime);
    }
}

void ObjectPropertiesComponent::CallShowPropertiesFunctions() {

    for(auto& [handle,prop] : m_AttachedComponentsProperties){
        
       
        ImGui::PushStyleColor(ImGuiCol_FrameBg,ImVec4(0.2,0.2,0.2,1));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,ImVec4(0.2,0.2,0.2,1));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered,ImVec4(0,0,0,1));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive,ImVec4(0,0,0,1));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,10);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding,10);
        
        
        
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 6,ImGui::GetCursorPos().y));
        if(!prop.m_IsShowPropertiesChildOpen){
            
            ImGui::BeginChild(GuiLayer::GetImGuiID(&prop.m_ShowPropertiesFunc).c_str(),ImVec2(0,30),true,ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysUseWindowPadding);
        }
        else{
            ImGui::BeginChild(GuiLayer::GetImGuiID(&prop.m_ShowPropertiesFunc).c_str(),ImVec2(0,200),true,ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysUseWindowPadding);
        
        }
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x - 6,ImGui::GetCursorPos().y));

        ImGui::SetNextItemOpen(true,ImGuiCond_FirstUseEver);

        
        prop.m_IsShowPropertiesChildOpen = ImGui::TreeNodeEx((prop.m_ClassName).c_str());
        

        ImGui::SameLine();
        
        
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 10,ImGui::GetCursorPos().y - 2));
        ImGui::Checkbox("##",prop.m_ActiveState);   
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x - 10,ImGui::GetCursorPos().y + 2));
        
        bool passed = false;
        if(prop.m_IsShowPropertiesChildOpen){
            if(!passed){
                ImGui::Spacing();
                prop.m_ShowPropertiesFunc();
                
            
            }
            else {
                prop.m_ShowPropertiesFunc();
            }
            ImGui::TreePop();
            passed = true;
        }
        
        
        ImGui::EndChild();
        
        
        
        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(2);
       
       
    }
}

std::string ObjectPropertiesComponent::GetName() const{
    return m_Name;
}

void ObjectPropertiesComponent::SetHightlightState(bool state) {
    m_ShouldHighlight = state;
    
}

void ObjectPropertiesComponent::SetHighlightColor(Color color) {
    m_HighlightColor = color;
}

bool ObjectPropertiesComponent::ShouldHighlight() const {
    return m_ShouldHighlight;
}

Color ObjectPropertiesComponent::GetHighlightColor() const {
    return m_HighlightColor;
}

void ObjectPropertiesComponent::HandleComponentProperties(entt::id_type type, AttachedComponentProperties prop) {
    m_AttachedComponentsProperties[type] = prop;
}

void ObjectPropertiesComponent::EraseComponentProperties(entt::id_type type) {
    if(m_AttachedComponentsProperties.find(type) != m_AttachedComponentsProperties.end()){
        m_AttachedComponentsProperties.erase(type);
    }
}
