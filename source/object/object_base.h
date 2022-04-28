#pragma once

#include "../../vendor/entt/single_include/entt/entt.hpp"
#include <iostream>

// dummy class to make the functions available to the register;

class ObjectBase {

protected:
	virtual void ShowProperties() {};
	virtual void Init() {};
	virtual void Destroy() {};

	/**
	 * Serializer function.
	 * Called before Destroy().
	 * *WARNING* -> node is already inside of component
	 */
	virtual bool Serialize(YAML::Node node) { return true; };

	/**
	 * Deserializer function.
	 * Called after Init()
	 * *WARNING* -> node is already inside of component
	 */
	virtual bool Deserialize(YAML::Node node) { return true; };



private:

	

	template<typename ObjectType>
	static void CallShowPropertiesForObject(entt::entity e) {
		auto obj = ObjectType(e);
		((ObjectBase*)(&obj))->ShowProperties();
	};

	friend class ObjectPropertyRegister;


};