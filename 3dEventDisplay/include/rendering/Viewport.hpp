#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "rendering/camera/Camera.hpp"
#include "rendering/camera/Projection.hpp"

namespace snd3D {
    class Viewport {
        public:
            Viewport(float aspectRatio, bool ortographic);

            // GETTER
            bool isCameraChanged();
            bool isOrthographic();
            const glm::vec3 getCameraTarget() const;
            const glm::vec3 getCameraPosition() const;
            const glm::mat4& getProjectionMatrix() const;
            const glm::mat4& getViewMatrix() const;

            // MOVEMENTS
            void moveParallel(float deltaX, float deltaY);
            void movePerpendicular(float deltaX, float deltaY);
            void rotateByAngles(float deltaAngleX, float deltaAngleY);
            void rotateTrackball(glm::vec3 origin, glm::vec3 destination);

            void setAspectRatio(float newAspectRatio);
            void setDirection(Camera::Directions direction);
            void toggleProjectionType();
            void zoom(float offset);
            void reset();


        private:
            std::unique_ptr<Camera> camera;
            std::unique_ptr<Projection> projection;
            bool ortographic;
            float aspectRatio;
    };
}