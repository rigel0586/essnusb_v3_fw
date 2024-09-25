#include "EveEvent.hpp"
ClassImp(esbroot::eve::superfgd::EveEvent)

#include "data/SuperFGD/FgdDetectorPoint.hpp"
#include <fairlogger/Logger.h>

#include <TSpline.h>
#include <TRandom.h>

#include <iostream>
#include <map>
using std::cout;
using std::endl;

namespace esbroot {
namespace eve {
namespace superfgd {


// -----   Default constructor   -------------------------------------------
EveEvent::EveEvent() 
{ 
}
// -------------------------------------------------------------------------

// -----   Constructor   -------------------------------------------
EveEvent::EveEvent(const char* geoConfigFile) 
{ 
  fParams.LoadPartParams(geoConfigFile);
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
EveEvent::~EveEvent() 
{
}
// -------------------------------------------------------------------------




// -----   Public method Exec   --------------------------------------------
bool EveEvent::Exec(int eventId, TClonesArray* data, std::vector<core::eve::ITrack>& tracks)
{
    LOG(debug2) <<" EveEvent::Exec Event " << eventId;

    fdPoints = data;
    const Int_t points = fdPoints->GetEntries();
    LOG(debug2) <<" fdPoints->GetEntries() " << fdPoints->GetEntries();
    int nextPoint(0);
    std::vector<core::eve::ITrackPoint> allPoints;
    //=============================================
    //====       Extract Data       ===============
    //=============================================
    for(Int_t i =0; i < points; i++)
    {
        data::superfgd::FgdDetectorPoint* point = (data::superfgd::FgdDetectorPoint*)fdPoints->At(i);

        
        double time = point->GetTime();
        Int_t pdg = point->GetPdg();
        Int_t trackId = point->GetTrackID();
        
        TVector3 dpos = point->GetDetectorpos(); // Get the detector position
        double pos_x = point->GetX() - dpos.X();
        double pos_y = point->GetY() - dpos.Y();
        double pos_z = point->GetZ() - dpos.Z();
        // TVector3 position(pos_x, pos_y, pos_z);
        TVector3 position(point->GetX(), point->GetY(), point->GetZ());
        
        TVector3 mom(point->GetPx(), point->GetPy(), point->GetPz());

        core::eve::ITrackPoint trPoint(time, pdg, trackId, mom, position);
        allPoints.emplace_back(trPoint);
    }

    //=============================================
    //====          Sort Data       ===============
    //=============================================
    std::map<Int_t, core::eve::ITrack> trackMap;
    for(int i = 0; i < allPoints.size(); ++i)
    {
        Int_t trId = allPoints[i].GetTrackId();
        auto it = trackMap.find(trId);
        if (it != trackMap.end())
        {
            trackMap[trId].addPoint(allPoints[i]);
        }
        else
        {
            core::eve::ITrack newTrack;
            newTrack.addPoint(allPoints[i]);
            trackMap[trId] = newTrack;
        }
    }

    //=============================================
    //====          Export Data      ==============
    //=============================================
    auto it = trackMap.begin();
    while(it != trackMap.end())
    {
        tracks.emplace_back(it->second);
        ++it;
    }

    return true;
}
// -------------------------------------------------------------------------
  

} //superfgd
} //eve
} //esbroot
