#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <concepts>
#include <unordered_map>
#include <map>
#include "../general/color.h"
#include "../vendor/entt/single_include/entt/entt.hpp"
using namespace std;


struct AttachedComponentProperties {
    bool* m_ActiveState = nullptr;
    bool* m_HideInEditor = nullptr;
    bool* m_IsDeletable = nullptr;
    std::string m_ClassName = "";
    std::string m_DisplayName = "";
    bool m_IsShowPropertiesChildOpen = true;
    size_t m_SizeInBytes = 0;
};



class Object;
class ObjectHandle;
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
    
    ObjectHandle GetParent();
    const std::vector<entt::entity>& GetChildren();
    const std::vector<std::string>& GetComponentClassNames();

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

    std::string GetComponentNameByID(entt::id_type type) const;
    
private:
    void HandleComponentProperties(entt::id_type type, AttachedComponentProperties prop);
    void EraseComponentProperties(entt::id_type type);

    std::vector<entt::id_type> m_IDsToDelete;
    std::unordered_map<entt::id_type,AttachedComponentProperties> m_AttachedComponentsProperties;
    std::vector<std::string> m_CurrentComponentNames;

    std::vector<entt::entity> m_Children;
    entt::entity m_Parent = entt::null;
    bool active = true;
    std::string m_Name = "";
    bool m_ShouldHighlight = false;
    entt::entity m_MasterHandle;
    Color m_HighlightColor = Color::Red;
    

    template<typename,typename>
    friend class Component;
    friend class Registry;
    friend class Object;
};