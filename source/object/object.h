#pragma once
#include "registry.h"
#include "object_properties.h"
#include "object_property_register.h"





class Component;
class TransformComponent;
class Object {
public:
    Object(entt::entity ent);
    ~Object();


    template<typename T>
    bool HasComponent() {
        return ObjectPropertyRegister::HasComponent<T>(m_EntityHandle);
    }
    bool HasComponent(entt::id_type type) {
        return Properties().m_ComponentClassNamesByType.find(type) != Properties().m_ComponentClassNamesByType.end();
    }

    bool HasComponent(std::string type);

    template<typename T>
    T& GetComponent() {
        NamedComponentHandle<T> handle(nullptr);
        try {
            handle = ObjectPropertyRegister::GetComponent<T>(m_EntityHandle);
        }
        catch (std::runtime_error& err) {
            throw err;
        }
        if (handle) {
            Properties().AddComponent(entt::type_hash<T>().value(), HelperFunctions::GetClassName<T>());
        }
        return *handle.GetComponentPointer();
    }


    

    bool CopyComponentByName(std::string stringToHash,Object from){
        auto resolved = entt::resolve(entt::hashed_string(stringToHash.c_str()));
        
        if(resolved){
            entt::meta_any component = resolved.construct(*this);
            if(auto func = resolved.func(entt::hashed_string("Copy Component")) ; func){
                return func.invoke({},from.ID(), this->ID()).operator bool();
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
    };

    

    std::string GetComponentNameByID(entt::id_type id);

    

    template<typename T,typename ...Args>
    T& AddComponent(Args&&... args){
        NamedComponentHandle<T> handle(nullptr);
        try {
            handle = ObjectPropertyRegister::GetComponent<T, Args...>(m_EntityHandle, std::forward<Args>(args)...);
        }
        catch (std::runtime_error& err) {
            throw err;
        }
        if (handle) {
            Properties().AddComponent(entt::type_hash<T>().value(), HelperFunctions::GetClassName<T>());
        }
        return *handle.GetComponentPointer();
    }

  




    template<typename T>
    void EraseComponent(){
        if (ObjectPropertyRegister::EraseComponent<T>(m_EntityHandle)) {
            Properties().EraseComponent(entt::type_hash<T>().value());

            Registry::Get().storage<T>().erase(m_EntityHandle);
        }
        
    };

    bool EraseComponentByName(std::string componentName){
        auto resolved = entt::resolve(entt::hashed_string(componentName.c_str()));
        
        if(resolved){
            if(auto func = resolved.func(entt::hashed_string("Erase Component")) ; func){
                return func.invoke({},m_EntityHandle).operator bool();
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
        return ObjectPropertyRegister::CopyComponent<T>(from.ID(), to.ID());
    };

    bool HasSameObjectTypeAs(Object other);
    

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
    friend class GameComponent;

    template<typename,typename>
    friend class ComponentSpecifier;

    friend class Component;
};

