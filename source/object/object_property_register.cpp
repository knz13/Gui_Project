#include "object_property_register.h"
#include "../kv.h"


ObjectHandle ObjectPropertyRegister::CreateObjectFromType(std::string type, std::string objectName)
{
	auto resolved = entt::resolve(entt::hashed_string(type.c_str()));

	if (resolved) {
		if (auto func = resolved.func(entt::hashed_string("Create")); func) {
			auto result = func.invoke({}, objectName);
			if (result) {
				return ObjectHandle(*((entt::entity*)result.data()));
			}
			DEBUG_LOG("Create function for object with name " + objectName + " was not successful!");
			return {};
		}
		DEBUG_LOG("Couldn't identify create function for object, make sure it is derived from TaggedObject");
		return {};
	}
	DEBUG_LOG("Couldn't identify object type specified, check for spelling errors...");
	return {};
	
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

bool ObjectPropertyRegister::IsClassRegistered(std::string className)
{
	return entt::resolve(entt::hashed_string(className.c_str())).operator bool();
}

bool ObjectPropertyRegister::SerializeScene(std::string savePath)
{

	if (std::filesystem::is_directory(savePath)) {
		return false;
	}

	

	YAML::Node mainRoot;

	

	GameObject::ForEach([&](GameObject obj){

		
		YAML::Node node;
		SerializeObject(ObjectHandle(obj.ID()),node);
		mainRoot.push_back(node);

	});


	YAML::Emitter em;
	em << mainRoot;

	std::string val = em.c_str();

	

	std::ofstream stream;

	stream.open(savePath);

	
	stream << val;
	

	stream.close();



	return true;
}

bool ObjectPropertyRegister::DeserializeScene(std::string path)
{
	return false;
}

ObjectHandle ObjectPropertyRegister::DeserializeObject(std::string objectType,YAML::Node& node)
{
	if(node.IsDefined()){
		return {};
	}
	if (!IsClassRegistered(objectType)) {
		return {};
	}

	if (!node["name"]) {
		return {};
	}
	
	std::string name;
	name = node["name"].as<std::string>();

	ObjectHandle obj = ObjectPropertyRegister::CreateObjectFromType(objectType,name);

	if (!obj) {
		return {};
	}

	if (!obj.GetAsObject().DeserializeBaseObject(node)) {
		DEBUG_LOG("Could not deserialize object with name " + name + " and type " + objectType + "!");
		return obj;
	}

	if (!node["Components"]) {
		DEBUG_LOG("Could not add components to object with name" + name + " and type " + objectType + "!" + "\nBecause components was not found in the YAML::Node!");
		return obj;
	}
	

	for (auto children : node["Components"]) {
		if (!children.IsDefined()) {
			continue;
		}

		if (!children["type"] || !children["name"]) {
			continue;
		}

		std::string type, componentName;
		componentName = children["name"].as<std::string>();
		type = children["type"].as<std::string>();

		Component* comp = ObjectPropertyRegister::GetComponentByName<Component>(obj.ID(), componentName);

		if (!comp) {
			DEBUG_LOG("Could not create component with type " + type + "!");
			continue;
		}

		if (!comp->Deserialize(node)) {
			DEBUG_LOG("Could not deserialize component of type " + type + "!");
		};

	}
	return obj;



	

}

bool ObjectPropertyRegister::SerializeObject(ObjectHandle obj, YAML::Node& node)
{
	if (!obj) {
		DEBUG_LOG("Trying to serialize invalid object with id: " + obj.ToString());
		return false;
	}

	YAML::Node& outerRef = node;

	

	if (!obj.GetAsObject().SerializeBaseObject(outerRef)) {
		DEBUG_LOG("Base object serialization for object " + obj.GetAsObject().GetName() + " failed unexpectedly.");
		return false;
	}

	auto serializeResult = HelperFunctions::CallMetaFunction(obj.GetAsObject().GetType(), "Serialize", obj.ID(),outerRef);

	if (!serializeResult) {
		DEBUG_LOG("Meta function for object " + obj.GetAsObject().GetName() + " of type " + obj.GetAsObject().GetType() + " did not succeed!");
		outerRef.reset();
		return false;
	}


	

	for (auto& componentName : ObjectPropertyRegister::GetObjectComponents(obj.ID())) {
		
		if (IsClassRegistered(componentName)) {
			
			YAML::Node ref;
			
			
			auto result = HelperFunctions::CallMetaFunction(componentName, "Serialize", obj.ID(), ref);
			if (result) {
				if (!* ((bool*)result.data())) {
					DEBUG_LOG("Could not serialize component " + componentName + " for object " + obj.GetAsObject().GetName());
					ref.reset();
				}
				else {
					outerRef["Components"][componentName] = ref;
				}
			}
			else {
				DEBUG_LOG("Could not serialize component " + componentName + " for object " + obj.GetAsObject().GetName() + " because Serialize meta function was not successful!");
				ref.reset();
			}
			
			
		}
	}

	return true;

	





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
