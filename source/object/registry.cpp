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

    m_Registry.emplace<ObjectPropertiesComponent>(ent,name);

    Object obj(ent);

    obj.GetComponent<Movable>();

    return obj;

}

void Registry::DeleteObject(entt::entity handle) {
    if(m_Registry.valid(handle)){
        m_Registry.destroy(handle);
    }
}

reactphysics3d::PhysicsCommon& Registry::GetPhysicsCommon() {
    return m_PhysicsManager; 
}

size_t Registry::GenerateRandomNumber() {
    return m_RandomGenerator();
}
