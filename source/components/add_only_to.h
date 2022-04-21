#pragma once
#include "component_behavior.h"
#include "../object/object_property_register.h"


template<typename Master,typename... Dependents>
class AddOnlyTo {
public:
	AddOnlyTo() {
		(void*)m_DummyVar;
	};

private:
	static inline bool m_DummyVar = []() {ObjectPropertyRegister::MakeComponentPresentIn<Dependents...>(); return false; }();

};