#include "scene/Node.hpp"

namespace snd3D {

    Node::Node(const aiScene* scene, aiNode* node, std::vector<std::shared_ptr<Mesh>>& meshes) {

        aiMatrix4x4 matrix = node->mTransformation;

        this->localModelMatrix = glm::mat4(
            matrix.a1, matrix.b1, matrix.c1, matrix.d1,
            matrix.a2, matrix.b2, matrix.c2, matrix.d2,
            matrix.a3, matrix.b3, matrix.c3, matrix.d3,
            matrix.a4, matrix.b4, matrix.c4, matrix.d4
        );

        this->name = std::string(node->mName.C_Str());

        // Get the reference to all the meshes of the node
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            this->meshes.push_back(meshes[node->mMeshes[i]]);
        }

        // Create children nodes
        this->childrenNode.reserve(node->mNumChildren);
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            this->childrenNode.push_back(std::make_unique<Node>(scene, node->mChildren[i], meshes));
        }
    }

    Node::Node(std::string name, std::vector<std::shared_ptr<Mesh>>& meshes) {
        this->name = name;
        this->localModelMatrix = glm::mat4(1.0f);
        this->globalModelMatrix = glm::mat4(1.0f);

        // Get the reference to all the meshes passed
        for (auto mesh : meshes) {
            this->meshes.push_back(mesh);
        }
    }

    void Node::setGlobalActive(bool value) {
        this->active = value;

        for (auto& node : this->childrenNode) {
            node->setGlobalActive(value);
        }

        for (auto& mesh : this->meshes) {
            mesh->setActive(value);
        }
    }

    void Node::updateGlobalModelMatrix(const glm::mat4& parentModelMatrix) {
        this->globalModelMatrix = parentModelMatrix * this->localModelMatrix;

        for (auto& node : this->childrenNode) {
            node->updateGlobalModelMatrix(this->globalModelMatrix);
        }
    }

    void Node::render(const glm::mat4& parentModelMatrix, bool showAnchor, Shader* shader) {
        glm::mat4 modelMatrix = parentModelMatrix * this->localModelMatrix;

        for (auto& node : this->childrenNode) {
            node->render(modelMatrix, showAnchor, shader);
        }

        for (auto& mesh : this->meshes) {
            mesh->render(modelMatrix, showAnchor, shader);
        }
    }

    void Node::render(bool showAnchor, Shader* shader) {

        if (this->active) {
            for (auto& node : this->childrenNode) {
                node->render(showAnchor, shader);
            }

            for (auto& mesh : this->meshes) {
                mesh->render(this->globalModelMatrix, showAnchor, shader);
            }
        }
    }
}
