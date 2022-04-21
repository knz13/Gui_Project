#pragma once 
#include "../object/object.h"
#include <concepts>
#include "../../vendor/entt/single_include/entt/entt.hpp"






class Component {
public:
    bool IsEnabled(){
        return GetActiveState();
    }
    bool IsVisibleInEditor(){
        return !m_ShouldHideInEditor;
    }
    bool IsRemovable(){
        return m_IsRemovable;
    }

    bool Valid() {
        return Registry::Get().valid(m_MasterHandle);
    }

    bool CanBeDisabled() {
        return m_CanBeDisabled;
    }
    virtual void ShowProperties() {};
protected:
    
    Component& operator=(const Component& comp){
        m_MyClassTypeID = comp.m_MyClassTypeID;
        m_ShouldHideInEditor = comp.m_ShouldHideInEditor;
        m_BaseComponentActiveState = comp.m_BaseComponentActiveState;
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

    virtual void Update(float deltaTime) {};

    void HideInEditor(bool state){
        m_ShouldHideInEditor = state;
    }
    void SetActiveState(bool state){
        m_BaseComponentActiveState = state;
    }

    void MakeRemovable(bool state){
        m_IsRemovable = state;
    }

    void MakeAlwaysEnabled(bool state){
        m_CanBeDisabled = !state;
    }

    

    ~Component(){}

    Object GetMasterObject() const {
        return Object(m_MasterHandle);
    }

    bool GetActiveState() {
        return m_BaseComponentActiveState;
    }


private:

    void SetMaster(entt::entity entity) {
        m_MasterHandle = entity;
    };
    
    entt::id_type m_MyClassTypeID = 0;
    entt::entity m_MasterHandle = entt::null;
    bool m_BaseComponentActiveState = true;
    bool m_ShouldHideInEditor = false;
    bool m_CanBeDisabled = true;
    bool m_IsRemovable = true;
    
    
    
    


    friend class Object;
    
    friend class ObjectProperties;

};




