#pragma once
#include "../general/helpers.h"
#include "../vendor/entt/single_include/entt/entt.hpp"

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


    
    
    
    

    
    
    static std::string GetComponentDisplayName(std::string componentClassName);

    static entt::registry& Get();
    static size_t GenerateRandomNumber();
    

    static ObjectHandle FindObjectByName(std::string name);

    


private:
   
    

    
    static std::mt19937 m_RandomGenerator;
   
    static entt::registry m_Registry;

};


class Object;

class ObjectHandle {
public:
    ObjectHandle(entt::entity ent);
    ObjectHandle(Object obj);
    ObjectHandle();

    Object GetAsObject();

    template<typename T>
    T GetAs() {
        return T(m_Handle);
    }

    operator bool() const{
        if (isNull) {
            return false;
        }
        return Registry::Get().valid(m_Handle);
    };

    entt::entity ID();

    std::string ToString();

    bool IsType(entt::id_type type);

    bool operator==(const ObjectHandle& other);



private:
    entt::entity m_Handle = entt::null;
    bool isNull = false;

};
