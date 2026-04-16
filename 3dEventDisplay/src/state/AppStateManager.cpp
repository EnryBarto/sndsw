#include "state/AppStateManager.hpp"

#include <iostream>

namespace snd3D {
    void AppStateManager::update() {
        if (this->currentState != this->nextState) {
            this->currentState = this->nextState;
        }

        switch (this->currentState) {
            case AppState::SHOW_GEOMETRY_LOAD:
                this->nextState = AppState::GEOMETRY_LOAD;
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
            case AppState::WELCOME:
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

    void AppStateManager::restart() {
        switch (this->currentState) {
            case AppState::WAIT_GEOM_ABORT:
            case AppState::GEOMETRY_CHOICE:
                this->nextState = AppState::WELCOME;
                break;

            default:
                std::cerr << "ERROR! Confirm geometry error not allowed in state: " << appStateToString(this->currentState) << std::endl;
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
