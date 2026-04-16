#include "rendering/engine/Material.hpp"

#include <assimp/material.h>
#include <assimp/GltfMaterial.h>
#include <assimp/types.h>

namespace snd3D {

    Material::Material(const aiMaterial* material) {
        aiString name;
        aiColor4D baseColor(0.5f, 0.5f, 0.5f, 1.0f);
        float metallic = 0, roughness = 1;

        if (material->Get(AI_MATKEY_NAME, name) != AI_SUCCESS) {
            this->name = "NoNameMaterial";
        } else {
            this->name = std::string(name.C_Str());
        }

        material->Get(AI_MATKEY_BASE_COLOR, baseColor);
        material->Get(AI_MATKEY_METALLIC_FACTOR, metallic);
        material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness);

        glm::vec3 color = glm::vec3(baseColor.r, baseColor.g, baseColor.b);
        this->alpha = baseColor.a;

        this->ambient = color * 0.1f;
        this->diffuse = color * (1.0f - metallic);
        this->specular = glm::mix(glm::vec3(0.04f), color, metallic);
        this->shininess = (1.0f - roughness) * 128.0f;
    }

    glm::vec3 Material::getAmbient() {
        return this->ambient;
    }

    glm::vec3 Material::getDiffuse() {
        return this->diffuse;
    }

    glm::vec3 Material::getSpecular() {
        return this->specular;
    }

    float Material::getShininess() {
        return this->shininess;
    }
}
