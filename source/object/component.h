#pragma once 
#include "../object/object.h"

template<typename U>
class AddWhenCalled;

template<typename T,typename Behavior=AddWhenCalled<T>>
class Component : public Behavior{
public:
    bool IsEnabled(){
        return GetActiveState();
    }
    bool IsVisibleInEditor(){
        return !m_ShouldHideInEditor;
    }
protected:
    Component(entt::entity master) {
        m_MasterHandle = master;
        
    }

    Component<T,Behavior>& operator=(const Component<T,Behavior>& comp){
        m_MyClassTypeID = comp.m_MyClassTypeID;
        m_ShouldHideInEditor = comp.m_ShouldHideInEditor;
        m_BaseComponentActiveState = comp.m_BaseComponentActiveState;
        return *this;

    }

    void HideInEditor(bool state){
        m_ShouldHideInEditor = state;
    }
    void SetActiveState(bool state){
        m_BaseComponentActiveState = state;
    }


    ~Component(){}

    Object GetMasterObject() {
        return Object(m_MasterHandle);
    }

    bool GetActiveState() {
        return m_BaseComponentActiveState;
    }

    

private:

    
    entt::id_type m_MyClassTypeID;
    entt::entity m_MasterHandle = entt::null;
    bool m_BaseComponentActiveState = true;
    bool m_ShouldHideInEditor = false;
    static inline bool Initialized = [](){Object::RegisterClassAsComponent<T>(); return true;}();

    friend class Object;

};