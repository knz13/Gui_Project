#include <iostream>
#include "kv.h"
#include "gui_layer/gui_layer.h"



int main(){
    Object::RegisterClassAsComponent<Movable>();
    Object::RegisterClassAsComponent<Camera>();
    Object::RegisterClassAsComponent<Drawable>();

    GuiLayer::Init();

    WindowCreationProperties prop;
    prop.openGLVersionMajor = 4;
    prop.openGLVersionMinor = 3;
    prop.width = 1280;
    prop.height = 720;
    prop.title = "window";
    Window win(prop);


    
    





    GuiLayer::AddUi(win);
    
    Object obj = Registry::CreateObject("Cube");

    Drawable& dr = obj.GetComponent<Drawable>();
    obj.GetComponent<Movable>().SetPosition(0,0,-10);
    
    
    std::vector<float> positions = {
      // front
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    // back
    -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0
    };

    std::vector<unsigned int> indices = {
        // front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
    };

    

    dr.GetVertexArray().CreateVertexBuffer(8)
        .AddAttribute(positions,false)
        .Generate();

    dr.GetVertexArray().CreateIndexBuffer()
        .SetIndices(indices);

    
    dr.SetShader("default_shaders/base_shader");
    

    while(win.IsOpen()){

        
        win.DrawingLoop();

    }
    


    
}