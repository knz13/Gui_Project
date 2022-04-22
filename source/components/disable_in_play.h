#pragma once
#include "../global.h"
#include "component.h"

class DisableInPlay : public GameComponent<DisableInPlay> {
public:
    void Init() { this->HideInEditor(true);};

};

