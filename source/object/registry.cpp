#include "registry.h"
#include "object.h"
#include "object_properties_component.h"
#include "../kv.h"

entt::registry Registry::m_Registry;
entt::registry& Registry::Get() {
    return Registry::m_Registry;
}

Object Registry::CreateObject() {
    entt::entity ent = m_Registry.create();

    m_Registry.emplace<ObjectPropertiesComponent>(ent);

    Object obj(ent);

    obj.GetComponent<Movable>();

    return obj;

}

void Registry::DeleteObject(entt::entity handle) {
    if(m_Registry.valid(handle)){
        m_Registry.destroy(handle);
    }
}
