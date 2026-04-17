#pragma once

#include "BasicProjection.hpp"

namespace snd3D {
    class PerspectiveProjection : public BasicProjection {
        public:
            PerspectiveProjection(float _aspectRatio, float _fov);

        protected:
            virtual void computeProjectionMatrix() override;
    };
}