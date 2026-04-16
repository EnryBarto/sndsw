#include "rendering/camera/OrthographicProjection.hpp"

#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

namespace snd3D {

    OrthographicProjection::OrthographicProjection(float aspectRatio, float fov, float cameraDistance) : BasicProjection(aspectRatio, fov) {
        this->distance = cameraDistance;
        this->computeProjectionMatrix();
    }

    void OrthographicProjection::computeProjectionMatrix() {
        // ortho() requires the vertices of the front plane: we compute them centering the view
        float zoom = this->distance * std::tan(glm::radians(this->fovY * 0.5f)); // Use the fov to scale the front plane
        float halfWidth = (zoom * this->aspectRatio) / 2.0f;
        float halfHeight = zoom / 2.0f;
        this->matrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, this->nearPlane, this->farPlane);
    }

    void OrthographicProjection::setCameraDistance(float cameraDistance) {
        this->distance = cameraDistance;
        this->computeProjectionMatrix();
    }
}
