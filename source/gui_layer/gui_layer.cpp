#include "gui_layer.h"
#include "../kv.h"


void GuiLayer::Init() {
    Window::WindowCreationEvent().Connect([](Window& win){
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(win.GetContextPointer(), true);
        ImGui_ImplOpenGL3_Init("#version 430 core");
    });

    
}

void GuiLayer::AddUi(Window& win) {
    
    

    if (NFD_Init() == NFD_OKAY) {
        win.Events().ClosingEvent().Connect([](Window& win) {
            NFD_Quit();
            });
    }
    ImGui::ClearIniSettings();
    
    GuiLayer::GameView::Setup(win);
    GuiLayer::ExplorerView::Setup(win);
    GuiLayer::SceneHierarchyView::SetupDefaultObjects();

    

    win.Events().PostDrawingLoopEvent().Connect([&](Window& win){
        
        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
            if (ImGui::IsKeyPressed(ImGuiKey_Equal)) {
                BaseSettings::FontScale += 0.1;
            }
            if (ImGui::IsKeyPressed(ImGuiKey_Minus)) {
                BaseSettings::FontScale -= 0.1;
            }
        }

        ImGui::PopStyleColor(2);

        ImGuiIO& io = ImGui::GetIO();
        ImGui::Render();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        

    });
    
    
    

    

    win.Events().PreDrawingLoopEvent().Connect([&](Window& win){

        static bool firstLoop = true;


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
        
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, Color(0, 0, 0).AsImVec4());
        ImGui::PushStyleColor(ImGuiCol_TitleBg, Color(0, 0, 0).AsImVec4());
        //setting up docks
        
        ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
        ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::Begin("BackWindow",0,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking);
        ImGui::PopStyleVar(2);
        
        if (ImGui::BeginMainMenuBar()) {

            if (ImGui::BeginMenu("File")) {
                std::string currPath = std::filesystem::current_path().string() + "/Assets/Scenes";
                if (ImGui::MenuItem("New Scene")) {

                }

                if (ImGui::MenuItem("Open Scene")) {
                    NFD::UniquePath path;

                    nfdfilteritem_t filterItem[1] = { {"Scene Files","scene"}};

                    auto result = NFD::OpenDialog(path,filterItem,1,currPath.c_str());

                    if (result == NFD_OKAY) {

                        if (BaseSettings::LoadedScenePath != "") {
                            //ObjectPropertyRegister::SerializeScene(BaseSettings::LoadedScenePath);
                        }
                        ObjectPropertyRegister::ClearEntities();

                        BaseSettings::LoadedScenePath = path.get();

                        ObjectPropertyRegister::DeserializeScene(BaseSettings::LoadedScenePath);
                    }
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Save")) {
                    if (BaseSettings::LoadedScenePath == "") {
                        nfdfilteritem_t filterItem[1] = { {"Scene Files","scene"} };

                        NFD::UniquePath path;
                        auto result = NFD::SaveDialog(path,filterItem,1,currPath.c_str(), "NewScene");

                        if (result == NFD_OKAY) {
                            BaseSettings::LoadedScenePath = path.get();
                            

                            ObjectPropertyRegister::SerializeScene(BaseSettings::LoadedScenePath);

                        }
                    }
                    else {
                        
                        ObjectPropertyRegister::SerializeScene(BaseSettings::LoadedScenePath);
                        
                    }
                }

                if (ImGui::MenuItem("Save As")) {
                    NFD::UniquePath path;
                    nfdfilteritem_t filterItem[1] = { {"Scene Files","scene"} };

                    auto result = NFD::SaveDialog(path, filterItem, 1, currPath.c_str(), "NewScene");

                    if (result == NFD_OKAY) {
                        BaseSettings::LoadedScenePath = path.get();
                        

                        ObjectPropertyRegister::SerializeScene(BaseSettings::LoadedScenePath);
                    }
                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        


        ImGuiID id = ImGui::GetID("MainWindowGroup"); 

        
        
        
        ImGui::DockSpace(id);
        
        if(firstLoop){
            
            
            ImGui::DockBuilderRemoveNode(id);            
            ImGui::DockBuilderAddNode(id,ImGuiDockNodeFlags_AutoHideTabBar);

            ImGui::DockBuilderSetNodeSize(id, ImGui::GetMainViewport()->WorkSize);
            ImGui::DockBuilderSetNodePos(id, ImGui::GetMainViewport()->WorkPos);

            ImGuiID dock1 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.2f, nullptr, &id);
            ImGuiID dock2 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.2f, nullptr, &id);
            ImGuiID dock3 = ImGui::DockBuilderSplitNode(dock2, ImGuiDir_Left, 0.75f, nullptr, &dock2);
            ImGuiID dock4 = ImGui::DockBuilderSplitNode(dock3, ImGuiDir_Down, 0.3f, nullptr, &dock3);

            WindowIDs::CreateDock("Scene Hierarchy", dock1);
            WindowIDs::CreateDock("Properties", dock2);
            WindowIDs::CreateDock("Explorer", dock4);
            WindowIDs::CreateDock("Game View", dock3);

            

            ImGui::DockBuilderFinish(id);
            firstLoop = false;

            
        }
        

       
        GuiLayer::GameView::Update(win);
        
        GuiLayer::ExplorerView::Update(win);
        
        GuiLayer::SceneHierarchyView::Update(win);


        GuiLayer::PropertiesView::Update(win);
        
       
        ImGui::End();

    
    });
}



