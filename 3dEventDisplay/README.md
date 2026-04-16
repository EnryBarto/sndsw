# SND 3D Event Display

A 3D visualization tool for the **SND@LHC** (Scattering and Neutrino Detector at the LHC) experiment.

## Build Requirements
* **C++17** compatible compiler
* **CMake** (version 3.15 or higher)
* **OpenGL** development headers and drivers

## Dependencies
The project utilizes a hybrid approach for dependency management, using **CMake FetchContent** for automatic downloads and local storage for core UI components.

### Automatically Managed (via CMake)
These are downloaded and compiled automatically during the first build:
* [**GLFW**](https://github.com/glfw/glfw): Window management and input handling.
* [**GLM**](https://github.com/g-truc/glm): Mathematics library for graphics operations.
* [**Assimp**](https://github.com/assimp/assimp): Open Asset Import Library for loading 3D models (glTF).
* [**ImGuiFileDialog**](https://github.com/aiekick/ImGuiFileDialog): A feature-rich file selection dialog for ImGui.
* [**stb_image**](https://github.com/nothings/stb): Image library for exporting screenshots.

### Local External Libraries
Located in the `/external` directory:
* [**ImGui**](https://github.com/ocornut/imgui): Bloat-free Graphical User interface for C++.
* [**GLAD**](https://glad.dav1d.de/): OpenGL loader-generator.

### Assets & Fonts
* [**JetBrains Mono**](https://www.jetbrains.com/lp/mono/): Licensed under the **SIL Open Font License 1.1**.
    * The font is **embedded** directly into the binary via `include/ui/JetBrainsMono.h` using a compressed byte array.

## Compilation
To compile the project:

```bash
mkdir build && cd build
cmake ..
make