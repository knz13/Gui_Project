#pragma once
#include "object.h"
#include "../components/component_specifier.h"
#include <filesystem>

template<typename DerivedObjectClass>
class ObjectTag {

private:
	int dummy = 0;

};


template<typename Derived,typename DerivedComponent,typename DerivedStorage>
class TaggedObject : public Object {
public:
	TaggedObject(entt::entity e) : Object(e) {
		(void)dummyVariable;
	};

	static void ForEach(std::function<void(Derived)> function) {
		auto resolved = entt::resolve(HelperFunctions::HashClassName<Derived>());

		if (resolved) {
			if (auto func = resolved.func(entt::hashed_string("ForEach")); func) {
				func.invoke({}, function);
			}
		}
	
	}

	void ApplyFuncToSelfAndChildren(std::function<void(Derived)> func) {
		func(*((Derived*)this));
		if (Properties().GetChildren().size() == 0) {
			return;
		}
		for (auto id : Properties().GetChildren()) {
			id.GetAs<Derived>().ApplyFuncToSelfAndChildren(func);
		}
	};


	static const std::vector<std::string>& GetRegisteredComponents() {
		return ObjectPropertyRegister::m_RegisteredComponentsByType[HelperFunctions::HashClassName<Derived>()];
	};

	DerivedComponent* AddComponentByName(std::string stringToHash) {
		return ObjectPropertyRegister::AddComponentByName<DerivedComponent>(this->ID(), stringToHash);
	};

	DerivedComponent* GetComponentByName(std::string stringToHash) {
		return ObjectPropertyRegister::GetComponentByName<DerivedComponent>(this->ID(), stringToHash);
	};
	
protected:
	
	DerivedStorage& Storage() {
		return Registry::Get().get_or_emplace<DerivedStorage>(ID());
	}

private:
	
	
	static inline int dummyVariable = []() {
		ObjectPropertyRegister::RegisterClassAsObjectTag<ObjectTag<Derived>, Derived>();
		ObjectPropertyRegister::RegisterClassAsPropertyStorage<DerivedStorage, Derived>();
		
		return 0;
	}();


};


