#pragma once
#include "../vendor/glm/glm/glm.hpp"
#include "../vendor/glm/glm/gtc/quaternion.hpp"
#include "../vendor/glm/glm/gtx/quaternion.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "../vendor/glm/glm/gtx/matrix_decompose.hpp"

namespace Math {

    bool DecomposeMatrix(const glm::mat4& transform,glm::vec3& translation,glm::vec3& rotation,glm::vec3& scale);
    glm::mat4 CalculateModelMatrix(const glm::vec3* position,const glm::vec3* rotation,const glm::vec3* scale,glm::mat4* outPos=nullptr,glm::mat4* outRot=nullptr,glm::mat4* outScale=nullptr);


};