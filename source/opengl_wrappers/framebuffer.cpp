#include "framebuffer.h"


void Framebuffer::Bind() {
    if(m_ID){
        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER,*m_ID.get()));
        if(m_AttachedTexture){
            m_AttachedTexture.get()->Bind();
        }
    }
}

void Framebuffer::Unbind() {
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER,0));
    if(m_AttachedTexture){
            m_AttachedTexture.get()->Unbind();
    }
}

Framebuffer::Framebuffer(float sizeX,float sizeY) : m_Size(sizeX,sizeY){
    if(!m_ID){
        auto deleter = [](unsigned int* id){
            GL_CALL(glDeleteFramebuffers(1,id));
        };
        m_ID = std::shared_ptr<unsigned int>(new unsigned int,deleter);
        GL_CALL(glGenFramebuffers(1,m_ID.get()));
        
        this->Bind();
        
        m_AttachedTexture = std::shared_ptr<Texture>(new Texture([=](Texture& tex){
            tex.Bind();

            GL_CALL(glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, sizeX, sizeY));

            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));  


            tex.Unbind();
        }));


        GL_CALL(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_AttachedTexture.get()->GetID(), 0));
        
        
        if(!m_RenderBufferID){
            auto renderBufferDeleter = [](unsigned int* id){
                GL_CALL(glDeleteRenderbuffers(1,id));
            };

            m_RenderBufferID = std::shared_ptr<unsigned int>(new unsigned int,renderBufferDeleter);
            GL_CALL(glGenRenderbuffers(1,m_RenderBufferID.get()));
            GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER,*m_RenderBufferID.get()));
            GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, sizeX, sizeY));

            GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *m_RenderBufferID.get()));  
            GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER,0));
        }

        
        
        
        
        
        this->Unbind();

    }    
}

bool Framebuffer::Status() {
    this->Bind();
    bool state;
    GL_CALL_WITH_RESULT(state,glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    this->Unbind();
    return state;

}

const glm::vec2& Framebuffer::GetSize() {
    return m_Size;    
}

Texture& Framebuffer::GetAttachedTexture() {
    return *m_AttachedTexture.get();
}
