#pragma once
#include "../global.h"
#include "gui_view.h"

class Window;
namespace GuiLayer {
class PropertiesView : public GuiView{
public:
    static void Update(Window& win);


};
};