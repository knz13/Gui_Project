#pragma once
#include "../global.h"
#include "texture.h"

class Framebuffer {

public:
    Framebuffer(float sizeX,float sizeY);
    void Bind();
    void Unbind();
    bool Status();
    void Clear(Color color = Color::Black);
    
    Texture& GetAttachedTexture();
    const glm::vec2& GetSize();


private:
    glm::vec2 m_Size;
    std::shared_ptr<unsigned int> m_ID;
    std::shared_ptr<Texture> m_AttachedTexture;
    std::shared_ptr<unsigned int> m_RenderBufferID;

};


