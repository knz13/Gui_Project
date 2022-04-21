#include "registry.h"
#include "object.h"
#include "object_properties.h"
#include "../kv.h"
#include <ctime>

std::vector<Object> Registry::m_ObjectsToDelete;
reactphysics3d::PhysicsCommon Registry::m_PhysicsManager;
entt::registry Registry::m_Registry;
std::mt19937 Registry::m_RandomGenerator(time(nullptr));
entt::registry& Registry::Get() {
    return Registry::m_Registry;
}



std::string Registry::GetComponentDisplayName(std::string componentClassName)
{
    auto resolved = entt::resolve(entt::hashed_string(componentClassName.c_str()));

    if (resolved) {
        if (auto func = resolved.func(entt::hashed_string("Get Display Name")); func) {

            if (auto result = func.invoke({}); result) {
                return result.cast<std::string>();
            }
            else {
                return "";
            }
        }
        else {
            return "";
        }
    }
    else {
        return "";
    }
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









void Registry::GetAllChildren(Object current, std::vector<Object>& objects)
{
    current.Properties().ClearParent();

    objects.push_back(current);

    for (auto handle : current.Properties().GetChildren()) {
        if (handle.GetAsObject().Valid()) {
            GetAllChildren(handle.GetAsObject(), objects);
        }
    }
}



void Registry::UpdateState() {
    for (auto& obj : m_ObjectsToDelete) {
        std::vector<Object> objectAndAllChildren;
        if(obj.Valid()){
            GetAllChildren(obj, objectAndAllChildren);
        }

        for (auto& object : objectAndAllChildren) {
            auto it = object.Properties().m_ComponentClassNamesByType.begin();
            while (it != object.Properties().m_ComponentClassNamesByType.end()) {
                object.EraseComponentByName(it->second);
                it = object.Properties().m_ComponentClassNamesByType.begin();
            }

            m_Registry.destroy(object.ID());
        }

    }
    m_ObjectsToDelete.clear();
}

ObjectHandle Registry::FindObjectByName(std::string name) {
    
    
    for(auto [handle,comp] : Registry::Get().storage<ObjectProperties>().each()){
        if(Object(handle).HasComponent<DisableInPlay>()){
            continue;
        }

        if(comp.GetName() == name){
            return ObjectHandle(handle);
        }
    }
    return ObjectHandle();
}
