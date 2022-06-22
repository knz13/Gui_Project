#pragma once
#include <iostream>
#include <map>
#include <unordered_map>
#include <functional>
#include <filesystem>
#include <memory>
#include <map>
#include "../vendor/glm/glm/gtc/type_ptr.hpp"
#include "../vendor/glm/glm/glm.hpp"
#include "../vendor/glm/glm/gtc/quaternion.hpp"
#include "../vendor/glm/glm/gtx/quaternion.hpp"
#include "general/color.h"
#include "../vendor/entt/single_include/entt/entt.hpp"
#include "../vendor/entt/single_include/entt/entt.hpp"
#include "../vendor/glew/include/GL/glew.h"

#include "../vendor/glm/glm/gtc/type_ptr.hpp"
#include "../vendor/assimp/include/assimp/Importer.hpp"
#include "../vendor/assimp/include/assimp/scene.h"
#include "../vendor/assimp/include/assimp/postprocess.h"
#include "../vendor/assimp/include/assimp/matrix4x4.h"
#include "../vendor/assimp/include/assimp/cimport.h"
#include "imgui.h"
#include "pybind11/embed.h"
#include "imgui_internal.h"
#include "imgui/backends/imgui_impl_sdl.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "../vendor/yaml/include/yaml-cpp/yaml.h"
#include "../vendor/dialog/src/include/nfd.hpp"
#include "../vendor/ecspp/include/ecspp.h"
#include "../vendor/yael/include/yael.h"
#include "gui_layer/console_view.h"



#define GL_SIZEOF(x) TestSize(x)
#define KV_CLASS private: \
                    friend class Registry; friend class Window; friend class Camera;
#define KV_DRAWING_MODE private: friend class Mesh;


static unsigned int TestSize(unsigned int dataType) {
    switch (dataType) {
    case GL_FLOAT:
        return sizeof(float);
    case GL_UNSIGNED_INT:
        return sizeof(unsigned int);
    case GL_UNSIGNED_BYTE:
        return sizeof(unsigned char);
    case GL_BYTE:
        return sizeof(char);
    case GL_INT:
        return sizeof(int);
    default:
        return 0;
    }
}


#define LOG(x) { std::stringstream val; val << x; GuiLayer::ConsoleView::PushText(ConsoleImportance::LOG,val.str()); }


#ifdef NDEBUG
#define DEBUG_LOG(x)
#define DEBUG_WARN(x)
#define DEBUG_ERROR(x)
#else
#define DEBUG_LOG(x) LOG(x)
#define DEBUG_WARN(x) { std::stringstream val; val << x; GuiLayer::ConsoleView::PushText(ConsoleImportance::WARN,val.str()); }
#define DEBUG_ERROR(x) std::cout << "ERROR! -> " << x  << std::endl <<  "At line: "<< __LINE__ << std::endl << "In file: " << __FILE__ << std::endl; __debugbreak()
#endif

static void ClearGLErrors() {
    while (glGetError() != GL_NO_ERROR) {
        ;
    }
}

static bool GetGLError(int line, std::string file) {
    GLenum code = glGetError();
    if (code != GL_NO_ERROR) {
        std::cout << "OpenGL error '" << std::to_string(code) << "' \nAt line: " << line << " \nIn file: " << file << std::endl;
        return true;
    }
    return false;
}

#ifdef NDEBUG
#define GL_CALL(x) x 
#define GL_CALL_WITH_RESULT(var,x) var = x 
#else
#define GL_CALL(x) ClearGLErrors(); x; if(GetGLError(__LINE__,__FILE__)) {;}
#define GL_CALL_WITH_RESULT(var,x) ClearGLErrors(); var = x; if(GetGLError(__LINE__,__FILE__)) {;}
#endif
