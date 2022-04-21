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
            return func.invoke({}, *this).operator bool();
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

TransformComponent& Object::Transform() {
    return GetComponent<TransformComponent>();
}

Component* Object::AddComponentByName(std::string stringToHash) {
    auto resolved = entt::resolve(entt::hashed_string(stringToHash.c_str()));
    if(resolved){
        entt::meta_any owner = resolved.construct(*this);
        return (Component*)owner.data();
    }
    else{
        return nullptr;
    }
}

Component* Object::GetComponentByName(std::string stringToHash){
    return AddComponentByName(stringToHash);
}

std::string Object::GetComponentNameByID(entt::id_type id)
{
    if (Properties().m_ComponentClassNamesByType.find(id) != Properties().m_ComponentClassNamesByType.end()) {
        return Properties().m_ComponentClassNamesByType[id];
    }
    return "";
}
