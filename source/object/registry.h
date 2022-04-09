#pragma once

#include "../vendor/entt/single_include/entt/entt.hpp"
#include "../vendor/react_physics/include/reactphysics3d/reactphysics3d.h"
#include <random>

class Object;
class Registry{
public:


    static void DeleteObject(entt::entity handle);

    static reactphysics3d::PhysicsCommon& GetPhysicsCommon();
    static Object CreateObject(std::string name);
    static entt::registry& Get();
    static size_t GenerateRandomNumber();
    static Object CopyEntity(Object other);
    


private:
    static std::mt19937 m_RandomGenerator;
    static reactphysics3d::PhysicsCommon m_PhysicsManager;
    static entt::registry m_Registry;

};