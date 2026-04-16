#pragma once

#define GLFW_INCLUDE_NONE // Include only GLFW data types
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace snd3D {
    class App; // Forward declaration to avoid circular include between App and Callbacks

    class Callbacks {
        public:
            Callbacks(App& app);
            // Static methods that will forward the request to the Callbacks object saved in the window user point
            static void keyAction(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void cursorPosition(GLFWwindow* window, double currentMousePosX, double currentMousePosY);
            static void mouseButton(GLFWwindow* window, int button, int action, int mods);
            static void framebufferSize(GLFWwindow* window, int newWidth, int newHeight);
            static void scroll(GLFWwindow* window, double xOffset, double yOffset);
            static void close(GLFWwindow* window);

        private:
            App& app; // Keep reference to App object to handle events

            // Instance methods that will be called on the Callbacks object saved in the window user point
            void keyAction(int key, int scancode, int action, int mods);
            void cursorPosition(double currentMousePosX, double currentMousePosY);
            void mouseButton(int button, int action, int mods);
            void framebufferSize(int newWidth, int newHeight);
            void scroll(double xOffset, double yOffset);
            void close();
    };
}
