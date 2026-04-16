#include "scene/Mesh.hpp"

#include <iostream>

#include <glad/glad.h>

using namespace std;
using namespace glm;

namespace snd3D {

    Mesh::Mesh(string name, vector<vec3>& vertices, vector<vec4>& colors, vector<vec3>& normals, vector<GLuint>& indices, vec4 baseColor) {
        this->name = name;
        this->gpuMesh = make_unique<GpuMesh>(vertices, colors, normals, indices, vertices[0]);
    }

    Mesh::Mesh(aiMesh* mesh, vec4 baseColor) {
        vector<vec3> vertices;
        vector<vec4> colors;
        vector<GLuint> indices;
        vector<vec3> normals;

        this->name = std::string(mesh->mName.C_Str());

        if (this->name.empty()) {
            this->name = "Unnamed_Mesh";
        }

        // Fill vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D pos = mesh->mVertices[i];
            vertices.push_back(vec3(pos.x, pos.y, pos.z));
        }

        // Fill Colors
        if (mesh->HasVertexColors(0)) {
            for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
                aiColor4D c = mesh->mColors[0][i];
                colors.push_back(vec4(c.r, c.g, c.b, c.a));
            }
        } else {
            // cerr << "Colors data missing - Using vec4(0.6f, 0.6f, 0.6f, 1.0f)" << endl;
            for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
                colors.push_back(baseColor);
            }
        }

        // Fill vertices normal
        if (mesh->HasNormals()) {
            for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
                aiVector3D n = mesh->mNormals[i];
                normals.push_back(vec3(n.x, n.y, n.z));
            }
        } else {
            cerr << "Normals data missing - Using vec3(0)" << endl;
            for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
                normals.push_back(vec3(0));
            }
        }

        // Fill face indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            // Assume the model has only triangles, so 3 indices per vertex
            indices.push_back(mesh->mFaces[i].mIndices[0]);
            indices.push_back(mesh->mFaces[i].mIndices[1]);
            indices.push_back(mesh->mFaces[i].mIndices[2]);
        }

        this->gpuMesh = make_unique<GpuMesh>(vertices, colors, normals, indices, vertices[0]);
    }

    void Mesh::setMaterial(const shared_ptr<Material>& material) {
        this->material = material;
    }

    void Mesh::setActive(bool value) {
        this->active = value;
    }

    void Mesh::render(const glm::mat4& modelMatrix, bool showAnchor, Shader* shader) {
        if (this->active) {
            if (shader == nullptr) {
                cerr << "ATTENTION!!! Shader not set" << endl;
                return;
            }
            shader->bindLocalUniforms(modelMatrix, this->material.get());
            this->gpuMesh->render(showAnchor);
        }
    }
}
