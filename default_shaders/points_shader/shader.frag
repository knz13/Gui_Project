#version 430 core

out vec4 mainColor;
uniform vec3 pointColor;


void main(){

    mainColor = vec4(pointColor,1.0f);

}