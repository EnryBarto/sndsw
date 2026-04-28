#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>

namespace snd3D {
    class Texture {
        public:
            Texture(std::string _name, std::string path);
            ~Texture();
            std::string getName();
            void setName(std::string _name);
            GLuint getProgramId();
            int getWidth();
            int getHeight();

        private:
            std::string name;
            GLuint programId;
            int width, height;
    };
}
