#pragma once

#include <memory>

#include "state/AppStateManager.hpp"
#include "core/WindowManager.hpp"
#include "core/AppSettings.hpp"
#include "scene/Scene.hpp"
#include "ui/Gui.hpp"

namespace snd3D {
    class App {

        // They need access to WindowManager and Scene
        friend class Callbacks;
        friend class Gui;

        public:
            App();
            void run();
            void exportImage();

        private:
            AppSettings settings;
            AppStateManager stateManager;
            std::unique_ptr<Callbacks> callbacks;
            std::unique_ptr<Gui> guiManager;
            std::unique_ptr<WindowManager> windowManager;
            std::unique_ptr<Scene> scene;
    };
}
