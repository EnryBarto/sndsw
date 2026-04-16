#include "scene/Object.hpp"

#include <vector>

#include <assimp/color4.h>
#include <glm/ext/matrix_transform.hpp>

#include "rendering/engine/Material.hpp"
#include "scene/Mesh.hpp"

namespace snd3D {

    Object::Object(const aiScene* scene) {
        // Load all materials
        std::vector<std::shared_ptr<Material>> materials;
        for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
            materials.push_back(std::make_shared<Material>(scene->mMaterials[i]));
        }

        // Load all meshes, also setting their materials
        std::vector<std::shared_ptr<Mesh>> meshes;
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            aiColor4D baseColor(0.6f, 0.6f, 0.6f, 1.0f); // Default color
            scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_BASE_COLOR, baseColor);
            auto mesh = std::make_shared<Mesh>(scene->mMeshes[i], glm::vec4(baseColor.r, baseColor.g, baseColor.b, baseColor.a));
            mesh->setMaterial(materials[scene->mMeshes[i]->mMaterialIndex]);
            meshes.push_back(mesh);
        }

        // Recursively start creating nodes and store buffered model matrices
        this->rootNode = std::make_unique<Node>(scene, scene->mRootNode, meshes);
        this->rootNode->updateGlobalModelMatrix(this->modelMatrix);
    }

    Object::Object(Node* rootNode) {
        this->rootNode = std::unique_ptr<Node>(rootNode);
    }

    void Object::setShader(const std::shared_ptr<Shader>& shader) {
        this->shader = shader;
    }

    void Object::updateModelMatrix(const glm::mat4& modelMatrix) {
        this->modelMatrix = modelMatrix;
        this->rootNode->updateGlobalModelMatrix(this->modelMatrix);
    }

    void Object::render(const Viewport& viewport, bool showAnchor, const float edgeAlphaValue, const float faceAlphaValue, const float edgeThickness) {
        if (this->active) {
            this->shader->use();
            this->shader->bindGlobalUniforms(viewport.getViewMatrix(), viewport.getProjectionMatrix(), viewport.getCameraPosition(), edgeAlphaValue, faceAlphaValue, edgeThickness);
            // Using the render mode without passing model matrix beacuse we calculated it in the object creation
            this->rootNode->render(showAnchor, this->shader.get());
        }
    }

    void Object::setGlobalActive(bool value) {
        this->active = value;
        this->rootNode->setGlobalActive(value);
    }
}
