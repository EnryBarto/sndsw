#include "io/SndswEventManager.hpp"

#include <utility>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <iostream>

#include "sndTchainGetter.h"
#include "sndScifiPlane.h"
#include "sndGeometryGetter.h"
#include "sndPlaneTools.h"

#include "io/HitData.hpp"

namespace snd3D {

    RunData* SndswEventManager::loadRun(int64_t runNumber) {
        
        this->chain = snd::analysis_tools::GetTChain(runNumber);
        std::pair<Scifi*, MuFilter*> geometry = snd::analysis_tools::GetGeometry(runNumber);
        this->scifiGeometry = geometry.first;
        this->mufilterGeometry = geometry.second;
        this->config = new snd::Configuration(snd::Configuration::GetOption(runNumber), this->scifiGeometry, this->mufilterGeometry);

        this->muHits = new TClonesArray("MuFilterHit");
        this->chain->SetBranchAddress("Digi_MuFilterHits", &this->muHits);
        this->sfHits = new TClonesArray("sndScifiHit");
        this->chain->SetBranchAddress("Digi_ScifiHits", &this->sfHits);
        this->header = new SNDLHCEventHeader();
        this->chain->SetBranchAddress(
                this->chain->GetBranch("EventHeader") ? "EventHeader" : "EventHeader.",
                &this->header
        );
        this->scifiGeometry->InitEvent(this->header);
        this->mufilterGeometry->InitEvent(this->header);

        this->scifiPlanes.clear();
        this->usPlanes.clear();

        // LOAD GEOMETRY FILENAME
        std::string fullPath = snd::analysis_tools::GetGeoPath(runNumber); 
        size_t lastSlash = fullPath.find_last_of('/');
        std::string fileName = (lastSlash == std::string::npos) ? fullPath : fullPath.substr(lastSlash + 1);

        size_t lastDot = fileName.find_last_of('.');
        if (lastDot != std::string::npos && fileName.substr(lastDot) == ".root") {
            fileName = fileName.substr(0, lastDot);
        }

        // LOAD RUN DATE AS THE DATE OF THE FIRST EVENT
        this->chain->GetEntry(0);
        int64_t rawTime = this->header->GetUTCtimestamp();
        time_t t = static_cast<time_t>(rawTime);
        struct tm *dt = std::gmtime(&t);
        std::stringstream ss;
        ss << std::put_time(dt, "%Y-%m-%d");

        return new RunData(runNumber, ss.str(), fileName, this->chain->GetEntries());
    }

    EventData* SndswEventManager::loadEvent(int64_t eventNumber) {
        this->chain->GetEntry(eventNumber);

        int64_t rawTime = this->header->GetUTCtimestamp();
        time_t t = static_cast<time_t>(rawTime);
        struct tm *dt = std::gmtime(&t);
        std::stringstream ss;
        ss << std::put_time(dt, "%Y-%m-%d %H:%M:%S");

        EventData* toReturn = new EventData(eventNumber, ss.str(), rawTime);

        this->scifiPlanes = snd::analysis_tools::FillScifi(*this->config, this->sfHits, this->scifiGeometry);
        this->usPlanes = snd::analysis_tools::FillUS(*this->config, this->muHits, this->mufilterGeometry);

        for (auto &us : this->usPlanes) {
            if (us.HasShower()) {
                us.FindCentroid();
                auto centroid = us.GetCentroid();
                if (!std::isnan(centroid.X()) && !std::isnan(centroid.Y())) {
                    toReturn->addCentroid(new HitData(centroid.X(), centroid.Y(), centroid.Z(), us.GetTotEnergy().large));
                }
            }
        }

        for (auto &sf : this->scifiPlanes) {
            if (sf.HasShower()) {
                sf.FindCentroid();
                auto centroid = sf.GetCentroid();
                if (!std::isnan(centroid.X()) && !std::isnan(centroid.Y())) {
                    toReturn->addCentroid(new HitData(centroid.X(), centroid.Y(), centroid.Z(), sf.GetTotEnergy().x + sf.GetTotEnergy().y));
                }
            }
        }

        return toReturn;
    }
}
