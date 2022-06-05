#pragma once
#include "../global.h"

namespace GuiLayer {
	class PropertiesView;
}

template<typename Derived>
class BaseObject : public ecspp::RegisterObjectType<Derived> {
public:
	BaseObject(entt::entity e) : ecspp::RegisterObjectType<Derived>(e) {};
private:
	virtual void ShowProperties() {};


	friend class GuiLayer::PropertiesView;

};

template<typename Derived>
class ComponentlessBaseObject : public ecspp::RegisterComponentlessObjectType<Derived> {
public:
	ComponentlessBaseObject(entt::entity e) : ecspp::RegisterComponentlessObjectType<Derived>(e) {};
private:
	virtual void ShowProperties() {};


	friend class GuiLayer::PropertiesView;
};