#pragma once
#include "../global.h"
#include "gui_view.h"

class Window;

namespace GuiLayer {

struct SceneHierarchyViewComponent : public Component<SceneHierarchyViewComponent,AddToEveryObject<SceneHierarchyViewComponent>> {
    SceneHierarchyViewComponent(entt::entity e) : Component(e){
        this->HideInEditor(true);
    };

    void ShowProperties(){};
    void Update(float time){};

    bool m_IsChoosingName = false;

    
};


class SceneHierarchyView : public GuiView {
public:
    static void Update(Window& win);

private:
    static void SetupObject(Object obj);
};
};