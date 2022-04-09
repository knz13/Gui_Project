#pragma once 
#include "../global.h"
#include "explorer_view.h"
#include "game_view.h"
#include "properties_view.h"
#include "scene_hierarchy_view.h"



class Window;

namespace GuiLayer {

    void Init();

    void AddUi(Window& win);
    

    std::string GetImGuiID(void* ptr);

    void SetupWidgetStyle(std::function<void()> beginCommand);
    void SetupWindowStyle(std::function<void(ImGuiWindowFlags)> beginCommand);

};

