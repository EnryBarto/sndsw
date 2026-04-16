#pragma once

#include <string>

#include "state/AppState.hpp"

namespace snd3D {
    class AppStateManager {
        public:
            void update();
            AppState getCurrentState();

            void close();
            void openGeometryDialog();
            void toggleMovingTrackball();
            void shiftPressed();
            void shiftReleased();
            void toggleMovingPan();
            void toggleImageExport();
            void geometryFileSelected(std::string filePath);
            std::string getDetectorPath();
            void errorLoadingGeometry();
            void geometryLoaded();
            void restart();
            void resetInteraction();

        private:
            AppState currentState = AppState::WELCOME;
            AppState nextState = AppState::WELCOME;

            std::string detectorPath;
    };
}
