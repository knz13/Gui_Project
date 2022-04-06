#include "window.h"
#include "kv.h"


std::map<GLFWwindow*,Window*> Window::m_CurrentWindows;
EventLauncher<void(Window&)> Window::m_StartWindowFuncs;

Window::Window(WindowCreationProperties prop) : m_Properties(prop) {

    if(glfwInit() != GLFW_TRUE){
        DEBUG_ERROR("GLFW was not initiated!");
        return;
    }

    if(prop.windowFlags != WindowFlag::None){
        if(prop.windowFlags & WindowFlag::OpenGLDebugContext){
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,GLFW_TRUE);
        }
        if(prop.windowFlags & WindowFlag::InitiallyMinimized){
            glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
        }
        if(prop.windowFlags & WindowFlag::NotDecorated){
            glfwWindowHint(GLFW_DECORATED,GLFW_FALSE);
        }
        if(prop.windowFlags & WindowFlag::NotResizeable){
            glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);
        }
        if(prop.windowFlags & WindowFlag::NotFocused){
            glfwWindowHint(GLFW_FOCUSED,GLFW_FALSE);
        }
        if(prop.windowFlags & WindowFlag::FocusOnShow){
            glfwWindowHint(GLFW_FOCUS_ON_SHOW,GLFW_TRUE);
        }
        
        if((prop.openGLVersionMajor == 3 && prop.openGLVersionMinor > 2) || prop.openGLVersionMajor > 3){
            if(prop.windowFlags & WindowFlag::CoreProfile){
                glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
            }
            else {
                glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_COMPAT_PROFILE);
            }
        }
    }

    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,prop.openGLVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,prop.openGLVersionMinor);

    if(prop.windowFlags & WindowFlag::FullScreen){
        this->m_ContextPointer = glfwCreateWindow(prop.width,prop.height,prop.title.c_str(),glfwGetPrimaryMonitor(),NULL);
    }
    else {
        this->m_ContextPointer = glfwCreateWindow(prop.width,prop.height,prop.title.c_str(),NULL,NULL);
    }

    Window::m_CurrentWindows[this->m_ContextPointer] = this;
    

    glfwMakeContextCurrent(this->m_ContextPointer);

    glewExperimental = true;
    if(glewInit() != GLEW_OK){
        DEBUG_ERROR("Glew wasn't initiated!");
        return;
    }

    CameraCreationProperties camProp;
    Camera::GeneratePerspectiveCamera(camProp,*this);

    GL_CALL(glEnable(GL_PROGRAM_POINT_SIZE));

    glfwSetKeyCallback(this->GetContextPointer(),[](GLFWwindow* ptr,int key, int scancode, int action, int mods){
        Window& win = *Window::GetWindow(ptr);
        KeyEventProperties prop;
        prop.key = key;
        prop.scancode = scancode;
        prop.action = action;
        prop.mods = mods;
        win.m_KeyEventFuncs.EmitEvent(win,prop);
    });

    glfwSetCursorPosCallback(this->GetContextPointer(),[](GLFWwindow* window, double xpos, double ypos){
        Window& win = *Window::GetWindow(window);
        MouseEventProperties prop;
        prop.position = glm::vec2(xpos,ypos);
        win.m_MouseMovedFuncs.EmitEvent(win,prop);
    });

    glfwSetCursorEnterCallback(this->GetContextPointer(),[](GLFWwindow* window,int entered){
        double xpos,ypos;
        glfwGetCursorPos(window,&xpos,&ypos);
        MouseEventProperties prop;
        prop.position = glm::vec2(xpos,ypos);
        Window& win = *Window::GetWindow(window);
        if(entered){
            win.m_MouseEnteredWindowFuncs.EmitEvent(win,prop);
        }
        else {
            win.m_MouseLeftWindowFuncs.EmitEvent(win,prop);
        }
    });

    glfwSetMouseButtonCallback(this->GetContextPointer(),[](GLFWwindow* window, int button, int action, int mods){
        double xpos,ypos;
        glfwGetCursorPos(window,&xpos,&ypos);
        Window& win = *Window::GetWindow(window);
        MouseButtonEventProperties prop;
        prop.button = button;
        prop.action = action;
        prop.mods = mods;
        prop.position = glm::vec2(xpos,ypos);
        win.m_MouseButtonFuncs.EmitEvent(win,prop);
    });

    glfwSetScrollCallback(this->GetContextPointer(),[](GLFWwindow* window, double xoffset, double yoffset){
        Window& win = *Window::GetWindow(window);
        MouseScrollEventProperties prop;
        prop.offsetFromTopLeft = glm::vec2(xoffset,yoffset);
        win.m_MouseScrollFuncs.EmitEvent(win,prop);
    });

    glfwSetFramebufferSizeCallback(this->GetContextPointer(),[](GLFWwindow* window, int width, int height){
        Window& win = *Window::GetWindow(window);
        WindowResizedEventProperties prop;
        prop.width = width;
        prop.height = height;
        win.m_WindowResizedEventFuncs.EmitEvent(win,prop);
    });

    glfwSetWindowCloseCallback(this->GetContextPointer(),[](GLFWwindow* window){
        Window& win = *Window::GetWindow(window);
        win.m_IsOpen = false;
        
    });

    this->Events().ResizedEvent().Connect([](Window& win,WindowResizedEventProperties prop){
        win.m_Properties.width = prop.width;
        win.m_Properties.height = prop.height;
        glm::vec4 viewport = win.GetCurrentCamera().GetViewPort();
        GL_CALL(glViewport(viewport.x*win.m_Properties.width,viewport.y*win.m_Properties.height,viewport.z*win.m_Properties.width,viewport.w*win.m_Properties.height));
    
       

    });
    

    //handle creation events

    Window::m_StartWindowFuncs.EmitEvent(*this);


}

