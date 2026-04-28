#pragma once

#include <string>
#include <cstdint>

#include "scene/Object.hpp"
#include "scene/Node.hpp"

namespace snd3D {
    class App;

    class Gui {
        public:
            Gui(App& _app, float _fontSize);
            ~Gui();
            void update();
            void render();
            void changeFontSize(float factor);
            bool isPointerOverGui();

        private:
            App& app;
            float fontSize;
            float menuBarHeight = 0;
            int64_t runInputNumber = 0;     // Used as a buffer for run number input
            int64_t eventInputNumber = 0;   // Used as a buffer for event number input

            void drawMenuBar();
            void drawInspector();
            void drawRenderOptions();
            void drawObjectTree(const std::string& label, Object* obj);
            void drawNodeTree(Node* node);
            void drawRunDialog();
            void drawLoadingData();
            void drawEventDialog();
            void drawGeometryInit();
            void drawGeometryFileDialog();
            void drawGeometryError();
    };
}
