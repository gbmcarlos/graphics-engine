#include "RenderCommand.h"

namespace engine {

    void RenderCommand::init() {
        getApi().init();
    }


    void RenderCommand::clear(const glm::vec4& color) {
        getApi().setClearColor(color.r, color.g, color.b, color.a);
        getApi().clear();
    }


    void RenderCommand::createVertexBuffer(unsigned int& id, unsigned int size) {
        getApi().createVertexBuffer(id, size);
    }

    void RenderCommand::createVertexBuffer(unsigned int& id, const void *data, unsigned int size) {
        getApi().createVertexBuffer(id, data, size);
    }

    void RenderCommand::bindVertexBuffer(unsigned int& id) {
        getApi().bindVertexBuffer(id);
    }

    void RenderCommand::submitVertexBufferData(const void *data, unsigned int size) {
        getApi().submitVertexBufferData(data, size);
    }

    void RenderCommand::unbindVertexBuffer() {
        getApi().unbindVertexBuffer();
    }


    void RenderCommand::createIndexBuffer(unsigned int& id, unsigned int* data, unsigned int count) {
        getApi().createIndexBuffer(id, data, count);
    }

    void RenderCommand::bindIndexBuffer(unsigned int& id) {
        getApi().bindIndexBuffer(id);
    }

    void RenderCommand::unbindIndexBuffer() {
        getApi().unbindIndexBuffer();
    }


    void RenderCommand::deleteBuffer(unsigned int& id) {
        getApi().deleteBuffer(id);
    }


    void RenderCommand::createVertexArray(unsigned int& id) {
        getApi().createVertexArray(id);
    }

    void RenderCommand::bindVertexArray(unsigned int& id) {
        getApi().bindVertexArray(id);
    }

    void RenderCommand::unbindVertexArray() {
        getApi().unbindVertexArray();
    }

    void RenderCommand::deleteVertexArray(unsigned int& id) {
        getApi().deleteVertexArray(id);
    }


    unsigned int RenderCommand::createShaderProgram() {
        return (unsigned int) getApi().createShaderProgram();
    }

    unsigned int RenderCommand::compileShader(unsigned int type, const std::string &source) {
        return (unsigned int) getApi().compileShader(type, source);
    }

    void RenderCommand::attachShader(unsigned int id, unsigned int shaderId) {
        getApi().attachShader(id, shaderId);
    }

    void RenderCommand::compileShaderProgram(unsigned int id) {
        getApi().compileShaderProgram(id);
    }

    void RenderCommand::bindShader(unsigned int id) {
        getApi().bindShader(id);
    }

    void RenderCommand::deleteShader(unsigned int id) {
        getApi().deleteShader(id);
    }


    int RenderCommand::getUniformLocation(unsigned int id, const std::string& name) {
        return (int) getApi().getUniformLocation(id, name);
    }

    void RenderCommand::setUniform3f(int location, const glm::vec3& value) {
        getApi().setUniform3f(location, value);
    }

    void RenderCommand::setUniform4f(int location, const glm::vec4& value) {
        getApi().setUniform4f(location, value);
    }

    void RenderCommand::setUniformMat3f(int location, const glm::mat3& value) {
        getApi().setUniformMat3f(location, value);
    }

    void RenderCommand::setUniformMat4f(int location, const glm::mat4& value) {
        getApi().setUniformMat4f(location, value);
    }


    void RenderCommand::addVertexArrayAttribute(unsigned int index, int count, GLenum type, bool normalized, int stride, int offset) {
        getApi().addVertexArrayAttribute(index, count, type, normalized, stride, offset);
    }

    void RenderCommand::drawIndexedTriangles(const std::shared_ptr<VertexArray>& vertexArray) {
        getApi().drawIndexedTriangles(vertexArray);
    }

}