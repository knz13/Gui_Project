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
            T& comp = Registry::Get().emplace<T>(m_EntityHandle,m_EntityHandle);
            
            AttachedComponentProperties prop;
            prop.m_UpdateFunc = [&](float time){
                comp.Update(time);
            };

            prop.m_ShowPropertiesFunc = [&](){
                comp.ShowProperties();
            };

            comp.m_MyClassTypeID = entt::type_id<T>().index();
            prop.m_ClassName = entt::type_id<T>().name();
            prop.m_ActiveState = &comp.m_BaseComponentActiveState;
            ObjectPropertiesComponent& properties = Registry::Get().get<ObjectPropertiesComponent>(m_EntityHandle);
            properties.HandleComponentProperties(entt::type_id<T>().index(),prop);


            return comp;
        }
        else {
            return Registry::Get().get<T>(m_EntityHandle);
        }
    };

    template<typename T,typename ...Args>
    T& GetComponent(Args&&... args){
        if(!this->HasComponent<T>()){
            T& comp = Registry::Get().emplace<T>(m_EntityHandle,args...,m_EntityHandle);

            AttachedComponentProperties prop;
            prop.m_UpdateFunc = [&](float time){
                comp.Update(time);
            };

            prop.m_ShowPropertiesFunc = [&](){
                comp.ShowProperties();
            };

            comp.m_MyClassTypeID = entt::type_id<T>().index();
            prop.m_ClassName = entt::type_id<T>().name();
            prop.m_ActiveState = &comp.m_BaseComponentActiveState;
            ObjectPropertiesComponent& properties = Properties();
            properties.HandleComponentProperties(entt::type_id<T>().index(),prop);

            return comp;
        }
        else {
            return Registry::Get().get<T>(m_EntityHandle);
        }
    }





    template<typename T>
    void EraseComponent(){
        if(!this->Valid()){
            DEBUG_ERROR("Calling EraseComponent<" + string(entt::type_id<T>().name()) + ">() with an invalid entity!");
            return;
        }
        if(this->HasComponent<T>()){
            
            ObjectPropertiesComponent& properties = Properties();
            properties.EraseComponentProperties(entt::type_id<T>().index());
            
            Registry::Get().erase<T>(m_EntityHandle);
        }
    };

    template<typename T>
    void DisableComponent() {
        if(!this->Valid()){
            return;
        }

        if(this->HasComponent<T>()){

            ObjectPropertiesComponent& prop = Properties();


        }

    };

    bool Valid(){
        return Registry::Get().valid(m_EntityHandle);
    }

    ObjectPropertiesComponent& Properties() {
        return Registry::Get().get<ObjectPropertiesComponent>(m_EntityHandle);
    };

    entt::entity ID(){
        return m_EntityHandle;
    }

private:
    entt::entity m_EntityHandle;
    

};


class ObjectHandle {
public:
    ObjectHandle(entt::entity ent){
        m_Handle = ent;
    };

    Object GetAsObject(){
        return Object(m_Handle);
    };

    operator bool(){
        return Registry::Get().valid(m_Handle);
    };



private:
    entt::entity m_Handle = entt::null;

    
};