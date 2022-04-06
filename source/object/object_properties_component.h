#pragma once

#include "../global.h"


class Object;
class ObjectPropertiesComponent {
public:

    void HandleUpdateFunction(entt::id_type type,std::function<void(float)> func);
    void EraseUpdateFunction(entt::id_type type);

    void HandleShowPropertiesFunction(entt::id_type type,std::function<void()> func);
    void EraseShowPropertiesFunction(entt::id_type type);

    bool IsActive();
    void SetActiveState(bool state);

    void CallUpdateFunctions(float deltaTime);
    void CallShowPropertiesFunctions();

private:

    std::map<entt::id_type,std::function<void(float)>> m_UpdateFunctions;
    std::map<entt::id_type,std::function<void()>> m_ShowPropertiesFunctions;
    bool active = true;

    
    friend class Object;
};