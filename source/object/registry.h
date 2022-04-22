#pragma once
#include "../general/helpers.h"
#include "../vendor/entt/single_include/entt/entt.hpp"
#include "../vendor/react_physics/include/reactphysics3d/reactphysics3d.h"
#include <random>

namespace ComponentHelpers {
    class Null {

    private:
        int dummy = 0;
    
    };
}

class Object;
class ObjectHandle;
class Registry{
public:


    static void DeleteObject(Object obj);
    
    
    /**
     * Function that needs to be called after every frame, when no components or objects are being used
     */
    static void UpdateState();

    static reactphysics3d::PhysicsCommon& GetPhysicsCommon();
    
    static std::string GetComponentDisplayName(std::string componentClassName);

    static entt::registry& Get();
    static size_t GenerateRandomNumber();
    

    static ObjectHandle FindObjectByName(std::string name);



private:
   
    static void GetAllChildren(Object current,std::vector<Object>& objects);

    static std::vector<Object> m_ObjectsToDelete;
    static std::mt19937 m_RandomGenerator;
    static reactphysics3d::PhysicsCommon m_PhysicsManager;
    static entt::registry m_Registry;

};


class Object;

class ObjectHandle {
public:
    ObjectHandle(entt::entity ent);
    ObjectHandle();

    Object GetAsObject();

    template<typename T>
    T GetAs() {
        return T(m_Handle);
    }

    operator bool() {
        if (isNull) {
            return false;
        }
        return Registry::Get().valid(m_Handle);
    };

    bool operator==(const ObjectHandle& other);



private:
    entt::entity m_Handle = entt::null;
    bool isNull = false;

};

template<typename T>
class TemplatedObjectHandle {
public:
    TemplatedObjectHandle(entt::entity ent) {
        m_Handle = ent;
        isNull = false;
    }
    TemplatedObjectHandle() {
        isNull = true;
    }

    T GetAsObject() {
        return T(m_Handle);
    }

    operator bool() {
        if (isNull) {
            return false;
        }
        return Registry::Get().valid(m_Handle);
    };

private:
    entt::entity m_Handle = entt::null;
    bool isNull = false;
};