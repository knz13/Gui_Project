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
    
    Object obj = Registry::CreateObject("Cube");

    Mesh& dr = obj.GetComponent<Mesh>();
    obj.GetComponent<TransformComponent>().SetPosition(0,0,-10);

    MeshAttribute::Vertex vertices;

    vertices.positions = {
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

    vertices.indices = {
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
    

    dr.SetVertices(vertices);
    dr.SetShader("default_shaders/base_shader");
    
    

    while(win.IsOpen()){

        
        win.DrawingLoop();

    }
    


    
}