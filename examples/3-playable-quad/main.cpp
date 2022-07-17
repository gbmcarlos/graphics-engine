#include <Core.h>

#include <array>

#include "glm/glm.hpp"

class PlayableQuadApplication : public engine::Application {

private:

    std::shared_ptr<engine::VertexArray> m_vertexArray;
    std::shared_ptr<engine::VertexBuffer> m_vertexBuffer;
    std::shared_ptr<engine::IndexBuffer> m_indexBuffer;
    std::shared_ptr<engine::Shader> m_shader;

    std::shared_ptr<engine::OrthographicCamera> m_camera;

    // Quad properties
    glm::vec4 m_quadColor = {1.0f, 1.0f, 1.0f, 1.0f};
    glm::vec2 m_quad1Position = {
            0.0f, 0.0f
    };
    float m_quadSize = 1.0f;
    float m_quadTargetSpeed = m_quadSize * 3;
    glm::vec2 m_quad1Speed = {0.0f, 0.0f};

    // Camera properties
    glm::vec2 m_cameraPosition = {0.0f, 0.0f};

    float m_windowWidth;
    float m_windowHeight;

    struct Vertex {
        glm::vec2 m_position;
    };

    std::array<Vertex, 4> getQuad(float positionX, float positionY, float size) {

        Vertex vertex0 = {
            {positionX - (size/2), positionY - (size/2)}
        };

        Vertex vertex1 = {
            {positionX + (size/2), positionY - (size/2)}
        };

        Vertex vertex2 = {
            {positionX + (size/2), positionY + (size/2)}
        };

        Vertex vertex3 = {
            {positionX - (size/2), positionY + (size/2)}
        };

        return {vertex0, vertex1, vertex2, vertex3};

    }

public:

    PlayableQuadApplication(int windowWidth, int windowHeight) {
        m_windowWidth = (float ) windowWidth;
        m_windowHeight = (float) windowHeight;
    }

    void onReady() override {

        m_camera = std::make_shared<engine::OrthographicCamera>(m_windowWidth, m_windowHeight, 100);

        engine::BufferLayout layout = {
            {"a_position", 2, engine::LayoutElementType::Float}
        };

        // Create the vertex buffer, which will contain the actual data (2 quads), together with the layout of the data
        m_vertexBuffer = std::make_shared<engine::VertexBuffer>(layout, sizeof(Vertex) * 4 * 2);

        unsigned int indices[] = {
                0, 1, 2,
                2, 3, 0,

                4, 5, 6,
                6, 7, 4
        };

        // Create an index buffer, which specifies how to use the vertices to draw triangles
        m_indexBuffer = std::make_shared<engine::IndexBuffer>(indices, 12);

        // Create a vertex array, and bind the vertex buffer and the index buffer into it
        m_vertexArray = std::make_shared<engine::VertexArray>();
        m_vertexArray->addBuffer(m_vertexBuffer, m_indexBuffer);

        m_shader = std::make_shared<engine::Shader>();
        m_shader->attach(GL_VERTEX_SHADER, "res/shaders/vertex-position.glsl");
        m_shader->attach(GL_FRAGMENT_SHADER, "res/shaders/fragment-color.glsl");
        m_shader->compile();
        m_shader->bind();
        m_shader->setUniform4f("u_color", glm::vec4(m_quadColor.x, m_quadColor.y, m_quadColor.z, m_quadColor.w));

    }

    void onUpdate(engine::TimeStep timeStep) override {

        m_quad1Position.x += m_quad1Speed.x * timeStep;
        m_quad1Position.y += m_quad1Speed.y * timeStep;

        // Prepare the quad1, positioned statically
        auto quad1 = getQuad(m_quad1Position.x, m_quad1Position.y, m_quadSize);
        auto quad2 = getQuad(3.0f, 3.0f, m_quadSize);
        Vertex vertices[8];
        memcpy(vertices, quad1.data(), quad1.size() * sizeof(Vertex));
        memcpy(vertices + quad1.size(), quad2.data(), quad2.size() * sizeof(Vertex));

        // Submit the quad1's data
        m_vertexBuffer->setData(vertices, sizeof(quad1) * 2);

        engine::RenderCommand::clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

        engine::Renderer::beginScene(m_camera);

        engine::Renderer::submit(m_shader, m_vertexArray);

    }

    void onGuiRender() override {}

    void onEvent(const engine::Event& event) override {

        if (event.getType() == engine::EventType::WindowResize) {
            engine::WindowResizeEvent derivedEvent = dynamic_cast<const engine::WindowResizeEvent&>(event);
            m_camera->onWindowResize(derivedEvent);
        }

        if (event.getType() == engine::EventType::KeyPress) {
            engine::KeyPressEvent derivedEvent = dynamic_cast<const engine::KeyPressEvent&>(event);
            if (derivedEvent.getKey() == GLFW_KEY_UP) {
                m_camera->addZoomLevel(0.1f);
                return;
            } else if (derivedEvent.getKey() == GLFW_KEY_DOWN) {
                m_camera->addZoomLevel(-0.1f);
                return;
            }
        }

        if (event.getType() == engine::EventType::KeyPress) {
            engine::KeyPressEvent derivedEvent = dynamic_cast<const engine::KeyPressEvent&>(event);
            if (derivedEvent.getKey() == GLFW_KEY_RIGHT) {
                m_quad1Speed.x = m_quadTargetSpeed;
                return;
            } else if (derivedEvent.getKey() == GLFW_KEY_LEFT) {
                m_quad1Speed.x = -m_quadTargetSpeed;
                return;
            }
        }

        if (event.getType() == engine::EventType::KeyRelease) {
            engine::KeyReleaseEvent derivedEvent = dynamic_cast<const engine::KeyReleaseEvent&>(event);
            if ((derivedEvent.getKey() == GLFW_KEY_RIGHT && m_quad1Speed.x > 0) || (derivedEvent.getKey() == GLFW_KEY_LEFT && m_quad1Speed.x < 0) ) {
                m_quad1Speed.x = 0;
                return;
            }
        }

        if (event.getType() == engine::EventType::KeyPress) {
            engine::KeyPressEvent derivedEvent = dynamic_cast<const engine::KeyPressEvent&>(event);
            if (derivedEvent.getKey() == GLFW_KEY_UP) {
                m_quad1Speed.y = m_quadTargetSpeed;
                return;
            } else if (derivedEvent.getKey() == GLFW_KEY_DOWN) {
                m_quad1Speed.y = -m_quadTargetSpeed;
                return;
            }
        }

        if (event.getType() == engine::EventType::KeyRelease) {
            engine::KeyReleaseEvent derivedEvent = dynamic_cast<const engine::KeyReleaseEvent&>(event);
            if ((derivedEvent.getKey() == GLFW_KEY_UP && m_quad1Speed.y > 0) || (derivedEvent.getKey() == GLFW_KEY_DOWN && m_quad1Speed.y < 0) ) {
                m_quad1Speed.y = 0;
                return;
            }
        }

    }
};

int main() {

    engine::Window window("Playable Quad");
    engine::RunLoop runLoop(window);

    PlayableQuadApplication app(window.getWidth(), window.getHeight());

    runLoop.run(app);

    return 0;

}