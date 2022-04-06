#pragma once
#include "../global.h"


class Texture {

public:
    Texture(std::function<void(Texture&)> initializerFunc);
    void Bind();
    void Unbind();
    const unsigned int GetID();

private:
    std::shared_ptr<unsigned int> m_ID;
    

};