#pragma once

#include <string>
#include <glad/glad.h>

namespace snd3D {
    class TextureMaker {
        public:
            static GLuint loadTexture(std::string path, int flip = 0);

        private:
            TextureMaker() { }
    };
}
