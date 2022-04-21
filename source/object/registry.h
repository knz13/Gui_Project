#pragma once
#include "../general/helpers.h"
#include "../vendor/entt/single_include/entt/entt.hpp"
#include "../vendor/react_physics/include/reactphysics3d/reactphysics3d.h"
#include <random>

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