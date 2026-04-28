#pragma once

#include <string>

namespace snd3D {
    struct RunData {
        const int runNumber;
        const std::string startDate;
        const std::string geoName;
        const int totalEvents;

        RunData(int id, std::string date, std::string _geoName, int events)
            : runNumber(id), startDate(std::move(date)), geoName(std::move(_geoName)), totalEvents(events) {}
    };
}
