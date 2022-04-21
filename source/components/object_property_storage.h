#pragma once
#include "../object/object_property_register.h"

template<typename MainClass>
class ObjectPropertyStorage {
public:
	ObjectPropertyStorage() {
		(void*)dummyVariable;
	};

private:
	bool dummyVariable = []() {
		ObjectPropertyRegister::RegisterClassAsPropertyStorage<MainClass>();
		return false;
	}();

};