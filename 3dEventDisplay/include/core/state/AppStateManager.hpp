#pragma once

#include <string>
#include <memory>
#include <cstdint>

#include "core/state/AppState.hpp"
#include "io/RunData.hpp"
#include "io/EventData.hpp"

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
            void runNumberSelected(int64_t number);
            void eventNumberSelected(int64_t number);
            void geometryFileSelected(std::string filePath);
            const RunData* getRun();
            const EventData* getEvent();
            std::string getDetectorPath();
            void errorLoadingGeometry();
            void geometryLoaded();
            void runLoaded(RunData* runData);
            void eventLoaded(EventData* eventData);
            void revert();
            void resetInteraction();

        private:
            AppState currentState = AppState::RUN_CHOICE;
            AppState nextState = AppState::RUN_CHOICE;

            std::unique_ptr<RunData> run;
            std::unique_ptr<EventData> event;
            std::string detectorPath;
    };
}
