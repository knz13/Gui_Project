#pragma once

#include <iostream>
#include <unordered_map>
#include <concepts>
#include "registry.h"
#include "../general/helpers.h"
#include "../../vendor/entt/single_include/entt/entt.hpp"

class Object;
class ObjectPropertyRegister {
public:

	template<typename Component,typename... Args>
	static void MakeComponentPresentIn() {
		(ObjectPropertyRegister::MakeComponentPresentBackground<Component, Args>(), ...);
	}

	template<typename T>
	static void MakeComponentOmnipresent() {
		m_ComponentsToMakeOmnipresent.push_back(HelperFunctions::GetClassName<T>());
	};

	template<typename Storage,typename MainClass>
	static void RegisterClassAsPropertyStorage() {
		m_PropertyStorageContainer[HelperFunctions::HashClassName<MainClass>()] = [](entt::entity e) {
			Registry::Get().emplace<Storage>(e);
		};
	};

	template<typename Tag,typename Attached>
	static void RegisterClassAsObjectTag() {
		entt::id_type hash = HelperFunctions::HashClassName<Attached>();
		entt::meta<Attached>().type(hash).template func<&ObjectPropertyRegister::ForEachByTag<Tag,Attached>>(entt::hashed_string("ForEach"));
		m_RegisteredObjectTagsStartingFuncs[hash] = [](entt::entity e) {
			Registry::Get().emplace<Tag>(e);
		};
	}

	template<typename T>
	static void InitializeObject(entt::entity ent) {

		entt::id_type hash = HelperFunctions::HashClassName<T>();
		
		
		
		if (m_RegisteredObjectTagsStartingFuncs.find(hash) != m_RegisteredObjectTagsStartingFuncs.end()) {
			m_RegisteredObjectTagsStartingFuncs[hash](ent);
		}

		if (m_PropertyStorageContainer.find(hash) != m_PropertyStorageContainer.end()) {
			m_PropertyStorageContainer[hash](ent);
		}

		T obj(ent);

		obj.Init();

		if (m_ComponentsToMakeAvailableAtStartByType.find(hash) != m_ComponentsToMakeAvailableAtStartByType.end()) {
			for (auto& componentName : m_ComponentsToMakeAvailableAtStartByType[hash]) {
				obj.AddComponentByName(componentName);
			}
		}

		for (auto& componentName : m_ComponentsToMakeOmnipresent) {
			obj.AddComponentByName(componentName);
		}


	}



private:

	template<typename Tag,typename Attached>
	static void ForEachByTag(std::function<void(Attached)> func) {
		auto view = Registry::Get().view<Tag>();
		for (auto entity : view) {
			func(Attached(entity));
		}
	}

	template<typename Component,typename T>
	static void MakeComponentPresentBackground() {
		m_ComponentsToMakeAvailableAtStartByType[HelperFunctions::HashClassName<T>()].push_back(HelperFunctions::GetClassName<Component>());

	};

	inline static std::vector <std::string> m_ComponentsToMakeOmnipresent;
	inline static std::unordered_map < entt::id_type, std::function<void(entt::entity)>> m_PropertyStorageContainer;
	inline static std::unordered_map<entt::id_type, std::vector<std::string>> m_ComponentsToMakeAvailableAtStartByType;
	inline static std::unordered_map<entt::id_type, std::function<void(entt::entity)>> m_RegisteredObjectTagsStartingFuncs;
	
	

};