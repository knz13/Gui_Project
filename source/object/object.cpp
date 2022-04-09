#include "object.h"
#include "../kv.h"

Object::~Object() {
  
}

Object::Object(entt::entity ent) {
    if(!Registry::Get().valid(ent)){
        DEBUG_ERROR("Passing an invalid entity!!!");
    }
    m_EntityHandle = ent;    
}

Movable& Object::Transform() {
    return GetComponent<Movable>();
}