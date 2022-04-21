#pragma once
#include "tagged_object.h"
#include "../general/structures.h"




class TransformComponent;
class GameObject : public TaggedObject<GameObject> {
	KV_CLASS
public:
	GameObject(entt::entity e);

	TransformComponent& Transform();

	void SetHighlightState(bool state);
	void SetHighlightColor(Color color);
	Color GetHighlightColor();
	bool GetHighlightState();
	bool IsActive();
	void SetActiveState(bool state);
	

	void ShowProperties();
	
	
protected:


};


class GameObjectProperties : public ObjectPropertyStorage<GameObjectProperties,GameObject> {
private:

	bool m_Active = true;
	Color m_HighlightColor = Color::Red;
	bool m_HighlightState = false;

	friend class GameObject;
};