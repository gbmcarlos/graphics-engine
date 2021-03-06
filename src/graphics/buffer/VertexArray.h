#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace engine {

    class VertexArray {
    private:
        unsigned int m_rendererId;
        std::shared_ptr<IndexBuffer> m_indexBuffer;

    public:
        VertexArray();
        ~VertexArray();
        void bind();
        void unbind();
        void addBuffer(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer);
        inline const std::shared_ptr<IndexBuffer>& getIndexBuffer() {return m_indexBuffer;};
    };

}