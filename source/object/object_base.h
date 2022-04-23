#pragma once

#include "../../vendor/entt/single_include/entt/entt.hpp"
#include <iostream>

// dummy class to make the functions available to the register;

class ObjectBase {

protected:
	virtual void ShowProperties() {};

private:
	template<typename ObjectType>
	static void CallShowPropertiesForObject(entt::entity e) {
		auto obj = ObjectType(e);
		((ObjectBase*)(&obj))->ShowProperties();
	};

	friend class ObjectPropertyRegister;


};