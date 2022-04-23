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
            return *((bool*)func.invoke({}, this->ID()).data());
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

void Object::ShowObjectProperties()
{
    auto resolved = entt::resolve(entt::hashed_string(ObjectPropertyRegister::GetClassNameByID(this->GetTypeOfObject()).c_str()));

    if (resolved) {
        if (auto func = resolved.func(entt::hashed_string("Show Properties")); func) {
            func.invoke({},m_EntityHandle);
        }
    }
}



