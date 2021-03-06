#include "OpenGLRenderAPI.h"
#include "opengl_utils.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace engine {

    void OpenGLRenderApi::init() {

        // Set up OpenGL, with GLEW
        if (glewInit() != GLEW_OK) {
            std::cout << "GLEW initialization failed" << std::endl;
        }

        GLint flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }

        std::cout << "OpenGL Specs:" << std::endl;
        std::cout << "openGL version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    }

    void OpenGLRenderApi::setClearColor(float red, float green, float blue, float alpha) {
        glCall(glClearColor(red, green, blue, alpha));
    }

    void OpenGLRenderApi::clear() {
        glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void OpenGLRenderApi::createVertexBuffer(unsigned int& id, unsigned int size) {
        glCall(glGenBuffers(1, &id));
        glCall(glBindBuffer(GL_ARRAY_BUFFER, id));
        glCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    }

    void OpenGLRenderApi::createVertexBuffer(unsigned int& id, const void *data, unsigned int size) {
        glCall(glGenBuffers(1, &id));
        glCall(glBindBuffer(GL_ARRAY_BUFFER, id));
        glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }

    void OpenGLRenderApi::bindVertexBuffer(unsigned int& id) {
        glCall(glBindBuffer(GL_ARRAY_BUFFER, id));
    }


    void OpenGLRenderApi::submitVertexBufferData(const void *data, unsigned int size) {
        glCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
    }

    void OpenGLRenderApi::unbindVertexBuffer() {
        glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    void OpenGLRenderApi::createIndexBuffer(unsigned int& id, unsigned int *data, unsigned int count) {
        glCall(glGenBuffers(1, &id));
        glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
        glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
    }

    void OpenGLRenderApi::bindIndexBuffer(unsigned int& id) {
        glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
    }

    void OpenGLRenderApi::unbindIndexBuffer() {
        glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    void OpenGLRenderApi::deleteBuffer(unsigned int& id) {
        glCall(glDeleteBuffers(1, &id));
    }

    void OpenGLRenderApi::createVertexArray(unsigned int& id) {
        glCall(glGenVertexArrays(1, &id));
    }

    void OpenGLRenderApi::bindVertexArray(unsigned int& id) {
        glCall(glBindVertexArray(id));
    }

    void OpenGLRenderApi::unbindVertexArray() {
        glCall(glBindVertexArray(0));
    }

    void OpenGLRenderApi::deleteVertexArray(unsigned int& id) {
        glCall(glDeleteVertexArrays(1, &id));
    }

    unsigned int OpenGLRenderApi::createShaderProgram() {
        return (unsigned int) glCreateProgram();
    }

    unsigned int OpenGLRenderApi::compileShader(ShaderType type, const std::string& source) {

        unsigned int id = glCreateShader(convertShaderType(type));
        const char* src = source.c_str();
        glCall(glShaderSource(id, 1, &src, nullptr));
        glCall(glCompileShader(id));

        int result;
        glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
        if (result == GL_FALSE) {
            int length;
            glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
            char* message = (char*)alloca(length * sizeof(char));
            glCall(glGetShaderInfoLog(id, length, &length, message));
            std::cout << "Failed to compile " << (type == ShaderType::Vertex ? "vertex" : "fragment") << " m_shader. Error:" << std::endl;
            std::cout << message << std::endl;
            glCall(glDeleteShader(id));
            return 0;
        }

        return (unsigned int) id;

    }

    void OpenGLRenderApi::deleteShader(unsigned int id) {
        glCall(glDeleteShader(id));
    }

    void OpenGLRenderApi::attachShader(unsigned int id, unsigned int shaderId) {
        glCall(glAttachShader(id, shaderId));
    }

    void OpenGLRenderApi::compileShaderProgram(unsigned int id) {
        glCall(glLinkProgram(id));
        glCall(glValidateProgram(id));
    }

    void OpenGLRenderApi::bindShader(unsigned int id) {
        glCall(glUseProgram(id));
    }

    unsigned int OpenGLRenderApi::getUniformLocation(unsigned int id, const std::string& name) {
        return (unsigned int) glGetUniformLocation(id, name.c_str());
    }

    void OpenGLRenderApi::setUniform1i(int location, int value) {
        glCall(glUniform1i(location, value));
    }

    void OpenGLRenderApi::setUniform3f(int location, const glm::vec3 &value) {
        glCall(glUniform3f(location, value.x, value.y, value.z));
    }

    void OpenGLRenderApi::setUniform4f(int location, const glm::vec4 &value) {
        glCall(glUniform4f(location, value.x, value.y, value.z, value.w));
    }

    void OpenGLRenderApi::setUniformMat3f(int location, const glm::mat3 &value) {
        glCall(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value)));
    }

    void OpenGLRenderApi::setUniformMat4f(int location, const glm::mat4 &value) {
        glCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
    }

    unsigned int OpenGLRenderApi::sizeOfVertexBufferLayoutElementType(VertexBufferLayoutElementType type) {
        return sizeof(convertVertexBufferLayoutElementType(type));
    }

    void OpenGLRenderApi::addVertexArrayAttribute(unsigned int index, int count, VertexBufferLayoutElementType type, bool normalized, int stride, int offset) {

        glCall(glEnableVertexAttribArray(index));
        glCall(glVertexAttribPointer(
            index,
            count,
            convertVertexBufferLayoutElementType(type),
                normalized ? GL_TRUE : GL_FALSE,
            stride,
            (const void*) offset
        ));
    }

    void OpenGLRenderApi::loadTexture(unsigned int& id, unsigned int width, unsigned int height, void* data) {

        glCall(glGenTextures(1, &id));
        glCall(glBindTexture(GL_TEXTURE_2D, id));

        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
        glCall(glGenerateMipmap(GL_TEXTURE_2D));

    }

    void OpenGLRenderApi::bindTexture(unsigned int id, unsigned int slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glCall(glBindTexture(GL_TEXTURE_2D, id));
    }

    void OpenGLRenderApi::deleteTexture(unsigned int& id) {
        glCall(glDeleteTextures(1, &id));
    }

    void OpenGLRenderApi::drawIndexedTriangles(unsigned int indexCount) {
        glCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr));
    }

    void OpenGLRenderApi::drawIndexedLines(unsigned int indexCount) {
        glCall(glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, nullptr));
    }


    GLenum OpenGLRenderApi::convertVertexBufferLayoutElementType(VertexBufferLayoutElementType type) {

        switch (type) {
            case VertexBufferLayoutElementType::Bool:       return GL_BOOL;
            case VertexBufferLayoutElementType::Int:        return GL_INT;
            case VertexBufferLayoutElementType::Float:      return GL_FLOAT;
        }

        throw std::runtime_error("Unknown layout element type");

    }

    GLenum OpenGLRenderApi::convertShaderType(ShaderType type) {

        switch (type) {
            case ShaderType::Vertex:    return GL_VERTEX_SHADER;
            case ShaderType::Geometry:    return GL_GEOMETRY_SHADER;
            case ShaderType::Fragment:    return GL_FRAGMENT_SHADER;
        }

        throw std::runtime_error("Unknown shader type");

    }

}