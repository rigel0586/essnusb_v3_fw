#include "EsbEveEventManager.hpp"
ClassImp(esbroot::core::eve::EsbEveEventManager)

#include "TGeoManager.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"
#include "TEveViewer.h"
#include "TSystem.h"

#include "TGLClip.h"
#include "TGLViewer.h"

#include "TEveManager.h"
#include "TEveGeoNode.h"
#include "TEveTrackPropagator.h"
#include "TEveTrack.h"
#include "TEvePathMark.h"

#include <fairlogger/Logger.h>

#include "Framework/ParticleData/PDGCodes.h"

namespace esbroot {
namespace core {
namespace eve {

std::map<int, EColor> EsbEveEventManager::m_trackColor = {
                                                            // Neutrinos
                                                            {genie::kPdgNuE , kWhite}, 
                                                            {genie::kPdgNuMu , kWhite}, 

                                                            {genie::kPdgAntiNuE , kWhite}, 
                                                            {genie::kPdgAntiNuMu , kWhite},

                                                            // Electron
                                                            {genie::kPdgElectron , kGreen},
                                                            {genie::kPdgPositron , kYellow},
                                                            
                                                            // Muon
                                                            {genie::kPdgMuon , kRed},
                                                            {genie::kPdgAntiMuon , kOrange},

                                                            // Nucleons
                                                            {genie::kPdgProton , kCyan},
                                                            {genie::kPdgNeutron , kPink},

                                                            // Photon
                                                            {genie::kPdgGamma , kGray},
                                                        };

void EsbEveEventManager::GotoEvent(Int_t event)
{
    if(gEve != nullptr && translateTracks(event)){
        m_currentEventId = event;
    }else{
        LOG(INFO) << "GotoEvent " << event << " does not exits";
    }
}

void EsbEveEventManager::NextEvent()
{
    int nextEventId = m_currentEventId + 1;
    if(gEve != nullptr && translateTracks(nextEventId)){
        m_currentEventId = nextEventId;
    }
}

void EsbEveEventManager::PrevEvent()
{
    int previousEventId = m_currentEventId - 1;
    if(gEve != nullptr && previousEventId >= 0 && translateTracks(previousEventId)){
        m_currentEventId = previousEventId;
    }
}

void EsbEveEventManager::AddEventTracks(int eventId, std::vector<ITrack>& tracks){
    LOG(INFO) << "AddEventTracks (Adding event) " << eventId << " tracks = " << tracks.size();

    auto it = m_tracks.find(eventId);
    if(it != m_tracks.end()) LOG(WARNING) << "Override AddEventTracks for eventId: " << eventId << " tracks = " << tracks.size();

    m_tracks[eventId] = tracks;
}

bool EsbEveEventManager::translateTracks(int eventId)
{
    auto it = m_tracks.find(eventId);
    if(it == m_tracks.end()) return false;

    RemoveElementsInternal();

    std::vector<ITrack>& tracks = it->second;

    TDatabasePDG* tDb =	TDatabasePDG::Instance();
    TEveTrackList* list = new TEveTrackList();
    std::string trackListName = "TEveTrackList event = " + std::to_string(eventId);
    list->SetName(trackListName.c_str());
    TEveTrackPropagator* prop =  list->GetPropagator();
    prop->SetFitDaughters(kTRUE);
    prop->SetStepper(TEveTrackPropagator::kRungeKutta);
    prop->SetMaxR(1000);
    prop->SetMaxZ(1000);
    prop->SetRnrReferences(kTRUE);
    prop->SetRnrDaughters(kTRUE);
    prop->SetRnrDecay(kTRUE);
    prop->RefPMAtt().SetMarkerStyle(4);

    const Int_t gamma_pdg = 22;
    for(int i = 0; i < tracks.size(); ++i)
    {
        ITrack& itrack = tracks[i];
        
        if(itrack.getPoints().empty()) continue;

        const ITrackPoint* firstPoint = &itrack.getPoints()[0];
        if(gamma_pdg == firstPoint->GetPgd()) continue; // Ignore photons

        TParticlePDG * tPar = tDb->GetParticle(firstPoint->GetPgd());
        
        if(tPar == nullptr) continue;
        Double_t charge = tPar->Charge()/3;
        LOG(INFO) << "Pdg " << firstPoint->GetPgd() << " charge " << charge << "color " << static_cast<int>(getColor(firstPoint->GetPgd()))
                    << "  E = " << (firstPoint->GetMomentum()).Mag();

        TEveRecTrackD* rc = new TEveRecTrackD();
        rc->fV.Set(firstPoint->GetPosition().X()
                    , firstPoint->GetPosition().Y()
                    , firstPoint->GetPosition().Z());

        rc->fP.Set(firstPoint->GetMomentum().X()
                    , firstPoint->GetMomentum().Y()
                    , firstPoint->GetMomentum().Z());
        rc->fSign = charge;

        TEveTrack* track = new TEveTrack(rc, prop);
        track->SetName(Form("%s", tPar->GetName()));
        if(!itrack.getPoints().empty()){
            const ITrackPoint* point = &itrack.getPoints()[0];
            track->SetPoint(0, point->GetPosition().X()
                            , point->GetPosition().Y()
                            , point->GetPosition().Z());

            TEvePathMarkD* pm1 = new TEvePathMarkD(TEvePathMarkD::kReference);
            pm1->fV.Set(point->GetPosition().X()
                    , point->GetPosition().Y()
                    , point->GetPosition().Z());

            pm1->fP.Set(point->GetMomentum().X()
                    , point->GetMomentum().Y()
                    , point->GetMomentum().Z());
            track->AddPathMark(*pm1);
        }
        
        for(int ti = 1; ti < itrack.getPoints().size(); ++ti)
        {
            TEvePathMarkD* pm1 = new TEvePathMarkD(TEvePathMarkD::kReference);
            const ITrackPoint* point = &itrack.getPoints()[ti];
            track->SetNextPoint(point->GetPosition().X()
                            , point->GetPosition().Y()
                            , point->GetPosition().Z());
            pm1->fV.Set(point->GetPosition().X()
                    , point->GetPosition().Y()
                    , point->GetPosition().Z());

            pm1->fP.Set(point->GetMomentum().X()
                    , point->GetMomentum().Y()
                    , point->GetMomentum().Z());
            track->AddPathMark(*pm1);
        }

        // track->SetRnrPoints(kTRUE);
        track->SetSmooth(kTRUE);

        track->SetMarkerStyle(4);
        //list->SetLineColor(getColor(firstPoint->GetPgd()), track);
        list->AddElement(track);
        // track->MakeTrack();
    }

    list->SetLineColor(kYellow);
    list->MakeTracks();

    TEveEventManager::AddElement(list);

    gEve->FullRedraw3D(kTRUE);

    return true;
}

EColor EsbEveEventManager::getColor(int pdg)
{
    auto it = m_trackColor.find(pdg);
    if(it == m_trackColor.end()) return kMagenta;

    return it->second;
}


} // namespace eve
} // namespace core
} // namespace esbroot