#pragma once

#include <GL/glew.h>

namespace engine {

    class OpenGLRenderApi {

    private:
        OpenGLRenderApi() {};

    public:
        static OpenGLRenderApi& getInstance(){
            static OpenGLRenderApi instance;
            return instance;
        }
        OpenGLRenderApi(OpenGLRenderApi const&) = delete;
        void operator=(OpenGLRenderApi const&)  = delete;

    public:
        void init();

        void setClearColor(float red, float green, float blue, float alpha);
        void clear();

        void createVertexBuffer(unsigned int& id, unsigned int size);
        void createVertexBuffer(unsigned int& id, const void *data, unsigned int size);
        void bindVertexBuffer(unsigned int& id);
        void submitVertexBufferData(const void *data, unsigned int size);
        void unbindVertexBuffer();

        void createIndexBuffer(unsigned int& id, unsigned int* data, unsigned int count);
        void bindIndexBuffer(unsigned int& id);
        void unbindIndexBuffer();

        void deleteBuffer(unsigned int& id);

        void createVertexArray(unsigned int& id);
        void bindVertexArray(unsigned int& id);
        void unbindVertexArray();
        void deleteVertexArray(unsigned int& id);

        void addVertexArrayAttribute(unsigned int index, int count, GLenum type, bool normalized, int stride, int offset);

    };

}