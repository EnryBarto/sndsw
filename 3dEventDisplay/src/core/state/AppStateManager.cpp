#include "core/state/AppStateManager.hpp"

#include <iostream>

namespace snd3D {
    void AppStateManager::update() {
        if (this->currentState != this->nextState) {
            this->currentState = this->nextState;
        }

        // Once the GUI showed the loading message, start loading the data
        switch (this->currentState) {
            case AppState::SHOW_RUN_LOAD:
                this->nextState = AppState::RUN_LOAD;
                break;

            case AppState::SHOW_EVENT_LOAD:
                this->nextState = AppState::EVENT_LOAD;
                break;

            case AppState::SHOW_GEOMETRY_LOAD:
                this->nextState = AppState::GEOMETRY_LOAD;
                break;

            default:
                break;
        }
    }

    AppState AppStateManager::getCurrentState() {
        return this->currentState;
    }

    void AppStateManager::close() {
        this->nextState = AppState::CLOSED;
    }

    void AppStateManager::openGeometryDialog() {
        switch (this->currentState) {
            case AppState::GEOMETRY_INIT:
                this->nextState = AppState::GEOMETRY_CHOICE;
                break;

            default:
                std::cerr << "ERROR! Opening geometry chooser not allowed in state: " << appStateToString(this->currentState) << std::endl;
                break;
        }
    }

    void AppStateManager::toggleMovingTrackball() {
        switch (this->currentState) {
            case AppState::TRACKBALL:
                this->nextState = AppState::MOVING_TRACKBALL;
                break;
            case AppState::MOVING_TRACKBALL:
                this->nextState = AppState::TRACKBALL;
                break;
            default:
                std::cerr << "ERROR! Toggle moving trackball not allowed in state: " << appStateToString(this->currentState) << std::endl;
                break;
        }
    }

    void AppStateManager::shiftPressed() {
        switch (this->currentState) {
            case AppState::TRACKBALL:
                this->nextState = AppState::PAN;
                break;

            case AppState::MOVING_TRACKBALL:
                this->nextState = AppState::MOVING_PAN;
                break;

            default:
                break;
        }
    }

    void AppStateManager::shiftReleased() {
        switch (this->currentState) {
            case AppState::PAN:
                this->nextState = AppState::TRACKBALL;
                break;

            case AppState::MOVING_PAN:
                this->nextState = AppState::MOVING_TRACKBALL;
                break;

            default:
                break;
        }
    }

    void AppStateManager::toggleMovingPan() {
        switch (this->currentState) {
            case AppState::PAN:
                this->nextState = AppState::MOVING_PAN;
                break;
            case AppState::MOVING_PAN:
                this->nextState = AppState::PAN;
                break;
            default:
                std::cerr << "ERROR! Toggle moving pan not allowed in state: " << appStateToString(this->currentState) << std::endl;
                break;
        }
    }
    void AppStateManager::toggleImageExport() {
        switch (this->currentState) {
            case AppState::TRACKBALL:
            case AppState::MOVING_TRACKBALL:
            case AppState::PAN:
            case AppState::MOVING_PAN:
                this->nextState = AppState::EXPORT_IMAGE;
                break;

            case AppState::EXPORT_IMAGE:
                this->nextState = AppState::TRACKBALL;
                break;

            default:
                break;
        }
    }

    void AppStateManager::runNumberSelected(int64_t number) {
        switch (this->currentState) {
            case AppState::RUN_CHOICE:
                this->run = std::make_unique<RunData>(number, "none", "none", 0);
                this->nextState = AppState::SHOW_RUN_LOAD;
                break;

            default:
                break;
        }
    }

    void AppStateManager::eventNumberSelected(int64_t number) {
        switch (this->currentState) {
            case AppState::EVENT_CHOICE:
                this->event = std::make_unique<EventData>(number, "none", 0);
                this->nextState = AppState::SHOW_EVENT_LOAD;
                break;

            default:
                break;
        }
    }

    void AppStateManager::geometryFileSelected(std::string filePath) {
        switch (this->currentState) {
            case AppState::GEOMETRY_CHOICE:
                this->detectorPath = filePath;
                this->nextState = AppState::SHOW_GEOMETRY_LOAD;
                break;

            default:
                break;
        }
    }

    const RunData* AppStateManager::getRun() {
        return this->run.get();
    }

    const EventData* AppStateManager::getEvent() {
        return this->event.get();
    }

    std::string AppStateManager::getDetectorPath() {
        return this->detectorPath;
    }

    void AppStateManager::errorLoadingGeometry() {
        switch (this->currentState) {
            case AppState::GEOMETRY_LOAD:
                this->nextState = AppState::WAIT_GEOM_ABORT;
                break;

            default:
                std::cerr << "ERROR! Loading geometry error not allowed in state: " << appStateToString(this->currentState) << std::endl;
                break;
        }
    }

    void AppStateManager::runLoaded(RunData* runData) {
        switch (this->currentState) {
            case AppState::RUN_LOAD:
                this->run = std::unique_ptr<RunData>(runData);
                this->nextState = AppState::EVENT_CHOICE;
                break;

            default:
                std::cerr << "ERROR! Loading run not allowed in state: " << appStateToString(this->currentState) << std::endl;
                break;
        }
    }
    void AppStateManager::eventLoaded(EventData* eventData) {
        switch (this->currentState) {
            case AppState::EVENT_LOAD:
                this->event = std::unique_ptr<EventData>(eventData);
                this->nextState = AppState::GEOMETRY_INIT;
                break;

            default:
                std::cerr << "ERROR! Loading event not allowed in state: " << appStateToString(this->currentState) << std::endl;
                break;
        }
    }

    void AppStateManager::geometryLoaded() {
        switch (this->currentState) {
            case AppState::GEOMETRY_LOAD:
                this->nextState = AppState::TRACKBALL;
                break;

            default:
                std::cerr << "ERROR! Loading geometry not allowed in state: " << appStateToString(this->currentState) << std::endl;
                break;
        }
    }

    void AppStateManager::revert() {
        switch (this->currentState) {
            case AppState::GEOMETRY_CHOICE:
                this->nextState = AppState::GEOMETRY_INIT;
                break;

            case AppState::GEOMETRY_INIT:
                this->nextState = AppState::EVENT_CHOICE;
                break;

            case AppState::EVENT_CHOICE:
                this->nextState = AppState::RUN_CHOICE;
                break;

            default:
                break;
        }
    }

    void AppStateManager::resetInteraction() {
        switch (this->currentState) {
            case AppState::TRACKBALL:
            case AppState::MOVING_TRACKBALL:
            case AppState::PAN:
            case AppState::MOVING_PAN:
                this->nextState = AppState::TRACKBALL;
                break;

            default:
                break;
        }
    }
}
