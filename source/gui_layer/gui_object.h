#pragma once
#include "../object/tagged_object.h"


class GuiObject;

template<typename ComponentType = ComponentHelpers::Null, typename... Behaviors>
class GuiComponent : public ComponentSpecifier<ComponentType, GuiObject>, public Behaviors... {




};

class GuiObject : public TaggedObject<GuiObject,GuiComponent<>> {


};

class GuiObjectStorage : public ObjectPropertyStorage<GuiObjectStorage, GuiObject> {


};