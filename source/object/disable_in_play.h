#pragma once
#include "../global.h"
#include "component.h"

class DisableInPlay : public Component<DisableInPlay> {
    void Init() { Component::HideInEditor(true);};

};

