#include "component.h"
#include "../kv.h"

Object Component::GetMasterObject() {
    return Object(m_MasterHandle);
}

bool Component::GetActiveState() {
    return m_BaseComponentActiveState;
}

void Component::SetActiveState(bool state) {
    m_BaseComponentActiveState = state;
}

Component::~Component() {
    
}

