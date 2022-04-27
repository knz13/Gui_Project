#pragma once
#include "../global.h"
#include "gui_view.h"

class Window;
namespace GuiLayer {
class ExplorerView : public GuiView {
public:
    static void Update(Window& win);

    static void Setup(Window& win);

    static void SetCurrentPath(std::string path);
    
private:
    static void OnUpdatePathOrReload();

    static inline size_t m_CurrentRenamingFileHash = 0;
    static inline ObjectHandle m_RenamingObject;
    static inline ImVec2 m_WidgetSize = ImVec2(100,100);
    static inline const std::string m_InitialPath = std::filesystem::current_path().string() + "/Assets";
    static inline std::string m_CurrentPath = std::filesystem::current_path().string() + "/Assets";
    static inline std::unordered_map<std::string,std::vector<std::string>> m_CurrentFilesByFolder;
};
};