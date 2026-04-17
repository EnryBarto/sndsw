#include "rendering/camera/PerspectiveProjection.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace snd3D {

    PerspectiveProjection::PerspectiveProjection(float _aspectRatio, float _fov) : BasicProjection(_aspectRatio, _fov) {
        this->computeProjectionMatrix();
    }

    void PerspectiveProjection::computeProjectionMatrix() {
        this->matrix = glm::perspective(glm::radians(this->fovY), this->aspectRatio, this->nearPlane, this->farPlane);
    }
}
