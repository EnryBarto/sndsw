#pragma once

#include "BasicProjection.hpp"

namespace snd3D {
    class OrthographicProjection : public BasicProjection {
        public:
            OrthographicProjection(float _aspectRatio, float _fov, float _cameraDistance);
            void setCameraDistance(float _cameraDistance);

        protected:
            float distance;     // Distance of the camera from the target

            virtual void computeProjectionMatrix() override;
    };
}