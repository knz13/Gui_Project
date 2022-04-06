#pragma once

#include "../global.h"
#include "object_properties_component.h"

class Object {
public:
    Object(entt::entity ent);
    ~Object();


    template<typename T>
    bool HasComponent(){
        if(!this->Valid()){
            DEBUG_ERROR("Calling HasComponent<" + string(entt::type_id<T>().name()) + ">() with an invalid entity!");
            return false;
        }
        return Registry::Get().any_of<T>(m_EntityHandle);
    };


    template<typename T>
    T& GetComponent(){
        if(!this->HasComponent<T>()){
            T& comp = Registry::Get().emplace<T>(m_EntityHandle);
            

            ObjectPropertiesComponent& properties = Registry::Get().get<ObjectPropertiesComponent>(m_EntityHandle);
            properties.HandleUpdateFunction(entt::type_id<T>().index(),[&](float time){
                comp.Update(time);
            });
            properties.HandleShowPropertiesFunction(entt::type_id<T>().index(),[&](){
                comp.ShowProperties();
            });


            return comp;
        }
        else {
            return Registry::Get().get<T>(m_EntityHandle);
        }
    };





    template<typename T>
    void EraseComponent(){
        if(!this->Valid()){
            DEBUG_ERROR("Calling EraseComponent<" + string(entt::type_id<T>().name()) + ">() with an invalid entity!");
            return;
        }
        if(this->HasComponent<T>()){
            
            ObjectPropertiesComponent& properties = Registry::Get().get<ObjectPropertiesComponent>(m_EntityHandle);
            properties.EraseUpdateFunction(entt::type_id<T>().index());
            properties.EraseShowPropertiesFunction(entt::type_id<T>().index());
            
            Registry::Get().erase<T>(m_EntityHandle);
        }
    };

    bool Valid(){
        return Registry::Get().valid(m_EntityHandle);
    }

    ObjectPropertiesComponent& Properties() {
        return Registry::Get().get<ObjectPropertiesComponent>(m_EntityHandle);
    };

private:
    entt::entity m_EntityHandle;
    

};