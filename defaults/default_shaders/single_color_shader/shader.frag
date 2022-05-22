#version 430 core

out vec4 mainColor;
uniform vec3 desiredColor;


void main(){

    mainColor = vec4(desiredColor,1.0f);

}