#pragma once
#include "../global.h"
#include "gui_view.h"

class Window;

namespace GuiLayer {

struct SceneHierarchyViewComponent : public Component<SceneHierarchyViewComponent,AddToEveryObject<SceneHierarchyViewComponent>> {
    
    void Init() { this->HideInEditor(true);};
    void ShowProperties(){};
    void Update(float time){};

    bool m_IsChoosingName = false;

    
};


class SceneHierarchyView : public GuiView {
public:
    static void SetupDefaultObjects();
    static void Update(Window& win);

private:
    static void SetupObject(Object obj);

    static inline std::map<std::string,std::function<void()>> m_DefaultObjects;
};
};