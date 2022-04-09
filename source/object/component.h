#pragma once 
#include "../global.h"

class Object;
class Component {
public:

    void SetActiveState(bool state);
protected:
    Component(entt::entity master) {
        m_MasterHandle = master;
        
    }

    
    ~Component();

    Object GetMasterObject();

    bool GetActiveState();

private:

    
    entt::id_type m_MyClassTypeID;
    entt::entity m_MasterHandle = entt::null;
    bool m_BaseComponentActiveState = true;
    
    
    friend class Object;

};