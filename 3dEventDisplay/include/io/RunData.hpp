#pragma once

#include <string>

namespace snd3D {
    struct RunData {
        const int runNumber;
        const std::string startDate;
        const std::string geoPath;
        const int totalEvents;

        RunData(int id, std::string date, std::string path, int events)
            : runNumber(id), startDate(std::move(date)), geoPath(std::move(path)), totalEvents(events) {}
    };
}
