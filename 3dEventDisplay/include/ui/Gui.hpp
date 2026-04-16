#pragma once

#include <string>

#include "scene/Object.hpp"
#include "scene/Node.hpp"

namespace snd3D {
    class App;

    class Gui {
        public:
            Gui(App& app, float fontSize);
            ~Gui();
            void update();
            void render();
            void changeFontSize(float factor);
            bool isPointerOverGui();

        private:
            App& app; // Keep reference to App object
            float fontSize;
            float menuBarHeight = 0;

            void drawMenuBar();
            void drawInspector();
            void drawRenderOptions();
            void drawObjectTree(const std::string& label, Object* obj);
            void drawNodeTree(Node* node);
            void drawGeometryFileDialog();
            void drawWelcomeMessage();
            void drawLoadingGeometry();
            void drawGeometryError();
    };
}
