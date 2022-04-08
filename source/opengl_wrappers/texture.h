#pragma once
#include "../global.h"


class Texture {

public:
    Texture(std::function<void(Texture&)> initializerFunc,GLenum textureType=GL_TEXTURE_2D);
    void Bind();
    void Unbind();
    const unsigned int GetID();

private:
    GLenum m_TextureType;
    std::shared_ptr<unsigned int> m_ID;
    

};