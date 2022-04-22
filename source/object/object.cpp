#include "object.h"
#include "../kv.h"
#include "../general/transform.h"

Object::~Object() {
  
}

bool Object::HasComponent(std::string type)
{
    auto resolved = entt::resolve(entt::hashed_string(type.c_str()));

    if (resolved) {
        if (auto func = resolved.func(entt::hashed_string("Has Component")); func) {
            return func.invoke({}, this->ID()).operator bool();
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }

}


Object::Object(entt::entity ent) {
    if(!Registry::Get().valid(ent)){
        DEBUG_ERROR("Passing an invalid entity!!!");
    }
    m_EntityHandle = ent;
}

entt::id_type Object::GetTypeOfObject()
{
    return Properties().m_MasterType;
}

bool Object::HasSameObjectTypeAs(Object other)
{
    return Properties().m_MasterType == other.Properties().m_MasterType;
}

