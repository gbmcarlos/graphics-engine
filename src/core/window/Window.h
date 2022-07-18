#pragma once

#include "../input/Input.h"

// GLEW needs to be included before GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

namespace engine {

    class Window {
    private:
        std::string m_title;
        int m_width, m_height;
        int m_viewportWidth, m_viewportHeight;

        GLFWwindow* m_windowContext;

        void init();

        static void keyboardEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void resizeEventCallback(GLFWwindow* window, int width, int height);

    public:
        Window(const std::string& title);
        Window(int width, int height, const std::string& title);
        ~Window();

        bool shouldRun();
        void setTitle(const std::string& title);
        void swap();
        void pollEvents();
        void bind() const;
        void unbind() const;

        inline const std::string& getTitle() { return m_title;}
        inline int getWidth() { return m_width;}
        inline int getHeight() { return m_height;}
        inline int getViewportWidth() { return m_viewportWidth;}
        inline int getViewportHeight() { return m_viewportHeight;}
        inline GLFWwindow*& getContext() {return m_windowContext;}

    };

}