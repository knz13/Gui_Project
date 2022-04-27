#include "object_property_register.h"
#include "../kv.h"


Object ObjectPropertyRegister::CreateObjectFromType(std::string type, std::string objectName)
{
	auto resolved = entt::resolve(entt::hashed_string(type.c_str()));

	if (resolved) {
		if (auto func = resolved.func(entt::hashed_string("Create")); func) {
			auto result = func.invoke({}, objectName);
			return Object(*((entt::entity*)result.data()));
		}
		throw std::runtime_error("Couldn't identify create function for object, make sure it is derived from TaggedObject");
	}
	throw std::runtime_error("Couldn't identify object type specified, check for spelling errors...");
	
}

std::vector<std::string> ObjectPropertyRegister::GetObjectComponents(entt::entity e)
{
	std::vector<std::string> vec;
	for (auto [id, storage] : Registry::Get().storage()) {
		if (id == entt::type_hash<ObjectProperties>().value()) {
			continue;
		}
		if (std::find(m_RegisteredComponentsByType[Object(e).GetTypeOfObject()].begin(), m_RegisteredComponentsByType[Object(e).GetTypeOfObject()].end(), ObjectPropertyRegister::GetComponentNameByID(id)) == m_RegisteredComponentsByType[Object(e).GetTypeOfObject()].end()) {
			continue;
		}
		if (storage.contains(e)) {
			vec.push_back(GetComponentNameByID(id));
		}
	}
	return vec;

}

std::string ObjectPropertyRegister::GetClassNameByID(entt::id_type id)
{
	if (m_RegisteredObjectNames.find(id) != m_RegisteredObjectNames.end()) {
		return m_RegisteredObjectNames[id];
	}
	return "";
}

void ObjectPropertyRegister::Each(std::function<void(Object)> func)
{
	Registry::Get().each([&](entt::entity e) {
		if (ObjectHandle(e)) {
			func(Object(e));
		}
	});
}

bool ObjectPropertyRegister::DeleteObject(ObjectHandle obj)
{
	if (obj) {
		m_ObjectsToDelete.push_back(obj);
		return true;
	}
	DEBUG_LOG("Could not delete object with id " + obj.ToString() + " because it was not valid!");
	return false;
}

void ObjectPropertyRegister::ClearDeletingQueue()
{
	for (auto& objHandle : m_ObjectsToDelete) {
		if (!objHandle) {

			continue;
		}
		std::vector<ObjectHandle> objectAndAllChildren;
		
		GetAllChildren(objHandle, objectAndAllChildren);
		

		for (auto& objectHandle : objectAndAllChildren) {
			if (!objectHandle) {
				DEBUG_LOG("Could not delete object with id " + objectHandle.ToString() + " because it was not valid! During ObjectPropertyRegister::ClearDeletingQueue()");
				continue;
			}
			Object object(objectHandle.ID());
			auto it = object.Properties().m_ComponentClassNames.begin();
			while (it != object.Properties().m_ComponentClassNames.end()) {
				object.EraseComponentByName(*it);
				it = object.Properties().m_ComponentClassNames.begin();
			}

			Registry::Get().destroy(object.ID());
		}

	}
	m_ObjectsToDelete.clear();
}

void ObjectPropertyRegister::GetAllChildren(ObjectHandle current, std::vector<ObjectHandle>& vec)
{
	current.GetAsObject().Properties().ClearParent();

	vec.push_back(current);

	for (auto handle : current.GetAsObject().Properties().GetChildren()) {
		if (handle) {
			GetAllChildren(handle, vec);
		}
	}
}
