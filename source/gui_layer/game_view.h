#pragma once
#include "../global.h"
#include "gui_view.h"

class Window;
class Object;
class Framebuffer;

namespace GuiLayer {
class GameView : public GuiView {
public:
    static RayCastHit RayCast(ImVec2 pos);
    static void Setup(Window& win);
    static void Update(Window& win);

    static ClickedObjectProperties& AnyObjectSelected();
private:
    static std::shared_ptr<Framebuffer> m_RaycastTexture;
    static inline ClickedObjectProperties m_IsObjectSelected;
    static std::unique_ptr<Framebuffer> m_Buffer;

};
};