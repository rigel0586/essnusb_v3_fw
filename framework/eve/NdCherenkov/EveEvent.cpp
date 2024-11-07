#include "EveEvent.hpp"
ClassImp(esbroot::eve::ndcherenkov::EveEvent)

#include "data/NDCherenkov/NDCherenkovDataPoint.hpp"
#include <fairlogger/Logger.h>

#include <TSpline.h>
#include <TRandom.h>

#include <iostream>
#include <map>
using std::cout;
using std::endl;

namespace esbroot {
namespace eve {
namespace ndcherenkov {


// -----   Default constructor   -------------------------------------------
EveEvent::EveEvent() 
{ 
}

EveEvent::EveEvent(const std::string& tree, const std::string& branch):IEvent(tree, branch)
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
        data::ndcherenkov::NDCherenkovDataPoint* point = (data::ndcherenkov::NDCherenkovDataPoint*)fdPoints->At(i);

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
        Int_t trackId = it->first; 
        core::eve::ITrack mapTrack = it->second;
        const std::vector<core::eve::ITrackPoint>& mapTrackPoints = mapTrack.getPoints();
        bool isExisting{false};
        for(int j = 0; j < tracks.size(); ++j)
        {
            core::eve::ITrack& tr = tracks[j];
            const std::vector<core::eve::ITrackPoint>& trPoints = tr.getPoints();
            if(!trPoints.empty() && trPoints[0].GetTrackId() == trackId){
                for(int p = 0; p < mapTrackPoints.size(); ++p)
                {
                    tr.addPoint(const_cast<core::eve::ITrackPoint&>(mapTrackPoints[p]));
                }
                isExisting = true;
            }
        }

        if(!isExisting)
            tracks.emplace_back(mapTrack);

        ++it;
    }

    return true;
}
// -------------------------------------------------------------------------
  

} //ndcherenkov
} //eve
} //esbroot
