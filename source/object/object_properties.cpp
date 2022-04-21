#include "object_properties.h"
#include "../kv.h"


ObjectHandle::ObjectHandle(entt::entity ent)
{
	m_Handle = ent;
}

ObjectHandle::ObjectHandle()
{
	isNull = true;
}

Object ObjectHandle::GetAsObject()
{
	return Object(m_Handle);
}

bool ObjectHandle::operator==(const ObjectHandle& other)
{
	return this->m_Handle == other.m_Handle;
}


ObjectProperties::ObjectProperties(std::string name, entt::entity e) : m_Name(name),m_Master(e)
{
}

std::string ObjectProperties::GetName() const
{
	return m_Name;
}

void ObjectProperties::SetName(std::string name)
{
	m_Name = name;
}

void ObjectProperties::CallUpdateFunctions(float deltaTime)
{
	for (auto& [id, name] : m_ComponentClassNamesByType) {
		m_Master.GetAsObject().GetComponentByName(name)->Update(deltaTime);
	}
}



ObjectHandle ObjectProperties::GetParent()
{
	return m_Parent;
}

void ObjectProperties::SetParent(Object e)
{
	this->m_Parent = ObjectHandle(e.ID());
	e.Properties().AddChildren(m_Master.GetAsObject());
}

void ObjectProperties::ClearParent()
{
	if (m_Parent) {
		m_Parent.GetAsObject().Properties().RemoveChildren(m_Master.GetAsObject());
	}
	this->m_Parent = ObjectHandle();
}

bool ObjectProperties::IsInChildren(Object obj)
{
	ObjectHandle handle(obj.ID());
	if (m_Children.size() == 0) {
		return false;
	}
	auto it = std::find(m_Children.begin(), m_Children.end(), handle);
	if (it != m_Children.end()) {
		return true;
	}
	for (auto& id : m_Children) {
		if (id.GetAsObject().Properties().IsInChildren(obj)) {
			return true;
		}
	}
	return false;
}

void ObjectProperties::RemoveChildren(Object e)
{
	ObjectHandle handle(e.ID());
	auto it = std::find(m_Children.begin(), m_Children.end(), handle);
	if (e.Valid() && it != m_Children.end()) {
		m_Children.erase(it);
	}
}

void ObjectProperties::AddChildren(Object e)
{
	ObjectHandle handle(e.ID());
	if (e.Valid() && std::find(m_Children.begin(), m_Children.end(), handle) == m_Children.end()) {
		m_Children.push_back(e.ID());
	}
}

const std::vector<ObjectHandle>& ObjectProperties::GetChildren() const
{
	return m_Children;
}

const std::unordered_map<entt::id_type, std::string>& ObjectProperties::GetComponentsWithIDAndName() const
{
	return m_ComponentClassNamesByType;
}

void ObjectProperties::AddComponent(entt::id_type id, std::string name)
{
	m_ComponentClassNamesByType[id] = name;
}

void ObjectProperties::EraseComponent(entt::id_type id )
{
	if (m_ComponentClassNamesByType.find(id) != m_ComponentClassNamesByType.end()) {
		m_ComponentClassNamesByType.erase(id);
	}
}

void ObjectProperties::ApplyFuncToSelfAndChildren(std::function<void(Object)> func)
{
	func(m_Master.GetAsObject());
		if (m_Children.size() == 0) {
			return;
		}
	for (auto& id : m_Children) {
		id.GetAsObject().Properties().ApplyFuncToSelfAndChildren(func);
	}
}