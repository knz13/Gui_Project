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
#include "../vendor/glfw/include/GLFW/glfw3.h"
#include "../vendor/thread_pool/thread_pool.hpp"
#include "../vendor/glm/glm/gtc/type_ptr.hpp"
#include "../vendor/assimp/include/assimp/Importer.hpp"
#include "../vendor/assimp/include/assimp/scene.h"
#include "../vendor/assimp/include/assimp/postprocess.h"
#include "../vendor/assimp/include/assimp/matrix4x4.h"
#include "../vendor/assimp/include/assimp/cimport.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "../vendor/ryml/include/ryml.hpp"
#include "../vendor/ryml/include/ryml_std.hpp"
#include "../vendor/markdown/imgui_markdown.h"

using namespace std;


#define GL_SIZEOF(x) TestSize(x)
#define KV_CLASS private: \
                    friend class Registry; friend class Window; friend class Object; friend class Camera;
#define KV_DRAWING_MODE private: friend class Mesh;
#define KV_REGISTER_CLASS_AS_COMPONENT(className) void* __UNUSEDVARIABLE##className = [](){ Object::RegisterClassAsComponent<className>();return nullptr;}()

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


#define LOG(x) cout << "LOG: " << x << endl <<  "At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl


#ifdef NDEBUG
#define DEBUG_LOG(x)
#define DEBUG_WARN(x)
#define DEBUG_ERROR(x)
#else
#define DEBUG_LOG(x) cout << "LOG: " << x << endl <<  "At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl
#define DEBUG_WARN(x) cout << "WARNING: " << x << endl <<  "At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl
#define DEBUG_ERROR(x) cout << "ERROR! -> " << x  << endl <<  "At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl; __debugbreak()
#endif

static void ClearGLErrors() {
    while (glGetError() != GL_NO_ERROR) {
        ;
    }
}

static bool GetGLError(int line, std::string file) {
    GLenum code = glGetError();
    if (code != GL_NO_ERROR) {
        cout << "OpenGL error '" << gluErrorString(code) << "' \nAt line: " << line << " \nIn file: " << file << endl;
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
