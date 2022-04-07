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

    
		
    // rightHandSide is the right hand side of the equation.
    glm::vec4 RightHandSide;
    RightHandSide[0] = LocalMatrix[0][3];
    RightHandSide[1] = LocalMatrix[1][3];
    RightHandSide[2] = LocalMatrix[2][3];
    RightHandSide[3] = LocalMatrix[3][3];

    // Clear the perspective partition
    LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
    LocalMatrix[3][3] = static_cast<T>(1);
        
    translation = glm::vec3(LocalMatrix[3]);
    LocalMatrix[3] = glm::vec4(0, 0, 0, LocalMatrix[3].w);

    scale.x = glm::length(LocalMatrix[0]);
    scale.y = glm::length(LocalMatrix[1]);
    scale.z = glm::length(LocalMatrix[2]);

    vec3 Row[3], Pdum3;
    glm::vec3 Skew;

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
