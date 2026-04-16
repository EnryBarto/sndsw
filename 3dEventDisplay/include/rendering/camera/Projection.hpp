#pragma once

#include <glm/glm.hpp>

namespace snd3D {
    class Projection {
        public:
            virtual void setAspectRatio(float newAspectRatio) = 0;
            virtual void setFov(float newFov) = 0;
            virtual const glm::mat4& getMatrix() = 0;
    };
}