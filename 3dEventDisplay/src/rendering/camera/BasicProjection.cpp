#include "rendering/camera/BasicProjection.hpp"

#include <glm/glm.hpp>

#include "core/Constants.hpp"

namespace snd3D {

    BasicProjection::BasicProjection(float aspectRatio, float fov) {
        this->nearPlane = constants::limits::PROJ_NEARPLANE;
        this->farPlane = constants::limits::PROJ_FARPLANE;
        this->aspectRatio = aspectRatio;
        this->fovY = glm::clamp(fov, constants::limits::PROJ_FOVY_MIN, constants::limits::PROJ_FOVY_MAX);
        this->matrix = glm::mat4(1.0f);
    }

    void BasicProjection::setAspectRatio(float newAspectRatio) {
        if (std::isnan(newAspectRatio)) return;
        this->aspectRatio = newAspectRatio;
        this->computeProjectionMatrix();
    }

    void BasicProjection::setFov(float newFov) {
        this->fovY = glm::clamp(newFov, constants::limits::PROJ_FOVY_MIN, constants::limits::PROJ_FOVY_MAX);
        this->computeProjectionMatrix();
    }

    const glm::mat4& BasicProjection::getMatrix() {
        return this->matrix;
    }
}