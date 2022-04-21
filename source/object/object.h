#pragma once
#include "registry.h"
#include "object_properties.h"
#include "object_property_register.h"



namespace ComponentHelpers {
    class Null;
}

class Component;
class TransformComponent;
class Object {
public:
    Object(entt::entity ent);
    ~Object();


    template<typename T>
    bool HasComponent(){
        if(!this->Valid()){
            return false;
        }
        return Registry::Get().all_of<T>(m_EntityHandle);
    };

    bool HasComponent(entt::id_type type) {
        return Properties().m_ComponentClassNamesByType.find(type) != Properties().m_ComponentClassNamesByType.end();
    }

    bool HasComponent(std::string type);

    

    template<typename T>
    T& GetComponent(){
        if(!this->HasComponent<T>()){
            T& comp = Registry::Get().emplace<T>(m_EntityHandle);
            comp.SetMaster(m_EntityHandle);
            comp.Init();
            
            Properties().AddComponent(entt::type_hash<T>().value(), HelperFunctions::GetClassName<T>());

            return comp;
        }
        else {
            return Registry::Get().get<T>(m_EntityHandle);
        }
    };

    TransformComponent& Transform();
    

    bool CopyComponentByName(std::string stringToHash,Object from){
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

    Component* AddComponentByName(std::string stringToHash);

    Component* GetComponentByName(std::string stringToHash);

    std::string GetComponentNameByID(entt::id_type id);

    

    template<typename T,typename ...Args>
    T& AddComponent(Args&&... args){
        if(!this->HasComponent<T>()){
            T& comp = Registry::Get().emplace<T>(m_EntityHandle,args...);
            comp.SetMaster(m_EntityHandle);
            comp.Init();
            

            Properties().AddComponent(entt::type_hash<T>().value(), HelperFunctions::GetClassName<T>());

            return comp;
        }
        else {
            return Registry::Get().get<T>(m_EntityHandle);
        }
    }

  




    template<typename T>
    void EraseComponent(){
        if(!this->Valid()){
            return;
        }
        if(this->HasComponent<T>()){
            
            T* comp = &GetComponent<T>();
            comp->Destroy();

            
            Properties().EraseComponent(entt::type_hash<T>().value());
            
            Registry::Get().storage<T>().erase(m_EntityHandle);
        }
    };

    bool EraseComponentByName(std::string componentName){
        auto resolved = entt::resolve(entt::hashed_string(componentName.c_str()));
        
        if(resolved){
            if(auto func = resolved.func(entt::hashed_string("Erase Component")) ; func){
                return func.invoke({},*this).operator bool();
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
    }

    template<typename T>
    void EnableComponent() {
        if(!this->Valid()){
            return;
        }

        if(this->HasComponent<T>()){
            this->GetComponent<T>().SetActiveState(true);
        }
    }

    template<typename T>
    void DisableComponent() {
        if(!this->Valid()){
            return;
        }

        if(this->HasComponent<T>()){
            this->GetComponent<T>().SetActiveState(false);
        }

    };

    bool Valid(){
        return Registry::Get().valid(m_EntityHandle);
    }

    
    
    ObjectProperties& Properties() {
        return Registry::Get().get<ObjectProperties>(m_EntityHandle);
    };

    const entt::entity& ID(){
        return m_EntityHandle;
    }

    template<typename T>
    static bool CopyComponent(Object from,Object to){
        if(from.HasComponent<T>() && to.HasComponent<T>()){
            T& first = from.GetComponent<T>();
            T& second = to.GetComponent<T>();
            second = first;
            return true;
        }
        else{
            return false;
        }
    };

    
    

    static const std::vector<std::string>& GetRegisteredComponents();

protected:
    virtual void Init() {};
    virtual void Destroy() {};

    template<typename T>
    T& GetPropertyStorage() {
        return Registry::Get().get_or_emplace<T>(m_EntityHandle);
    }
    
private:

    template<typename T>
    static entt::id_type RegisterClassAsComponent() {
        entt::id_type hash = HelperFunctions::HashClassName<T>();
        entt::meta<T>().type(hash).template ctor<&Object::GetComponent<T>, entt::as_ref_t>();
        entt::meta<T>().type(hash).template func<&Object::CopyComponent<T>>(entt::hashed_string("Copy Component"));
        entt::meta<T>().type(hash).template func<&Object::EraseComponent<T>>(entt::hashed_string("Erase Component"));
        entt::meta<T>().type(hash).template func<&Object::HasComponent<T>>(entt::hashed_string("Has Component"));
        entt::meta<T>().type(hash).template func<&HelperFunctions::GetClassDisplayName<T>>(entt::hashed_string("Get Display Name"));
        return hash;
    }
    
    inline static std::vector<std::string> m_RegisteredComponents;

    entt::entity m_EntityHandle;
    
    friend class ObjectPropertyRegister;

    friend class Registry;

    template<typename,typename>
    friend class ObjectPropertyStorage;

    template<typename,typename...>
    friend class DeriveFromComponent;

    friend class Component;
};

