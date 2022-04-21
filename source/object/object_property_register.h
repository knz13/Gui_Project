#pragma once

#include <iostream>
#include <unordered_map>
#include <concepts>
#include "registry.h"
#include "../general/helpers.h"
#include "../../vendor/entt/single_include/entt/entt.hpp"

class ObjectPropertyRegister {
public:

	template<typename Component,typename... Args>
	static void MakeComponentPresentIn() {
		((ObjectPropertyRegister::))
	}

	template<typename T>
	static void MakeComponentOmnipresent() {

	};

	template<typename T>
	static void RegisterClassAsPropertyStorage() {

	};

	template<typename T>
	static void RegisterClassAsObjectTag() {

	}

	template<typename T>
	static void InitializeObject(entt::entity ent) {

	}



private:

	template<typename Component,typename T>
	static void MakeComponentPresentBackground() {
		m_ComponentsToMakeAvailableAtStartByType[HelperFunctions::HashClassName<T>()].push_back(HelperFunctions::HashClassName<Component>());

	};

	std::vector <std::string> m_ComponentsToMakeOmnipresent;
	std::unordered_map < entt::id_type, std::string> m_PropertyStorageContainer;
	std::unordered_map<entt::id_type, std::vector<std::string>> m_ComponentsToMakeAvailableAtStartByType;
	
	

};