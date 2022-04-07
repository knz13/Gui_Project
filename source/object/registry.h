#pragma once

#include "../vendor/entt/single_include/entt/entt.hpp"

class Object;
class Registry{
public:


    static void DeleteObject(entt::entity handle);



    static Object CreateObject(std::string name);
    static entt::registry& Get();


private:
    static entt::registry m_Registry;

};