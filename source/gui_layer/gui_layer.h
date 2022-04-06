#pragma once 
#include "../global.h"

class Object;
class Framebuffer;
class Window;

class GuiLayer {
public:

    static void Init();

    static void AddUi(Window& win);
    static RayCastHit RayCast(ImVec2 pos);

    static std::shared_ptr<Framebuffer> m_RaycastTexture;
private:
    static std::unique_ptr<Framebuffer> m_Buffer;


};