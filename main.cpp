#include <iostream>
#include "kv.h"
#include "gui_layer/gui_layer.h"




int main(){
    

    GuiLayer::Init();

    WindowCreationProperties prop;
    prop.openGLVersionMajor = 4;
    prop.openGLVersionMinor = 3;
    prop.width = 1280;
    prop.height = 720;
    prop.title = "window";
    Window win(prop);

    GuiLayer::AddUi(win);
    

    


    win.DrawingLoop();

    


    
}