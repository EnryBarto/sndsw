#pragma once

#include "BasicProjection.hpp"

namespace snd3D {
    class PerspectiveProjection : public BasicProjection {
        public:
            PerspectiveProjection(float aspectRatio, float fov);

        protected:
            virtual void computeProjectionMatrix() override;
    };
}