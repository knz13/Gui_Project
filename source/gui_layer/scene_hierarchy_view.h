#pragma once
#include "../global.h"
#include "gui_view.h"

class Window;

namespace GuiLayer {

struct SceneHierarchyViewComponent : public GameComponent<SceneHierarchyViewComponent>, public ecspp::AddOnlyTo<SceneHierarchyViewComponent,GameObject> {
    void Init() override;
    
    

    bool m_IsChoosingName = false;

    
};


class SceneHierarchyView : public GuiView {
public:
    static void SetupDefaultObjects();
    static void Update(Window& win);

private:
    static void SetupObject(GameObject obj);

    static inline std::map<std::string,std::function<void()>> m_DefaultObjects;
};
};