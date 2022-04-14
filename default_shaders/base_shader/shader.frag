#version 430 core

layout(rgba32f,binding=3) uniform image2D rayCastTexture;

in vec3 u_Identifier;

out vec4 mainColor;

void main() {

    

    imageStore(rayCastTexture,ivec2(gl_FragCoord.x,gl_FragCoord.y),vec4(u_Identifier.x,u_Identifier.y,u_Identifier.z,1));

    mainColor = vec4(1,1,1,1);

}