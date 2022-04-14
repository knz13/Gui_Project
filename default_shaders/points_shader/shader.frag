#version 430 core

out vec4 mainColor;
uniform vec3 pointColor;


void main(){

    vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
    
    if (dot(circCoord, circCoord) > 1.0) {
        discard;
    }
    mainColor = vec4(pointColor,1.0f);

}