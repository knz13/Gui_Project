#include "texture.h"



void Texture::Bind() {
    if(m_ID){
        GL_CALL(glBindTexture(m_TextureType,*m_ID.get()));
    }
}

void Texture::Unbind() {
    GL_CALL(glBindTexture(m_TextureType,0));
}

const unsigned int Texture::GetID() {
    return *m_ID.get();    
}


Texture::Texture(std::function<void(Texture&)> initializerFunc,GLenum textureType) : m_TextureType(textureType) {
    if(!m_ID){
        auto deleter = [](unsigned int* id){
            GL_CALL(glDeleteTextures(1,id));
        };
        m_ID = std::shared_ptr<unsigned int>(new unsigned int,deleter);
        GL_CALL(glGenTextures(1,m_ID.get()));
    }
    this->Bind();
    initializerFunc(*this);
    this->Unbind();
}