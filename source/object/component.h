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

    bool IsAvailableDuringPlay() {
        return m_IsAvailableDuringPlay;
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
    /**
     * Use instead of constructor.
     */
    virtual void Init() {};
    /**
     * Use instead of destructor.
     */
    virtual void Destroy() {};

    virtual void Update(float deltaTime) {};
    virtual void ShowProperties() {};

    void HideInEditor(bool state){
        m_ShouldHideInEditor = state;
    }
    void SetActiveState(bool state){
        m_BaseComponentActiveState = state;
    }

    void MakeRemovable(bool state){
        m_IsRemovable = state;
    }

    void SetDisabledInPlay(bool state) {
        m_IsAvailableDuringPlay = !state;
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
    bool m_IsAvailableDuringPlay = true;
    static inline bool Initialized = [](){Object::RegisterClassAsComponent<T>(); return true;}();

    friend class Object;
    friend class ObjectPropertiesComponent;
    

};



template<>
class Component<const void*,const void*> : public AddWhenCalled<const void*>{
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

    Component<const void*,const void*>& operator=(const Component<const void*,const void*>& comp){
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
    virtual void ShowProperties() {};

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
    
    static inline bool Initialized = true;

    friend class Object;
    friend class ObjectPropertiesComponent;
    

};



