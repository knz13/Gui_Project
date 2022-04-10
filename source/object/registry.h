#pragma once
#include "../general/helpers.h"
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

    template<typename T>
    static std::string GetClassName(){
        std::string name = std::string(entt::type_id<T>().name());
        HelperFunctions::EraseWordFromString(name,"Component");
        HelperFunctions::EraseWordFromString(name,"class ");
        return name;
    }

    template<typename T>
    static entt::id_type HashClassName() {
        return entt::hashed_string(Registry::GetClassName<T>().c_str());
    }


private:
    static std::mt19937 m_RandomGenerator;
    static reactphysics3d::PhysicsCommon m_PhysicsManager;
    static entt::registry m_Registry;

};