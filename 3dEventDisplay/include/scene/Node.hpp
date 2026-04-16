#pragma once

#include <memory>
#include <vector>
#include <string>

#include <glm/fwd.hpp>
#include <assimp/scene.h>

#include "scene/Mesh.hpp"
#include "rendering/engine/Shader.hpp"

namespace snd3D {
    class Node {

        friend class Gui; // Needs access to show and edit node propreties

        public:
            Node(const aiScene* scene, aiNode* node, std::vector<std::shared_ptr<Mesh>>& meshes);
            Node(std::string name, std::vector<std::shared_ptr<Mesh>>& meshes);
            void setGlobalActive(bool value);
            void updateGlobalModelMatrix(const glm::mat4& parentModelMatrix);
            void render(const glm::mat4& parentModelMatrix, bool showAnchor, Shader* shader);
            void render(bool showAnchor, Shader* shader); // Use the buffered model matrix

        private:
            std::string name;
            std::vector<std::unique_ptr<Node>> childrenNode;
            std::vector<std::shared_ptr<Mesh>> meshes;
            glm::mat4 localModelMatrix;
            glm::mat4 globalModelMatrix;
            bool active = true;
    };
}
