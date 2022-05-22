#pragma once
#include "../includes.h"


class Type2D {
private:
    int dummy = 0;
};

class Type3D {
private:
    int dummy = 0;
};


template<typename TextureType>
class Texture {

public:
    Texture(std::function<void(Texture&)> initializerFunc) {
        if (!m_ID) {
            auto deleter = [](unsigned int* id) {
                GL_CALL(glDeleteTextures(1, id));
            };
            m_ID = std::shared_ptr<unsigned int>(new unsigned int, deleter);
            GL_CALL(glGenTextures(1, m_ID.get()));
        }
        FigureOutType();
        this->Bind();
        initializerFunc(*this);
        this->Unbind();
    }

    Texture() {
        FigureOutType();
    }

    operator bool() {
        return m_ID.operator bool();
    }

    GLenum GetType() {
        return m_TextureType;
    }

    void Bind() {
        if (m_ID) {
            GL_CALL(glBindTexture(m_TextureType, *m_ID.get()));
        }
        
    }
    void Unbind() {
        GL_CALL(glBindTexture(m_TextureType, 0));
    }
    const unsigned int GetID() {
        return *m_ID.get();
    }

private:
    void FigureOutType() {
        
        if (entt::type_hash<TextureType>() == entt::type_hash<Type2D>()) {
            m_TextureType = GL_TEXTURE_2D;
        }
        if (entt::type_hash<TextureType>() == entt::type_hash<Type3D>()) {
            m_TextureType = GL_TEXTURE_3D;
        }
    };

    GLenum m_TextureType;
    std::shared_ptr<unsigned int> m_ID;
    

};