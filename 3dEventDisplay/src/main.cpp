#include <cstdlib>
#include <iostream>

#include "core/App.hpp"

int main() {

    std::cout << "Starting 3dEventDisplay" << std::endl;

    try {
        snd3D::App app;
        app.run();
    }
    catch (const std::exception& ex) {
        std::cerr << "An Exception Occurred:\n" << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
