#pragma once
#include "registry.h"

class Object;

class ObjectHandle {
public:
	ObjectHandle(entt::entity ent);
	ObjectHandle();

	Object GetAsObject();

	template<typename T>
	T GetAs() {
		return T(m_Handle);
	}

	operator bool() {
		if (isNull) {
			return false;
		}
		return Registry::Get().valid(m_Handle);
	};

	bool operator==(const ObjectHandle& other);



private:
	entt::entity m_Handle = entt::null;
	bool isNull = false;

};

class ObjectProperties {
public:

	ObjectProperties(std::string name,entt::entity e);

	std::string GetName() const;
	void SetName(std::string name);

	void CallUpdateFunctions(float deltaTime);
	


	ObjectHandle GetParent();
	void SetParent(Object e);
	void ClearParent();
	bool IsInChildren(Object obj);
	void RemoveChildren(Object e);
	void AddChildren(Object e);
	const std::vector<ObjectHandle>& GetChildren() const;

	const std::unordered_map<entt::id_type, std::string>& GetComponentsWithIDAndName() const;


private:
	void AddComponent(entt::id_type id, std::string name);
	void EraseComponent(entt::id_type id);

	void ApplyFuncToSelfAndChildren(std::function<void(Object)> func);


	std::unordered_map<entt::id_type, std::string> m_ComponentClassNamesByType;


	std::vector<ObjectHandle> m_Children;
	ObjectHandle m_Parent = ObjectHandle();
	std::string m_Name;
	ObjectHandle m_Master;

	friend class Object;
	friend class Registry;
};

