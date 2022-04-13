#pragma once 
#include "add_to_every_object.h"
#include "add_when_called.h"


template<typename T,typename Behavior=AddWhenCalled<T>>
class Component : public Behavior{
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
protected:
    Component(){

    };

    Component<T,Behavior>& operator=(const Component<T,Behavior>& comp){
        m_MyClassTypeID = comp.m_MyClassTypeID;
        m_ShouldHideInEditor = comp.m_ShouldHideInEditor;
        m_BaseComponentActiveState = comp.m_BaseComponentActiveState;
        return *this;

    }

    virtual void Init() {};
    virtual void Destroy() {};

    void HideInEditor(bool state){
        m_ShouldHideInEditor = state;
    }
    void SetActiveState(bool state){
        m_BaseComponentActiveState = state;
    }

    void MakeRemovable(bool state){
        m_IsRemovable = state;
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
    
    entt::id_type m_MyClassTypeID;
    entt::entity m_MasterHandle = entt::null;
    bool m_BaseComponentActiveState = true;
    bool m_ShouldHideInEditor = false;
    bool m_IsRemovable = true;
    static inline bool Initialized = [](){Object::RegisterClassAsComponent<T>(); return true;}();

    friend class Object;
    

};