Window::~Window() {

    
    m_CreatedShaders.clear();
    m_CreatedVertexArrays.clear();

    m_DrawingQueue.clear();

    Window::m_CurrentWindows.erase(m_ContextPointer);

    glfwDestroyWindow(m_ContextPointer);
}

bool Window::IsOpen() {
    return !glfwWindowShouldClose(m_ContextPointer);
}

GLFWwindow* Window::GetContextPointer() {
    return m_ContextPointer;
}

void Window::EndDrawState() {
    glfwSwapBuffers(m_ContextPointer);
}

void Window::BeginDrawState() {
    glfwPollEvents();

    glm::vec3 color = m_ClearColor.Normalized();
    GL_CALL(glClearColor(color.x,color.y,color.z,1.0f));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}



void Window::SetClearColor(Color color) {
    m_ClearColor = color;
}

const WindowCreationProperties& Window::Properties() const {
    return m_Properties;
}

void Window::SetCamera(Camera& camera) {
    m_MainCamera = &camera;
}

Camera& Window::GetCurrentCamera() {
    return *m_MainCamera;
}

void Window::AddToDrawingQueue(unsigned int id) {
    m_DrawingQueue[id] = Drawable::m_DrawableObjects[id];
}

void Window::RemoveFromDrawingQueue(unsigned int id) {
    if(m_DrawingQueue.find(id) != m_DrawingQueue.end()){
        m_DrawingQueue.erase(id);
    }
}

WindowEvents Window::Events() {
    return WindowEvents(*this);
}

void Window::DrawingLoop() {
    static float currentTime=0,oldTime=0;

    

    glfwMakeContextCurrent(m_ContextPointer);

    m_PreDrawingLoopFuncs.EmitEvent(*this);
    
    BeginDrawState();
    
    currentTime = glfwGetTime();
    m_DeltaTime = static_cast<float>(currentTime - oldTime);

    oldTime = currentTime;

    Registry::Get().each([&](auto entity){

        Registry::Get().get<ObjectPropertiesComponent>(entity).CallUpdateFunctions(m_DeltaTime);

    });

  
    auto view = Registry::Get().view<Movable,Drawable>();
    for(auto entity : view){
        auto& transform = view.get<Movable>(entity);
        auto& drawable = view.get<Drawable>(entity);

        if(!drawable.ReadyToDraw()){
            continue;
        }

        Shader& currentObjectShader = *m_CreatedShaders[drawable.m_ShaderName].get();

        currentObjectShader.Bind();
        currentObjectShader.SetUniformMat4f("MVP", GetCurrentCamera().GetViewProjection(*this)*transform.GetModelMatrix());

        //GuiLayer::m_RaycastTexture.get()->GetAttachedTexture().Bind();
        //GL_CALL(glBindImageTexture(3,GuiLayer::m_RaycastTexture.get()->GetAttachedTexture().GetID(),0,GL_FALSE,0,GL_WRITE_ONLY,GL_RGBA32F));
        currentObjectShader.SetUniform1i("rayCastTexture",3);

        uint32_t val = (uint32_t)entity;
        val++;

        float r = ((uint32_t)val & 0x000000FF) >>  0;
        float g = ((uint32_t)val & 0x0000FF00) >>  8;
        float b = ((uint32_t)val & 0x00FF0000) >>  16;
        
        currentObjectShader.SetUniform3f("UMyIdentifier",r/255.0f,g/255.0f,b/255.0f);
    
        drawable.m_PreDrawFuncs.EmitEvent(drawable,currentObjectShader);

        drawable.Draw();

        drawable.m_PostDrawFuncs.EmitEvent(drawable);

    }

    
    
    m_PostDrawingLoopFuncs.EmitEvent(*this);

    EndDrawState();

    if(!m_IsOpen){
        m_ClosingCallbackFuncs.EmitEvent(*this);
    }
    

}



