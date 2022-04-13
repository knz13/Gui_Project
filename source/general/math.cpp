#include "math.h"


bool Math::DecomposeMatrix(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale) {
    
    using namespace glm;
    using T = float;
    
    glm::mat4 LocalMatrix(transform);

    if(epsilonEqual(LocalMatrix[3][3], static_cast<T>(0), epsilon<T>()))
			return false;
    if(
			epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
    {
    // Clear the perspective partition
        LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
        LocalMatrix[3][3] = static_cast<T>(1);
    }
        
    translation = glm::vec3(LocalMatrix[3]);
    LocalMatrix[3] = glm::vec4(0, 0, 0, LocalMatrix[3].w);


    vec3 Row[3], Pdum3;
    

    // Now get scale and shear.
    for(length_t i = 0; i < 3; ++i)
    for(length_t j = 0; j < 3; ++j)
        Row[i][j] = LocalMatrix[i][j];

    
    scale.x = length(Row[0]);// v3Length(Row[0]);
    Row[0] = detail::scale(Row[0], static_cast<T>(1));
    scale.y = length(Row[1]);
    Row[1] = detail::scale(Row[1], static_cast<T>(1));
    scale.z = length(Row[2]);
    Row[2] = detail::scale(Row[2], static_cast<T>(1));
    

    

    rotation.y = asin(-Row[0][2]);
    if (cos(rotation.y) != 0) {
        rotation.x = atan2(Row[1][2], Row[2][2]);
        rotation.z = atan2(Row[0][1], Row[0][0]);
    } else {
        rotation.x = atan2(-Row[2][0], Row[1][1]);
        rotation.z = 0;
    }

    return true;
}

glm::mat4 Math::CalculateModelMatrix(const glm::vec3* position, const glm::vec3* rotation, const glm::vec3* scale,glm::mat4* outPos,glm::mat4* outRot,glm::mat4* outScale) {
    
    glm::mat4 finalMatrix(1.0f);
    glm::mat4 traslateMat(1.0f);
    glm::mat4 rotateMat(1.0f);
    glm::mat4 scaleMat(1.0f);

    if(position){
        traslateMat = glm::translate(glm::mat4(1.0f),*position);
        finalMatrix *= traslateMat;
    }
    if(rotation){
        rotateMat = glm::toMat4(glm::quat(*rotation));
        finalMatrix *= rotateMat;
    }
    if(scale){
        scaleMat = glm::scale(glm::mat4(1.0f),*scale);
        finalMatrix *= scaleMat;
    }

    if(outPos){
        *outPos = traslateMat;
    }
    if(outRot){
        *outRot = rotateMat;
    }
    if(outScale){
        *outScale = scaleMat;
    }
    
    return finalMatrix;
}

bool Math::IsPointInRect(ImVec2 min, ImVec2 max, ImVec2 point) {
    if(point.x >= min.x && point.x <= max.x){
        if(point.y >= min.y && point.y <= max.y){
            return true;
        }
    }
    return false;
}
