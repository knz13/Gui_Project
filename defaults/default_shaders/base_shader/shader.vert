#version 430 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent;

out vec3 u_Identifier;
out vec2 u_Viewport;


uniform vec3 UMyIdentifier;
uniform mat4 MVP;

void main(){
    
    //imageStore(rayCastTexture,ivec2(100,100),vec4(200,200,200,1));
    u_Identifier = UMyIdentifier;
    gl_Position = MVP * vec4(pos,1.0);
}