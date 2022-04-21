#pragma once
#include "component.h"

template<typename T,typename... Behaviors>
class DeriveFromComponent :public Component,public Behaviors... {
public:
    DeriveFromComponent() {
        (void*)Initialized;
    }


private:
    static inline bool Initialized = []() {
        if (entt::type_id<T>().name() != entt::type_id<ComponentHelpers::Null>().name()) {
            Object::RegisterClassAsComponent<T>();
        }
        return true;
    }();

};