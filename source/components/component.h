#pragma once 
#include <concepts>
#include "../../vendor/entt/single_include/entt/entt.hpp"
#include "../object/registry.h"





class Component {
public:
    bool Valid() {
        return Registry::Get().valid(m_MasterHandle);
    }

    
protected:
    
    Component& operator=(const Component& comp){
        return *this;

    }
    /**
     * Use instead of constructor.
     */
    virtual void Init() {};
    /**
     * Use instead of destructor.
     */
    virtual void Destroy() {};

    

    

    

    ~Component(){}

    

    


private:

    void SetMaster(entt::entity entity) {
        m_MasterHandle = entity;
    };
    
    
    entt::entity m_MasterHandle = entt::null;
    
    
    
    
    
    template<typename,typename>
    friend class ComponentSpecifier;
    
    
    friend class ObjectPropertyRegister;

};




