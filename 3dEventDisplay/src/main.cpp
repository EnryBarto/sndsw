#include <cstdlib>
#include <iostream>

#include "core/App.hpp"
#include "sndTchainGetter.h"

int main() {

    // Test for SND libraries
    int run_number = 100639; // from TB 2023

    // Note: chain is a smart pointer
    // By defalut, chain includes all partitions for that run
    // and retrieves the data using the default csv mapping in sndsw
    auto chain = snd::analysis_tools::GetTChain(run_number);

    std::cout << chain->GetEntries() << "\n";

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
