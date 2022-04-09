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

    static std::string GetImGuiID(void* ptr);

    static void SetupWidgetStyle(std::function<void()> beginCommand);

    static std::shared_ptr<Framebuffer> m_RaycastTexture;
private:

    static void SetupFileExplorer(Window& win);
    static void SetupGameView(Window& win);
    static void SetupPropertiesView(Window& win);
    static void SetupSceneHierarchy(Window& win);
    static void SetupWindowStyle(std::function<void(ImGuiWindowFlags)> beginCommand);

    static inline ClickedObjectProperties m_IsObjectSelected;
    static std::unique_ptr<Framebuffer> m_Buffer;


};