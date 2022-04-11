#pragma once
#include "../object/object.h"

//dummy class that specifies that component will only get created when the AddComponent or GetComponent is called

template<typename T>
class AddWhenCalled{
public:
    AddWhenCalled(){

    };
};