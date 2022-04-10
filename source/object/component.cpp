#include "component.h"
#include "../kv.h"

Object Component::GetMasterObject() {
    return Object(m_MasterHandle);
}


void Component::SetActiveState(bool state) {
    m_BaseComponentActiveState = state;
}

Component::~Component() {
    
}


bool Component::GetActiveState() {
    return m_BaseComponentActiveState;
}

Component& Component::operator=(const Component& comp) {
    m_MyClassTypeID = comp.m_MyClassTypeID;
    m_BaseComponentActiveState = comp.m_BaseComponentActiveState;
    return *this;
}
