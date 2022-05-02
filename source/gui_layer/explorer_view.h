#pragma once
#include "../global.h"
#include "gui_view.h"

class Window;
namespace GuiLayer {
class ExplorerView : public GuiView {
public:
    static void Update(Window& win);

    static void Setup(Window& win);
    static void Reload();

    static void SetCurrentPath(std::string path);
    static void SetupDefaultPopupMenuWidgets();
    
    
    static ObjectHandle& GetTempObject();
private:
    static void OnUpdatePathOrReload();
    static void ShowAllSubFolders(std::string current);
    static bool FolderHasFoldersInside(std::string folderPath);
    static void SetupFolderExplorerAboveFileExplorer();

    static inline ObjectHandle m_CurrentCreatingObject;
    static inline size_t m_CurrentRenamingFileHash = 0;
    static inline ObjectHandle m_RenamingObject;
    static inline ImVec2 m_WidgetSize = ImVec2(80,80);
    static inline const std::string m_InitialPath = std::filesystem::current_path().string() + "/Assets";
    static inline std::string m_CurrentPath = "";
    static inline std::unordered_map<std::string,std::vector<std::string>> m_CurrentFilesByFolder;
};
};