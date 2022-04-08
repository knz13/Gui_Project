#pragma once 
#include "../global.h"

class Object;
class Component {

protected:
    Component(entt::entity master) {
        m_MasterHandle = master;
    }
    ~Component();

    Object GetMasterObject();

    bool GetActiveState();
    void SetActiveState(bool state);
private:
    
    entt::id_type m_MyClassTypeID;
    entt::entity m_MasterHandle;
    bool m_BaseComponentActiveState = true;
    
    
    friend class Object;

};