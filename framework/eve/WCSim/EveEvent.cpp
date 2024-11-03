#include "EveEvent.hpp"
ClassImp(esbroot::eve::wcsim::EveEvent)

#include "data/WCSim/WCSimDataPoint.hpp"
#include <fairlogger/Logger.h>

#include <TSpline.h>
#include <TRandom.h>

#include <iostream>
#include <map>
using std::cout;
using std::endl;

namespace esbroot {
namespace eve {
namespace wcsim {


// -----   Default constructor   -------------------------------------------
EveEvent::EveEvent() 
{ 
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
        data::wcsim::WCSimDataPoint* point = (data::wcsim::WCSimDataPoint*)fdPoints->At(i);

        core::eve::ITrackPoint trPoint(point->GetStepTime()
                            , point->GetPdg()
                            , point->GetTrackID()
                            , point->GetMomentum()
                            , point->GetPosition());
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
  

} //wcsim
} //eve
} //esbroot
