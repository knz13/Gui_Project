#include "registry.h"
#include "object.h"
#include "object_properties_component.h"
#include "../kv.h"
#include <ctime>

std::vector<Object> Registry::m_ObjectsToDelete;
reactphysics3d::PhysicsCommon Registry::m_PhysicsManager;
entt::registry Registry::m_Registry;
std::mt19937 Registry::m_RandomGenerator(time(nullptr));
entt::registry& Registry::Get() {
    return Registry::m_Registry;
}

Object Registry::CreateObject(std::string name) {
    entt::entity ent = m_Registry.create();

    int index = 1;
    if(Registry::FindObjectByName(name)){
        if(name.find_last_of(")") == std::string::npos || (name.find_last_of(")") != name.size() - 1)){
            name += "(" + std::to_string(index) + ")";
        }
    }

    while(Registry::FindObjectByName(name)){
        index++;
        name.replace(name.find_last_of("(")+1,std::to_string(index-1).size(),std::to_string(index));
    }

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
                obj.TryAddComponent(other.Properties().GetComponentNameByID(id));
                obj.TryCopyComponent(other.Properties().GetComponentNameByID(id),other);
        }
        else if(storage.contains(obj.ID())){
            
            obj.TryCopyComponent(obj.Properties().GetComponentNameByID(id),other);
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

ObjectHandle Registry::FindObjectByName(std::string name) {
    
    
    for(auto [handle,comp] : Registry::Get().storage<ObjectPropertiesComponent>().each()){
        if(Object(handle).HasComponent<DisableInPlay>()){
            continue;
        }

        if(comp.GetName() == name){
            return ObjectHandle(handle);
        }
    }
    return ObjectHandle();
}