void GuiLayer::SetupWindowStyle(std::string name,std::function<void(ImGuiWindowFlags)> beginCommand) {
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::PushStyleColor(ImGuiCol_WindowBg,BaseColors::WindowBg.AsImVec4());
   
    
    ImGui::PushStyleColor(ImGuiCol_ChildBg,BaseColors::WindowBg.AsImVec4());
    ImGui::PushStyleColor(ImGuiCol_FrameBg,BaseColors::WindowBg.AsImVec4());

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleColor(ImGuiCol_Tab, BaseColors::WindowBg.AsImVec4());
    ImGui::PushStyleColor(ImGuiCol_TabActive, BaseColors::WindowBg.AsImVec4());
    ImGui::PushStyleColor(ImGuiCol_TabHovered, BaseColors::WindowBg.AsImVec4());
    ImGui::PushStyleColor(ImGuiCol_TabUnfocused, BaseColors::WindowBg.AsImVec4());
    ImGui::PushStyleColor(ImGuiCol_TabUnfocusedActive, BaseColors::WindowBg.AsImVec4());


    ImGui::SetNextWindowDockID(WindowIDs::GetID(name),ImGuiCond_Once);
    beginCommand(flags);

    ImGui::SetWindowFontScale(BaseSettings::FontScale);
    ImGui::PopStyleColor(8);
    ImGui::PopStyleVar(3);
}
void GuiLayer::SetupChildStyle(std::function<void()> command)
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, (BaseColors::WindowBg - Color(10,10,10)).AsImVec4());
    ImGui::PushStyleColor(ImGuiCol_WindowBg, (BaseColors::WindowBg - Color(10,10,10)).AsImVec4());
    //ImGui::PushStyleColor(ImGuiCol_, (BaseColors::WindowBg - Color(20, 20, 20)).AsImVec4());
    

    command();
    ImGui::PopStyleColor(2);
}

bool GuiLayer::LoadScene(std::string path)
{
    if (!AssetRegister::IsPathRegisteredAs<SceneAsset>(path)) {
        return false;
    }
    if (BaseSettings::LoadedScenePath != "") {
        //ObjectPropertyRegister::SerializeScene(BaseSettings::LoadedScenePath);
        //TODO: popup here
    }
    ObjectPropertyRegister::ClearEntities();

    BaseSettings::LoadedScenePath = path;

    if (!ObjectPropertyRegister::DeserializeScene(BaseSettings::LoadedScenePath)) {
        DEBUG_LOG("Could not deserialize scene at path: " + path);
        return false;
    }

    return true;

}

std::string GuiLayer::GetImGuiID(void* ptr) {
    return ("##" + std::to_string(std::hash<void*>()(ptr)));
}



void GuiLayer::SetupWidgetStyle(std::function<void()> beginCommand) {
    ImGui::PushStyleColor(ImGuiCol_FrameBg,ImVec4(60,60,60,1));
    ImGui::PushStyleColor(ImGuiCol_TitleBg,ImVec4(60,60,60,1));
    
    beginCommand();
    ImGui::PopStyleColor(2);
}

void GuiLayer::SetupStaticTreeNodeStyle(std::function<void()> beginCommand)
{
    ImGui::PushStyleColor(ImGuiCol_Header, BaseColors::StaticWidgetBg.AsImVec4());
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, BaseColors::StaticWidgetActiveBg.AsImVec4());
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, BaseColors::StaticWidgetHoveredBg.AsImVec4());
    beginCommand();
    ImGui::PopStyleColor(3);
}

void GuiLayer::SetupStaticButtonStyle(std::function<void()> beginCommand)
{
    ImGui::PushStyleColor(ImGuiCol_Button,BaseColors::StaticWidgetBg.AsImVec4());
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,BaseColors::StaticWidgetActiveBg.AsImVec4());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,BaseColors::StaticWidgetHoveredBg.AsImVec4());
    beginCommand();
    ImGui::PopStyleColor(3);
}

ImGuiID GuiLayer::WindowIDs::GetID(std::string name)
{
    return m_RegisteredIDs[name];
}

void GuiLayer::WindowIDs::CreateDock(std::string name, ImGuiID id)
{
    ImGui::DockBuilderDockWindow(name.c_str(),id);
    m_RegisteredIDs[name] = id;
}

ObjectHandle& GuiLayer::AnyObjectSelected() {
    static ObjectHandle m_Selected;

    return m_Selected;
}
