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

	template<typename T>
	static T CopyObject(T other) {
		entt::entity firstObject = entt::null;
		entt::entity lastObject = entt::null;

		other.Properties().ApplyFuncToSelfAndChildren([&](T current) {
			T newOne = DuplicateObject<T>(current);
			if (firstObject == entt::null) {
				firstObject = newOne.ID();
			}
			if (lastObject != entt::null) {
				newOne.Properties().SetParent(T(lastObject));
			}
			lastObject = newOne.ID();
			});
		return T(firstObject);
	};


	template<typename T>
	static T CreateNew(std::string name) {
		static_assert(std::is_base_of<Object, T>::value);

		entt::entity ent = Registry::Get().create();

		int index = 1;
		if (Registry::FindObjectByName(name)) {
			if (name.find_last_of(")") == std::string::npos || (name.find_last_of(")") != name.size() - 1)) {
				name += "(" + std::to_string(index) + ")";
			}
		}

		while (Registry::FindObjectByName(name)) {
			index++;
			name.replace(name.find_last_of("(") + 1, std::to_string(index - 1).size(), std::to_string(index));
		}

		Registry::Get().emplace<ObjectProperties>(ent, name, ent);

		ObjectPropertyRegister::InitializeObject<T>(ent);

		return T(ent);
	}


private:
	template<typename T>
	static T DuplicateObject(T other) {
		T obj = ObjectPropertyRegister::CreateNew<T>(other.Properties().GetName());

		for (auto [id, storage] : Registry::Get().storage()) {
			if (id == entt::type_hash<ObjectProperties>().value()) {
				continue;
			}
			if (storage.contains(other.ID()) && !storage.contains(obj.ID())) {
				obj.AddComponentByName(other.GetComponentNameByID(id));
				obj.CopyComponentByName(other.GetComponentNameByID(id), other);
			}
			else if (storage.contains(obj.ID())) {

				obj.CopyComponentByName(obj.GetComponentNameByID(id), other);
			}
		}

		return obj;
	};

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