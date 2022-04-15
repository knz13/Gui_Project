#pragma once 
#include "add_to_every_object.h"
#include <concepts>

namespace ComponentHelpers {
    class Null;
};

template<typename T=ComponentHelpers::Null,typename ... Behaviors>
class Component : public Behaviors...{
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
    Component() {

    };

    Component<T,Behaviors...>& operator=(const Component<T,Behaviors...>& comp){
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
    static inline bool Initialized = [](){
        if(entt::type_id<T>().name() != entt::type_id<ComponentHelpers::Null>().name()){
            Object::RegisterClassAsComponent<T>(); 
        }
        return true;
    }();

    friend class Object;
    friend class ObjectPropertiesComponent;
    

};


namespace ComponentHelpers {
    class Null : public Component<Null>{
    };

};




