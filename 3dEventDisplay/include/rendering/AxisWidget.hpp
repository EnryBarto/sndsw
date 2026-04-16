#pragma once

#include <memory>

#include <imgui.h>
#include <glm/glm.hpp>

#include "rendering/engine/Shader.hpp"

namespace snd3D {
    class AxisWidget {
        public:
            AxisWidget(std::shared_ptr<Shader> shader, const int size, const int margin);
            ~AxisWidget();

            void draw(const glm::mat4& viewMatrix, int screenWidth, int screenHeight);

        private:
            // GPU buffers location
            GLuint vao = 0;     // Vertex Array Object (VAO) identifier
            GLuint vboVertices = 0; // Vertex Buffer Object (VBO) identifier containing the object's vertex coordinates
            GLuint vboColors = 0;   // VBO identifier containing the object's vertex colors

            std::shared_ptr<Shader> shader;
            glm::mat4 projectionMatrix;

            int totalSize;
            int margin;

            void printLabel(glm::vec3 pos, const char* label, ImU32 color, const glm::mat4& viewRotation, int screenWidth, int screenHeight);
    };
}