#pragma once
#include "../global.h"
#include "component.h"

class DisableInPlay : public DeriveFromComponent<DisableInPlay> {
public:
    void Init() { Component::HideInEditor(true);};

};

