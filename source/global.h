#pragma once
#include <iostream>
#include <fstream>
#include <future>
#include <optional>
#include <random>
#include <memory>
#include <unordered_map>
#include <filesystem>
#include <map>
#include <concepts>
#include "../vendor/glm/glm/glm.hpp"
#include "../vendor/glm/glm/gtc/quaternion.hpp"
#include "../vendor/glm/glm/gtx/quaternion.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "../vendor/glm/glm/gtx/matrix_decompose.hpp"

#define GLEW_STATIC
#include "../vendor/glew/include/GL/glew.h"
#include "../vendor/glfw/include/GLFW/glfw3.h"
#include "../vendor/glm/glm/gtc/type_ptr.hpp"
#include "general/structures.h"
#include "general/color.h"
#include "../vendor/assimp/include/assimp/Importer.hpp"
#include "../vendor/assimp/include/assimp/scene.h"
#include "../vendor/assimp/include/assimp/postprocess.h"
#include "../vendor/assimp/include/assimp/matrix4x4.h"
#include "../vendor/assimp/include/assimp/cimport.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "gui_layer/gui_useful_implementations.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "../vendor/imguizmo/ImGuizmo.h"
#include "object/registry.h"
#include "components/add_to_every_object.h"
#include "general/math.h"
#include "general/helpers.h"
#include "components/component.h"
#include "object/object.h"
#include "components/add_only_to.h"
#include "../vendor/react_physics/include/reactphysics3d/reactphysics3d.h"
using namespace std;






static std::string LoadFileContents(std::string fileName) {
    std::string fileContents = "";

    ifstream stream(fileName);

    std::string line;
    while(getline(stream,line)){
        fileContents += line;
        fileContents += "\n";
    }

    return fileContents;
    


}