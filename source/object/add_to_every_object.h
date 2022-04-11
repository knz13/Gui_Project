#pragma once
#include "../object/object.h"

// dummy class to just specify to add to all objects created from the start

template<typename T>
class AddToEveryObject{
public:
    AddToEveryObject(){
        
    };

private:
    static inline bool m_DummyVar = [](){Object::MakeComponentOmnipresent<T>(); return false;}();
};