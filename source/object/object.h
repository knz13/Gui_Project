#pragma once

#include "../global.h"
#include "object_properties_component.h"



class Movable;
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

    bool HasComponent(entt::id_type type) {
        return Properties().m_AttachedComponentsProperties.find(type) != Properties().m_AttachedComponentsProperties.end();
    }

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

            comp.m_MyClassTypeID = entt::type_hash<T>().value();
            prop.m_ClassName = std::string(entt::type_id<T>().name()).substr(6);
            prop.m_SizeInBytes = sizeof(T);
            prop.m_ActiveState = &comp.m_BaseComponentActiveState;
            ObjectPropertiesComponent& properties = Registry::Get().get<ObjectPropertiesComponent>(m_EntityHandle);
            properties.HandleComponentProperties(entt::type_hash<T>().value(),prop);


            return comp;
        }
        else {
            return Registry::Get().get<T>(m_EntityHandle);
        }
    };

    Movable& Transform();
    

    bool TryCopyComponent(std::string stringToHash,Object from){
        auto resolved = entt::resolve(entt::hashed_string(stringToHash.c_str()));
        
        if(resolved){
            entt::meta_any component = resolved.construct(*this);
            if(auto func = resolved.func(entt::hashed_string("Copy Component")) ; func){
                return func.invoke({},from,*this).operator bool();
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
    };

    bool TryAddComponent(std::string stringToHash){

        auto resolved = entt::resolve(entt::hashed_string(stringToHash.c_str()));
        if(resolved){
            entt::meta_any owner = resolved.construct(*this);
            return owner.operator bool();
        }
        else{
            return false;
        }
    }

    template<typename T,typename ...Args>
    T& AddComponent(Args&&... args){
        if(!this->HasComponent<T>()){
            T& comp = Registry::Get().emplace<T>(m_EntityHandle,args...,m_EntityHandle);
            

            AttachedComponentProperties prop;
            prop.m_UpdateFunc = [&](float time){
                comp.Update(time);
            };

            prop.m_ShowPropertiesFunc = [&](){
                comp.ShowProperties();
            };

            comp.m_MyClassTypeID = entt::type_hash<T>().value();
            prop.m_ClassName = std::string(entt::type_id<T>().name()).substr(6);
            prop.m_SizeInBytes = sizeof(T);
            prop.m_ActiveState = &comp.m_BaseComponentActiveState;
            ObjectPropertiesComponent& properties = Properties();
            properties.HandleComponentProperties(entt::type_hash<T>().value(),prop);

            return comp;
        }
        else {
            return Registry::Get().get<T>(m_EntityHandle);
        }
    }

    const AttachedComponentProperties* GetComponentData(entt::id_type type){
        if(!HasComponent(type)){
            return nullptr;
        }

        return &Properties().m_AttachedComponentsProperties[type];
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

    template<typename T>
    static bool CopyComponent(Object from,Object to){
        if(from.HasComponent<T>() && to.HasComponent<T>()){
            T& first = from.GetComponent<T>();
            to.GetComponent<T>() = first;
            return true;
        }
        else{
            return false;
        }
    };

    template<typename T>
    static void RegisterClassAsComponent(){
        std::string name = string(entt::type_id<T>().name()).substr(6);
        entt::meta<T>().type(entt::hashed_string(name.c_str())).ctor<&Object::GetComponent<T>,entt::as_ref_t>();
        entt::meta<T>().type(entt::hashed_string(name.c_str())).func<&Object::CopyComponent<T>>(entt::hashed_string("Copy Component"));
    };

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