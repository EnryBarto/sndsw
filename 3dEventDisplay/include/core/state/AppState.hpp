#pragma once

namespace snd3D {
    enum class AppState {
        RUN_CHOICE,
        SHOW_RUN_LOAD,
        RUN_LOAD,
        EVENT_CHOICE,
        SHOW_EVENT_LOAD,
        EVENT_LOAD,
        GEOMETRY_INIT,
        GEOMETRY_CHOICE,
        SHOW_GEOMETRY_LOAD,
        GEOMETRY_LOAD,
        WAIT_GEOM_ABORT,
        TRACKBALL,
        MOVING_TRACKBALL,
        PAN,
        MOVING_PAN,
        EXPORT_IMAGE,
        CLOSED
    };

    inline const char* appStateToString(AppState state) {
        switch (state) {
            case AppState::RUN_CHOICE:          return "Choosing run";
            case AppState::SHOW_RUN_LOAD:       return "Loading RUN";
            case AppState::RUN_LOAD:            return "Loading RUN";
            case AppState::EVENT_CHOICE:        return "Choosing event";
            case AppState::SHOW_EVENT_LOAD:     return "Loading event";
            case AppState::EVENT_LOAD:          return "Loading event";
            case AppState::GEOMETRY_INIT:       return "Choosing geometry";
            case AppState::GEOMETRY_CHOICE:     return "Choosing geometry";
            case AppState::GEOMETRY_LOAD:       return "Loading geometry";
            case AppState::SHOW_GEOMETRY_LOAD:  return "Loading geometry";
            case AppState::WAIT_GEOM_ABORT:     return "Failed loading geometry";
            case AppState::TRACKBALL:           return "Trackball";
            case AppState::MOVING_TRACKBALL:    return "Trackball moving";
            case AppState::PAN:                 return "Pan";
            case AppState::MOVING_PAN:          return "Pan moving";
            case AppState::EXPORT_IMAGE:        return "Export image";
            case AppState::CLOSED:              return "Closing";
            default:                            return "Unknown";
        }
    }

    inline bool isInteractionState(AppState state) {
        return state == AppState::TRACKBALL
            || state == AppState::MOVING_TRACKBALL
            || state == AppState::PAN
            || state == AppState::MOVING_PAN;
    }
}
