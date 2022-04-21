#pragma once
#include "object_property_register.h"

template<typename Storage,typename MainClass>
class ObjectPropertyStorage {
public:
	ObjectPropertyStorage() {
		dummyVariable = true;
	};

private:
	static inline bool dummyVariable = []() {
		ObjectPropertyRegister::RegisterClassAsPropertyStorage<Storage,MainClass>();
		return false;
	}();

};