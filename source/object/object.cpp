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