#include "color.h"
#include "../global.h"



Color Color::White = Color(255,255,255);
Color Color::Black = Color(0,0,0);
Color Color::Red = Color(255,0,0);
Color Color::Green = Color(0,255,0);
Color Color::Blue = Color(0,0,255);

glm::ivec4 Color::AsIntegers() const{
    return glm::ivec4(m_ColorValue*255.0f);
}

ImVec4 Color::AsImVec4()
{
    return ImVec4(Normalized().x,Normalized().y,Normalized().z,Normalized().w);
}

glm::vec4& Color::Normalized() {
    return m_ColorValue;
}

void Color::Set(unsigned char r, unsigned char g, unsigned char b,unsigned char a) {
    m_ColorValue.x = r/255.0f;
    m_ColorValue.y = g/255.0f;
    m_ColorValue.z = b/255.0f;
}

void Color::Set(float r, float g, float b,float a) {
    m_ColorValue.x = r;
    m_ColorValue.y = g;
    m_ColorValue.z = b;
}

void Color::Modify(std::function<void(glm::vec4&)> changeFunction) {
    changeFunction(m_ColorValue);
}
Color Color::FromFloat(float r, float g, float b, float a)
{
    return Color(r*255,g*255,b*255,a*255);
}
Color::Color(unsigned char r,unsigned char g,unsigned char b,unsigned char a) {
    m_ColorValue.x = r/255.0f;
    m_ColorValue.y = g/255.0f;
    m_ColorValue.z = b/255.0f;
}

Color::Color() {
    m_ColorValue = glm::vec4(0,0,0,1);
}

Color& Color::operator+= (const Color& other) {
    this->m_ColorValue += other.m_ColorValue;
    return *this;
}

Color& Color::operator -= (const Color& other) {
    this->m_ColorValue -= other.m_ColorValue;
    return *this;
}

Color Color::operator+ (const Color& other) {

    Color newColor;
    newColor.m_ColorValue = other.m_ColorValue + this->m_ColorValue;

    return newColor;
}

Color Color::operator- (const Color& other) {

    Color newColor;
    newColor.m_ColorValue = this->m_ColorValue - other.m_ColorValue;

    return newColor;
}