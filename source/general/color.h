#pragma once
#include <functional>
#include "../../vendor/glm/glm/glm.hpp"
#include "../../vendor/imgui/imgui.h"


class Color {
public:
    
    Color(unsigned char r,unsigned char g,unsigned char b,unsigned char a = 255);
    Color();

    /**
     * 
     * @return the color as floats from 0 to 1
     */
    glm::vec4& Normalized();
    /**
     * 
     * @return the color as integers from 0 to 255
     */
    glm::ivec4 AsIntegers() const;

    ImVec4 AsImVec4();
    
    Color& operator+= (const Color& other);
    Color& operator-= (const Color& other);

    Color operator+ (const Color& other);

    Color operator- (const Color& color);

    /**
    * 
     * Sets the color from rgb values normalized
     */
    void Set(float r,float g,float b,float a= 1);
    /**
     * Sets the color from rgb values from 0 to 255
     */
    void Set(unsigned char r,unsigned char g,unsigned char b,unsigned char a = 255);
    
    /**
     * A function to modify the color value as needed
     * @param  changeFunction -> a lambda or function to use for changing (take a glm::vec3 as parameter for the color(0-1))
     */
    void Modify(std::function<void(glm::vec4&)> changeFunction);
    
    static Color White;
    static Color Black;
    static Color Red;
    static Color Green;
    static Color Blue;
    static Color FromFloat(float r, float g, float b, float a = 1);
    

private:
    glm::vec4 m_ColorValue = glm::vec4(0,0,0,1);


};