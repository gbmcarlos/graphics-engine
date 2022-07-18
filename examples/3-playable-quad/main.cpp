#include <core/Core.h>
#include <graphics/Graphics.h>

#include <graphics/mesh/2d/samples/TriangleMesh.h>
#include <graphics/mesh/2d/samples/SquareMesh.h>

#include <array>
#include <iostream>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

class PlayableQuadApplication : public engine::Application {

private:

    std::shared_ptr<engine::Shader> m_shader;

    std::shared_ptr<engine::OrthographicCamera> m_camera;
    glm::vec2 m_cameraPosition = {0.0f, 0.0f};

    engine::SquareMesh m_quadMesh;
    engine::TriangleMesh m_triangleMesh;

    // Triangle properties
    glm::vec2 m_trianglePosition = {-3.0, -2.0f};
    float m_triangleSize = 2.0f;
    glm::mat4 m_triangleTransform;

    // Quad properties
    glm::vec2 m_quad1Position = {0.0f, 0.0f};
    float m_quadSize = 3.0f;
    float m_quadTargetSpeed = m_quadSize * 3;
    glm::vec2 m_quad1Speed = {0.0f, 0.0f};


public:

    PlayableQuadApplication(int viewportWidth, int viewportHeight) {
        m_camera = std::make_shared<engine::OrthographicCamera>(viewportWidth, viewportHeight, 100);
    }

    void onReady() override {

        m_shader = std::make_shared<engine::Shader>();
        m_shader->attach(GL_VERTEX_SHADER, "res/shaders/vertex-position.glsl");
        m_shader->attach(GL_FRAGMENT_SHADER, "res/shaders/fragment-color.glsl");
        m_shader->compile();

        // Prepare the transform matrix of the quad
        m_triangleTransform =
                glm::translate(glm::mat4(1.0f), glm::vec3(m_trianglePosition, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), {0, 0, 1}) *
                glm::scale(glm::mat4(1.0f), {m_triangleSize, m_triangleSize, 1.0f})
        ;

    }

    void onUpdate(engine::TimeStep timeStep) override {

        // Update the position of the quad based on its speed
        m_quad1Position.x += m_quad1Speed.x * timeStep;
        m_quad1Position.y += m_quad1Speed.y * timeStep;

        // Prepare the transform matrix of the quad
        glm::mat4 quadTransform =
            glm::translate(glm::mat4(1.0f), glm::vec3(m_quad1Position, 0.0f)) *
            glm::scale(glm::mat4(1.0f), {m_quadSize, m_quadSize, 1.0f})
        ;

        engine::RenderCommand::clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

        engine::Renderer::beginScene(m_camera);
        engine::Renderer::beginBatch(m_shader, engine::MeshElementType::Face);

        engine::Renderer::submit(m_quadMesh, quadTransform);
        engine::Renderer::submit(m_triangleMesh, m_triangleTransform);

        engine::Renderer::flushBatch();
        engine::Renderer::endScene();

    }

    void onGuiRender() override {}

    void onEvent(const engine::Event& event) override {

        engine::EventDispatcher dispatcher(event);

        dispatcher.dispatch<engine::WindowResizeEvent>(GE_BIND_CALLBACK_FN(PlayableQuadApplication::onWindowResizeEvent));
        dispatcher.dispatch<engine::KeyPressEvent>(GE_BIND_CALLBACK_FN(PlayableQuadApplication::onKeyPressEvent));
        dispatcher.dispatch<engine::KeyReleaseEvent>(GE_BIND_CALLBACK_FN(PlayableQuadApplication::onKeyReleaseEvent));
        dispatcher.dispatch<engine::MouseButtonPress>(GE_BIND_CALLBACK_FN(PlayableQuadApplication::onMouseButtonPressEvent));
        dispatcher.dispatch<engine::MouseButtonRelease>(GE_BIND_CALLBACK_FN(PlayableQuadApplication::onMouseButtonReleaseEvent));
        dispatcher.dispatch<engine::CursorMoveEvent>(GE_BIND_CALLBACK_FN(PlayableQuadApplication::onCursorMoveEvent));
        dispatcher.dispatch<engine::ScrollEvent>(GE_BIND_CALLBACK_FN(PlayableQuadApplication::onScrollEvent));

    }

    void onMouseButtonPressEvent(engine::MouseButtonPress& event) {
        std::cout << "mouse button pressed, button:  " << event.getButton() << std::endl;
    }

    void onMouseButtonReleaseEvent(engine::MouseButtonRelease& event) {
        std::cout << "mouse button pressed, button:  " << event.getButton() << std::endl;
    }

    void onCursorMoveEvent(engine::CursorMoveEvent& event) {
        std::cout << "cursor move, x: " << event.getXPosition() << ", y: " << event.getYPosition() << std::endl;
    }

    void onScrollEvent(engine::ScrollEvent& event) {
        std::cout << "scroll, x: " << event.getXOffset() << ", y: " << event.getXOffset() << std::endl;
    }

    void onWindowResizeEvent(engine::WindowResizeEvent& event) {
        m_camera->onWindowResize(event);
    }

    void onKeyPressEvent(engine::KeyPressEvent& event) {

        if (event.getKey() == GLFW_KEY_UP) {
            m_camera->addZoomLevel(0.1f);
            return;
        } else if (event.getKey() == GLFW_KEY_DOWN) {
            m_camera->addZoomLevel(-0.1f);
            return;
        }

        if (event.getKey() == GLFW_KEY_RIGHT) {
            m_quad1Speed.x = m_quadTargetSpeed;
            return;
        } else if (event.getKey() == GLFW_KEY_LEFT) {
            m_quad1Speed.x = -m_quadTargetSpeed;
            return;
        }

    }

    void onKeyReleaseEvent(engine::KeyReleaseEvent& event) {

        if ((event.getKey() == GLFW_KEY_RIGHT && m_quad1Speed.x > 0) || (event.getKey() == GLFW_KEY_LEFT && m_quad1Speed.x < 0) ) {
            m_quad1Speed.x = 0;
            return;
        }

        if ((event.getKey() == GLFW_KEY_UP && m_quad1Speed.y > 0) || (event.getKey() == GLFW_KEY_DOWN && m_quad1Speed.y < 0) ) {
            m_quad1Speed.y = 0;
            return;
        }

    }

};

int main() {

    engine::Window window("Playable Quad");
    engine::RunLoop runLoop(window);

    PlayableQuadApplication app(window.getViewportWidth(), window.getViewportHeight());

    runLoop.run(app);

    return 0;

}