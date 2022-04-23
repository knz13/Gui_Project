#pragma once
#include "../object/tagged_object.h"


class GuiObject;

template<typename ComponentType = ComponentHelpers::Null>
class GuiComponent : public ComponentSpecifier<ComponentType, GuiObject> {




};

class GuiObject : public TaggedObject<GuiObject,GuiComponent<>> {
public:
	GuiObject(entt::entity e);

};

class GuiObjectStorage : public ObjectPropertyStorage<GuiObjectStorage, GuiObject> {


};