#pragma once

#include "../global.h"



struct AttachedComponentProperties {
    std::function<void(float)> m_UpdateFunc;
    std::function<void()> m_ShowPropertiesFunc;
    bool* m_ActiveState = nullptr;
    std::string m_ClassName = "";
    bool m_IsShowPropertiesChildOpen = true;
    size_t m_SizeInBytes = 0;
};



class Object;
class ObjectPropertiesComponent {
public:

    ObjectPropertiesComponent(std::string name,entt::entity e) : m_MasterHandle(e){
        this->m_Name = name;
    };


    void Copy(const ObjectPropertiesComponent& other);

    void SetHightlightState(bool state);
    void SetHighlightColor(Color color);

    bool IsActive() const;
    void SetActiveState(bool state);

    void CallUpdateFunctions(float deltaTime);
    void CallShowPropertiesFunctions();
    
    ReturnedObjectProperties GetParent();
    const std::vector<entt::entity>& GetChildren();
    void SetParent(Object e);
    void ClearParent();
    bool IsInChildren(Object obj);
    void RemoveChildren(Object e);
    void AddChildren(Object e);
    void ApplyFuncToSelfAndChildren(std::function<void(Object)> func);

    Color GetHighlightColor() const;
    bool ShouldHighlight() const;
    std::string GetName() const;
    void SetName(std::string name);

    std::string GetComponentByName(entt::id_type type) const;

private:
    void HandleComponentProperties(entt::id_type type, AttachedComponentProperties prop);
    void EraseComponentProperties(entt::id_type type);

    std::unordered_map<entt::id_type,AttachedComponentProperties> m_AttachedComponentsProperties;
    
    std::vector<entt::entity> m_Children;
    entt::entity m_Parent = entt::null;
    bool active = true;
    std::string m_Name = "";
    bool m_ShouldHighlight = false;
    entt::entity m_MasterHandle;
    Color m_HighlightColor = Color::Red;
    

    friend class Component;
    friend class Object;
};