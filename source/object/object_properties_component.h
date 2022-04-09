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

    ObjectPropertiesComponent(std::string name){
        this->m_Name = name;
    };


    void Copy(const ObjectPropertiesComponent& other);

    void SetHightlightState(bool state);
    void SetHighlightColor(Color color);

    bool IsActive() const;
    void SetActiveState(bool state);

    void CallUpdateFunctions(float deltaTime);
    void CallShowPropertiesFunctions();



    Color GetHighlightColor() const;
    bool ShouldHighlight() const;
    std::string GetName() const;
    std::string GetComponentByName(entt::id_type type) const;

private:
    void HandleComponentProperties(entt::id_type type, AttachedComponentProperties prop);
    void EraseComponentProperties(entt::id_type type);

    std::unordered_map<entt::id_type,AttachedComponentProperties> m_AttachedComponentsProperties;
    
    bool active = true;
    std::string m_Name = "";
    bool m_ShouldHighlight = false;
    Color m_HighlightColor = Color::Red;

    friend class Component;
    friend class Object;
};