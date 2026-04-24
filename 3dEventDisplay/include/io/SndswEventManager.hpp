#pragma once

#include <memory>
#include <cstdint>
#include <vector>

#include "io/RunData.hpp"
#include "io/EventData.hpp"
#include "Scifi.h"
#include "MuFilter.h"
#include "TChain.h"
#include "TClonesArray.h"
#include "SNDLHCEventHeader.h"
#include "sndConfiguration.h"
#include "sndScifiPlane.h"
#include "sndUSPlane.h"


namespace snd3D {
    class SndswEventManager {
        public:
            RunData* loadRun(int64_t runNumber);
            EventData* loadEvent(int64_t eventNumber);

        private:
            // Run data
            std::unique_ptr<TChain> chain;    
            Scifi* scifiGeometry;
            MuFilter* mufilterGeometry;
            snd::Configuration* config;
            TClonesArray* muHits;
            TClonesArray* sfHits;
            SNDLHCEventHeader* header;
            std::vector<snd::analysis_tools::ScifiPlane> scifiPlanes;
            std::vector<snd::analysis_tools::USPlane> usPlanes;

    };
}
