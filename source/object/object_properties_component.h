#pragma once

#include "../global.h"


class Object;
class ObjectPropertiesComponent {
public:

    ObjectPropertiesComponent(std::string name){
        this->m_Name = name;
    };

    void HandleUpdateFunction(entt::id_type type,std::function<void(float)> func);
    void EraseUpdateFunction(entt::id_type type);

    void HandleShowPropertiesFunction(entt::id_type type,std::function<void()> func);
    void EraseShowPropertiesFunction(entt::id_type type);

    void SetHightlightState(bool state);
    void SetHighlightColor(Color color);

    bool IsActive() const;
    void SetActiveState(bool state);

    void CallUpdateFunctions(float deltaTime);
    void CallShowPropertiesFunctions();


    Color GetHighlightColor() const;
    bool ShouldHighlight() const;
    std::string GetName() const;

private:

    std::map<entt::id_type,std::function<void(float)>> m_UpdateFunctions;
    std::map<entt::id_type,std::function<void()>> m_ShowPropertiesFunctions;
    bool active = true;
    std::string m_Name = "";
    bool m_ShouldHighlight = false;
    Color m_HighlightColor = Color::Red;

    friend class Object;
};