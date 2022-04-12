#include "color.h"
#include "../global.h"



Color Color::White = Color(255,255,255);
Color Color::Black = Color(0,0,0);
Color Color::Red = Color(255,0,0);
Color Color::Green = Color(0,255,0);
Color Color::Blue = Color(0,0,255);

glm::ivec3 Color::AsIntegers() {
    return glm::ivec3(m_ColorValue*255.0f);
}

glm::vec3& Color::Normalized() {
    return m_ColorValue;
}

void Color::Set(unsigned char r, unsigned char g, unsigned char b) {
    m_ColorValue.x = r/255.0f;
    m_ColorValue.y = g/255.0f;
    m_ColorValue.z = b/255.0f;
}

void Color::Set(float r, float g, float b) {
    m_ColorValue.x = r;
    m_ColorValue.y = g;
    m_ColorValue.z = b;
}

void Color::Modify(std::function<void(glm::vec3&)> changeFunction) {
    changeFunction(m_ColorValue);
}
Color::Color(unsigned char r,unsigned char g,unsigned char b) {
    m_ColorValue.x = r/255;
    m_ColorValue.y = g/255;
    m_ColorValue.z = b/255;
}

Color::Color() {
    m_ColorValue = glm::vec3(0,0,0);
}