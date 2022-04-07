#pragma once 
#include "../global.h"

class Object;
class Component {

protected:
    Component(entt::entity master) {
        m_MasterHandle = master;
    }

    Object GetMasterObject();

private:
    entt::entity m_MasterHandle;

};