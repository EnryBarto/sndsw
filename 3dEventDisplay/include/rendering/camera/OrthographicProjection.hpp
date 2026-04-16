#pragma once

#include "BasicProjection.hpp"

namespace snd3D {
    class OrthographicProjection : public BasicProjection {
        public:
            OrthographicProjection(float aspectRatio, float fov, float cameraDistance);
            void setCameraDistance(float cameraDistance);

        protected:
            float distance;     // Distance of the camera from the target

            virtual void computeProjectionMatrix() override;
    };
}