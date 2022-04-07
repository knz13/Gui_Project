#include "component.h"
#include "../kv.h"

Object Component::GetMasterObject() {
    return Object(m_MasterHandle);
}
