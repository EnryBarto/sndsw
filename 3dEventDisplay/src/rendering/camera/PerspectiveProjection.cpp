#include "rendering/camera/PerspectiveProjection.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace snd3D {

    PerspectiveProjection::PerspectiveProjection(float aspectRatio, float fov) : BasicProjection(aspectRatio, fov) {
        this->computeProjectionMatrix();
    }

    void PerspectiveProjection::computeProjectionMatrix() {
        this->matrix = glm::perspective(glm::radians(this->fovY), this->aspectRatio, this->nearPlane, this->farPlane);
    }
}
