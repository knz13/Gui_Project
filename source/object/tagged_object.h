#pragma once
#include "object.h"
#include "object_property_storage.h"

template<typename DerivedObjectClass>
class ObjectTag {

private:
	int dummy = 0;

};


template<typename Derived>
class TaggedObject : public Object {
public:
	TaggedObject(entt::entity e) : Object(e) {
		(void*)dummyVariable;
	};

	static void ForEach(std::function<void(Derived)> function) {
		auto resolved = entt::resolve(HelperFunctions::HashClassName<Derived>());

		if (resolved) {
			if (auto func = resolved.func(entt::hashed_string("ForEach")); func) {
				func.invoke({}, function);
			}
		}
	
	}


	


private:
	static inline int dummyVariable = []() {
		ObjectPropertyRegister::RegisterClassAsObjectTag<ObjectTag<Derived>, Derived>();
		return 0;
	}();


};