Window* Window::GetWindow(GLFWwindow* win) {
    if(Window::m_CurrentWindows.find(win) != Window::m_CurrentWindows.end()){
        return Window::m_CurrentWindows[win];
    }
    return nullptr;
}

VertexArray& WindowCreators::NewVertexArray() {
    VertexArray& vertex = *m_Master.m_CreatedVertexArrays.emplace_back(std::make_unique<VertexArray>()).get();
    return vertex;
}

Shader& WindowCreators::CachedShader(std::string shaderRelativePath, bool* loadResult) {
    if(m_Master.m_CreatedShaders.find(shaderRelativePath) != m_Master.m_CreatedShaders.end()){
        Shader& shader = *m_Master.m_CreatedShaders[shaderRelativePath].get();
        if(loadResult){
            *loadResult=true;
        }
        return shader;
    }
    
    m_Master.m_CreatedShaders[shaderRelativePath] = std::make_unique<Shader>();
    Shader& shader = *m_Master.m_CreatedShaders[shaderRelativePath].get();

    if(!std::filesystem::exists(shaderRelativePath)){
        LOG("Could not load shader at path " + std::filesystem::absolute(shaderRelativePath).string() + " passing empty shader");
        if(loadResult){
            *loadResult=false;
        }
        return shader;
    }

    std::vector<std::pair<ShaderType,std::string>> sources;
    for(auto file : std::filesystem::directory_iterator(shaderRelativePath)){
        std::string fileName = file.path().filename().string();

        if(fileName.ends_with("vert")){
            std::string source = LoadFileContents(std::filesystem::absolute(shaderRelativePath + "/" + fileName).string());
            sources.push_back(std::make_pair(ShaderType::Vertex,source));
        }
        if(fileName.ends_with("frag")){
            std::string source = LoadFileContents(std::filesystem::absolute(shaderRelativePath + "/" + fileName).string());
            sources.push_back(std::make_pair(ShaderType::Fragment,source));
        }
    }
    

    ShaderCreationProperties prop = m_Master.m_CreatedShaders[shaderRelativePath].get()->CreateNew();
    for(auto& [type,source] : sources){
        prop.AddShader(type,source);
    }

    if(!prop.Generate()){
        if(loadResult){
            *loadResult=false;
        }
        return shader;
    }

    if(loadResult){
        *loadResult=true;
    }
    return shader;

}

WindowCreators Window::Create() {
    return WindowCreators(*this);
}





FunctionSink<void(Window&,MouseEventProperties)> WindowEvents::MouseLeftWindowEvent() {
    return FunctionSink<void(Window&,MouseEventProperties)>(m_Master.m_MouseLeftWindowFuncs);
}



FunctionSink<void(Window&,MouseEventProperties)> WindowEvents::MouseEnteredWindowEvent() {
    return FunctionSink<void(Window&,MouseEventProperties)>(m_Master.m_MouseEnteredWindowFuncs);
}

FunctionSink<void(Window&,MouseButtonEventProperties)> WindowEvents::MouseButtonEvent() {
    return FunctionSink<void(Window&,MouseButtonEventProperties)>(m_Master.m_MouseButtonFuncs);
}

FunctionSink<void(Window&,MouseScrollEventProperties)> WindowEvents::MouseScrollEvent() {
    return FunctionSink<void(Window&,MouseScrollEventProperties)>(m_Master.m_MouseScrollFuncs);
}

FunctionSink<void(Window&,WindowResizedEventProperties)> WindowEvents::ResizedEvent() {
    return FunctionSink<void(Window&,WindowResizedEventProperties)>(m_Master.m_WindowResizedEventFuncs);
}

FunctionSink<void(Window&,KeyEventProperties)> WindowEvents::KeyEvent() {
    return FunctionSink<void(Window&,KeyEventProperties)>(m_Master.m_KeyEventFuncs);
}

FunctionSink<void(Window&)> Window::WindowCreationEvent() {
    return FunctionSink<void(Window&)>(Window::m_StartWindowFuncs);
}

FunctionSink<void(Window&)> WindowEvents::PreDrawingLoopEvent() {
    return FunctionSink<void(Window&)>(m_Master.m_PreDrawingLoopFuncs);
}

FunctionSink<void(Window&)> WindowEvents::PostDrawingLoopEvent() {
    return FunctionSink<void(Window&)>(m_Master.m_PostDrawingLoopFuncs);
}
FunctionSink<void(Window&)> WindowEvents::ClosingEvent() {
    return FunctionSink<void(Window&)>(m_Master.m_ClosingCallbackFuncs);
}


RayCastHit Window::RayCast(glm::vec2 screenPos) {
    return GuiLayer::RayCast(ImVec2(screenPos.x,screenPos.y));    
}

float Window::GetDeltaTime() {
    return m_DeltaTime;
}