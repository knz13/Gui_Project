#include "object.h"
#include "../kv.h"
#include "../general/transform.h"

Object::~Object() {
  
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

Component<const void*,const void*>* Object::TryAddComponent(std::string stringToHash) {
    auto resolved = entt::resolve(entt::hashed_string(stringToHash.c_str()));
    if(resolved){
        entt::meta_any owner = resolved.construct(*this);
        return (Component<const void*,const void*>*)owner.data();
    }
    else{
        return nullptr;
    }
}

Component<const void*,const void*>* Object::GetComponentByName(std::string stringToHash){
    return TryAddComponent(stringToHash);
}