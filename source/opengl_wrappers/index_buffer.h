#pragma once
#include "../global.h"


class VertexArray;
class IndexBuffer {
public:

    IndexBuffer(VertexArray& master);

    void SetIndices(std::vector<unsigned int> indices);


    void Bind();
    void Unbind();

private:
    VertexArray& m_Master;
    unsigned int m_Count;
    std::shared_ptr<unsigned int> m_ID;

    friend class VertexArray;
};