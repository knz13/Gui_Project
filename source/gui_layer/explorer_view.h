#pragma once
#include "../global.h"
#include "gui_view.h"

class Window;
namespace GuiLayer {
class ExplorerView : public GuiView {
public:
    static void Update(Window& win);

    static void Setup(Window& win);
    
private:
    static inline size_t m_CurrentRenamingFileHash = 0;
    static inline ObjectHandle m_RenamingObject;

};
};