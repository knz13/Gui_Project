#include "registry.h"
#include "object.h"
#include "object_properties_component.h"
#include "../kv.h"
#include <ctime>

reactphysics3d::PhysicsCommon Registry::m_PhysicsManager;
entt::registry Registry::m_Registry;
std::mt19937 Registry::m_RandomGenerator(time(nullptr));
entt::registry& Registry::Get() {
    return Registry::m_Registry;
}

Object Registry::CreateObject(std::string name) {
    entt::entity ent = m_Registry.create();

    m_Registry.emplace<ObjectPropertiesComponent>(ent,name,ent);


    Object obj(ent);


    for(auto& className : Object::m_ClassesToAddEveryTime){
        obj.TryAddComponent(className);
    }

    return obj;

}

void Registry::DeleteObject(Object obj) {
    m_ObjectsToDelete.push_back(obj);
}

reactphysics3d::PhysicsCommon& Registry::GetPhysicsCommon() {
    return m_PhysicsManager; 
}

size_t Registry::GenerateRandomNumber() {
    return m_RandomGenerator();
}



Object Registry::CopyEntity(Object other) {
    entt::entity firstObject = entt::null;
    entt::entity lastObject = entt::null;
    
    other.Properties().ApplyFuncToSelfAndChildren([&](Object current){
        Object newOne = Registry::DuplicateObject(current);
        if(firstObject == entt::null){
            firstObject = newOne.ID();
        }
        if(lastObject != entt::null){
            newOne.Properties().SetParent(Object(lastObject));
        }
        lastObject = newOne.ID();
    });
    return Object(firstObject);
}

Object Registry::DuplicateObject(Object other) {
    Object obj = Registry::CreateObject(other.Properties().GetName());

    for(auto [id,storage] : m_Registry.storage()){
        if(id == entt::type_hash<ObjectPropertiesComponent>().value()){
            continue;
        }
        if(storage.contains(other.ID()) && !storage.contains(obj.ID())){
                void* oldData = storage.get(other.ID());
                obj.TryCopyComponent(other.Properties().GetComponentByName(id),other);
        }
        else if(storage.contains(obj.ID())){
            
            obj.TryCopyComponent(obj.Properties().GetComponentByName(id),other);
        }
    }

    return obj;
}

void Registry::UpdateState() {
    for(auto& obj : m_ObjectsToDelete){
        if(obj.Valid()){
            auto it = obj.Properties().m_AttachedComponentsProperties.begin();
            while(it != obj.Properties().m_AttachedComponentsProperties.end()){
                obj.EraseComponent(it->second.m_ClassName);
                it = obj.Properties().m_AttachedComponentsProperties.begin();
            }
            m_Registry.destroy(obj.ID());
        }
    }
    m_ObjectsToDelete.clear();
}
