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
    static void SetupEditorCameraDrawing();
    static void HandleEditorCameraMovement(Window& win);
    static void HandleSelectionGuizmo(Window& win);
    static void HandleObjectSelection(Window& win);

    static bool m_CanSelect;
    static std::shared_ptr<Framebuffer> m_RaycastTexture;
    static ObjectHandle m_EditorCamera;
    static inline ClickedObjectProperties m_IsObjectSelected;
    static inline ImVec2 m_ContentRectMin;
    static inline ImVec2 m_ContentRectMax;
};
};