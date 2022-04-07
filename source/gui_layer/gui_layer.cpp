#include "gui_layer.h"
#include "../kv.h"

std::shared_ptr<Framebuffer> GuiLayer::m_RaycastTexture;
std::unique_ptr<Framebuffer> GuiLayer::m_Buffer;
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
    

    ImGui::ClearIniSettings();
    
    


    win.Events().PostDrawingLoopEvent().Connect([&](Window& win){
        GL_CALL(glMemoryBarrier(GL_ALL_BARRIER_BITS));

        m_Buffer.get()->Unbind();
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

    
    
    win.Events().ClosingEvent().Connect([&](Window& win){
        GuiLayer::m_RaycastTexture.reset();
        m_Buffer.reset();
    });

    win.Events().PreDrawingLoopEvent().Connect([&](Window& win){

        static bool firstLoop = true;
        static int imguizmoMode = ImGuizmo::OPERATION::TRANSLATE;
        static bool initialized = false;
        static ImVec2 lastSize;
        static ClickedObjectProperties wasClicked;

        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuizmo::BeginFrame();

        
        

        
        
        //setting up docks
        

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoTitleBar ;
        
        
        ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
        ImGui::Begin("BackWindow",0,ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);
        


        ImGuiID id = ImGui::GetID("MainWindowGroup"); 

        
        
        
        ImGui::DockSpace(id,ImGui::GetWindowViewport()->WorkSize,  ImGuiDockNodeFlags_NoResize);
        
        if(firstLoop){
            
            
            ImGui::DockBuilderRemoveNode(id);            
            ImGui::DockBuilderAddNode(id,ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_NoResize);

            ImGui::DockBuilderSetNodeSize(id, ImGui::GetWindowViewport()->WorkSize);
            ImGui::DockBuilderSetNodePos(id, ImGui::GetWindowViewport()->WorkPos);

            ImGuiID dock1 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.2f, nullptr, &id);
            ImGuiID dock2 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.8f, nullptr, &id);
            ImGuiID dock3 = ImGui::DockBuilderSplitNode(dock2, ImGuiDir_Left, 0.6f, nullptr, &dock2);
            ImGuiID dock4 = ImGui::DockBuilderSplitNode(dock3, ImGuiDir_Down, 0.4f, nullptr, &dock3);

            ImGui::DockBuilderDockWindow("Objects", dock1);
            ImGui::DockBuilderDockWindow("Properties", dock2);
            ImGui::DockBuilderDockWindow("Explorer", dock4);
            ImGui::DockBuilderDockWindow("Game View", dock3);    
            ImGui::DockBuilderFinish(id);
            firstLoop = false;

            
        }
        
        ImGui::End();
        
        
        
        
        GuiLayer::SetupWindowStyle([&](){
            ImGui::Begin("Objects",0,flags );
        });
        
        if(Registry::Get().alive() > 0){
            
            Registry::Get().each([&](const entt::entity e){
                Object obj(e);
                ImGui::SetNextItemOpen(true);
                if(ImGui::TreeNodeEx(obj.Properties().GetName().c_str())){
                    
                    if(ImGui::IsItemClicked(ImGuiMouseButton_Left)){
                        if(wasClicked){
                            Object(wasClicked.objectID).Properties().SetHightlightState(false);
                        }
                        wasClicked = ClickedObjectProperties(e);
                        Object(wasClicked.objectID).Properties().SetHightlightState(true);
                    }
                    ImGui::TreePop();
                }
                
            });
    
        }

        ImGui::End();
        


        GuiLayer::SetupWindowStyle([&](){
            ImGui::Begin("Properties",0,flags );
        });

        if(wasClicked){
            Object(wasClicked.objectID).Properties().CallShowPropertiesFunctions();
        }

        ImGui::End();
        
        
        GuiLayer::SetupWindowStyle([&](){
            ImGui::Begin("Game View",0,flags );
        });

        ImGui::BeginChild("GameRender");
        if(!initialized){
            GuiLayer::m_RaycastTexture = std::make_shared<Framebuffer>(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
            m_Buffer = std::make_unique<Framebuffer>(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
            GL_CALL(glViewport(0,0,ImGui::GetWindowSize().x,ImGui::GetWindowSize().y));
            initialized = true;
        }
        else{
            if(lastSize.x != m_Buffer.get()->GetSize().x || lastSize.y != m_Buffer.get()->GetSize().y){
                GuiLayer::m_RaycastTexture = std::make_shared<Framebuffer>(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
                m_Buffer = std::make_unique<Framebuffer>(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
                GL_CALL(glViewport(0,0,ImGui::GetWindowSize().x,ImGui::GetWindowSize().y));
            }
        }
        m_Buffer.get()->Bind();
        lastSize = ImGui::GetWindowSize();
        // Get the size of the child (i.e. the whole draw size of the windows).
        ImVec2 wsize = ImGui::GetWindowSize();

        if(ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()){
            ImVec2 pos;
            pos.x = ImGui::GetMousePos().x - ImGui::GetWindowPos().x;
            pos.y = wsize.y - (ImGui::GetMousePos().y - ImGui::GetWindowPos().y);
            RayCastHit hit = RayCast(pos);
            if(hit){
                if(wasClicked){
                    Object(wasClicked.objectID).Properties().SetHightlightState(false);
                }
                wasClicked = ClickedObjectProperties(hit.hitObjectID);
                Object(wasClicked.objectID).Properties().SetHightlightState(true);
            }
           
        }
        if(ImGui::IsMouseDown(ImGuiMouseButton_Right) && ImGui::IsWindowHovered()){
            if(wasClicked){
                Object(wasClicked.objectID).Properties().SetHightlightState(false);
            }
            wasClicked = ClickedObjectProperties();
        }
        

        ImGui::Image((ImTextureID)GuiLayer::m_Buffer.get()->GetAttachedTexture().GetID(),wsize, ImVec2(0, 1), ImVec2(1, 0));
        if(wasClicked & win.GetCurrentCamera()){

            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y,ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);

            Object clickedObject(wasClicked.objectID);
            glm::mat4 proj = win.GetCurrentCamera().GetAsObject().GetComponent<Camera>().GetProjection(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
            glm::mat4 view = win.GetCurrentCamera().GetAsObject().GetComponent<Camera>().GetView();
            Movable& objectTransform = clickedObject.GetComponent<Movable>();
            
            if(objectTransform.GetScale().x == 0){
                objectTransform.SetScale(0.1,0,0);
            }
            if(objectTransform.GetScale().y == 0){
                objectTransform.SetScale(0,0.1,0);
            }
            if(objectTransform.GetScale().z == 0){
                objectTransform.SetScale(0,0,0.1);
            }

            glm::mat4 model = objectTransform.GetModelMatrix();

            float snap[5] = {0,0,0,0,0.1};
            ImGuizmo::Manipulate(glm::value_ptr(view),glm::value_ptr(proj),(ImGuizmo::OPERATION)imguizmoMode,ImGuizmo::MODE::LOCAL,glm::value_ptr(model),0,snap);


            if(ImGuizmo::IsUsing()){
                glm::vec3 position,scale;
                glm::quat rotation;
                glm::vec3 skew;
                glm::vec4 perspective;
                glm::decompose(model,scale,rotation,position,skew,perspective);

                objectTransform.m_Position = position;
                objectTransform.m_Rotation = glm::eulerAngles(rotation);
                objectTransform.m_Scale = scale;

            }

            

        }

        if(ImGui::IsKeyPressed(ImGuiKey_E)){
            imguizmoMode = ImGuizmo::OPERATION::TRANSLATE;
        }
        if(ImGui::IsKeyPressed(ImGuiKey_R)){
            imguizmoMode = ImGuizmo::OPERATION::ROTATE;
        }
        if(ImGui::IsKeyPressed(ImGuiKey_T)){
            imguizmoMode = ImGuizmo::OPERATION::SCALE;
        }

        
        ImGui::EndChild();



        ImGui::End();

        

        GuiLayer::SetupWindowStyle([&](){
            ImGui::Begin("Explorer",0,flags );
        });
        
        ImGui::End();

        m_RaycastTexture.get()->Bind();

        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        m_RaycastTexture.get()->Unbind();

        if(m_Buffer.get()->Status()){

            m_Buffer.get()->Bind();
        }


        m_RaycastTexture.get()->GetAttachedTexture().Bind();
        GL_CALL(glBindImageTexture(3,m_RaycastTexture.get()->GetAttachedTexture().GetID(),0,GL_FALSE,0,GL_WRITE_ONLY,GL_RGBA32F));

    });
}

RayCastHit GuiLayer::RayCast(ImVec2 pos) {
    m_RaycastTexture.get()->Bind();
    unsigned char data[4];
    GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    GL_CALL(glReadPixels(pos.x,pos.y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,data));
    
    uint32_t pickedID = data[0] + data[1]*256 + data[2]*256*256;
    pickedID--;
    
    uint32_t max = 0;
    max--;
    if(pickedID == max){
        return RayCastHit((entt::entity)max);
    }

    m_RaycastTexture.get()->Unbind();
    if(Registry::Get().valid((entt::entity)(pickedID))){
        return RayCastHit((entt::entity)pickedID);
    }
    else{
        return RayCastHit((entt::entity)max);
    }

}

void GuiLayer::SetupWindowStyle(std::function<void()> beginCommand) {
    ImGui::PushStyleColor(ImGuiCol_WindowBg,ImVec4(0,0,0,1));
    beginCommand();
    ImGui::PopStyleColor();
}