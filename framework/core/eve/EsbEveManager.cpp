#include "EsbEveManager.hpp"
ClassImp(esbroot::core::eve::EsbEveManager)

#include "TGeoManager.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"

#include "TGLClip.h"
#include "TGLViewer.h"

#include "TEveManager.h"
#include "TEveGeoNode.h"
#include "TEveTrackPropagator.h"
#include "TEveTrack.h"
#include "TEvePathMark.h"

#include <fairlogger/Logger.h>
#include "EsbWriterPersistency.hpp"

namespace esbroot {
namespace core {
namespace eve {

// typedef TEvePathMarkD TEvePathMarkT<double>;
using TEvePathMarkD = TEvePathMarkT<double>;

EsbEveManager::EsbEveManager(const std::string& inputGeomFile
                    , const std::string& inputFile
                    , const std::string& tree
                    , const std::string& branch) 
    : finputGeomFile(inputGeomFile),
        finputFile(inputFile),
        fTree(tree),
        fBranch(branch)
{
    io::EsbReaderPersistency::Instance().setInFile(finputFile);
    io::EsbReaderPersistency::Instance().Register(fTree.c_str(), fBranch.c_str(), &fReadItem);
}

EsbEveManager::~EsbEveManager()
{
}

void EsbEveManager::setLoggerSeverity(Severity sev){
    switch(sev)
    {
        case Severity::nolog: fair::Logger::SetConsoleSeverity(fair::Severity::nolog);
                    break;

        case Severity::trace: fair::Logger::SetConsoleSeverity(fair::Severity::trace);
                    break;
        
        case Severity::debug4: fair::Logger::SetConsoleSeverity(fair::Severity::debug4);
                    break;

        case Severity::debug3: fair::Logger::SetConsoleSeverity(fair::Severity::debug3);
                    break;

        case Severity::debug2: fair::Logger::SetConsoleSeverity(fair::Severity::debug2);
                    break;

        case Severity::debug1: fair::Logger::SetConsoleSeverity(fair::Severity::debug1);
                    break;

        case Severity::debug: fair::Logger::SetConsoleSeverity(fair::Severity::debug);
                    break;

        case Severity::detail: fair::Logger::SetConsoleSeverity(fair::Severity::detail);
                    break;

        case Severity::info: fair::Logger::SetConsoleSeverity(fair::Severity::info);
                    break;

        case Severity::state: fair::Logger::SetConsoleSeverity(fair::Severity::state);
                    break;

        case Severity::warn: fair::Logger::SetConsoleSeverity(fair::Severity::warn);
                    break;

        case Severity::important: fair::Logger::SetConsoleSeverity(fair::Severity::important);
                    break;

        case Severity::alarm: fair::Logger::SetConsoleSeverity(fair::Severity::alarm);
                    break;

        case Severity::error: fair::Logger::SetConsoleSeverity(fair::Severity::error);
                    break;

        case Severity::fatal: fair::Logger::SetConsoleSeverity(fair::Severity::fatal);
                    break;

        default:    fair::Logger::SetConsoleSeverity(fair::Severity::nolog);
                    break;
    }

    fair::Logger::SetConsoleColor(true);
}


void EsbEveManager::run()
{
    if(!Init()){
        return;
    }

    beforeRun();

    std::vector<TEveEventManager*> tEvents;
    bool rc{true};
    for(int event = fStartEvents; rc && (event < fEvents); ++event){
        TTree* ptr_tree = fReadItem.fTree;
        ptr_tree->GetEntry(event);

        std::vector<ITrack> ltracks;

        for(int i = 0; rc && (i < fIEvents.size()); ++i){
            fIEvents[i]->beforeEvent();
            rc = fIEvents[i]->Exec(event, fReadItem.fColl, ltracks);
            fIEvents[i]->afterEvent();

            if(!rc){
                LOG(warning) << "Event  " << fIEvents[i]->getName() << " failed to Execute successfully";
            }
        }

        TEveEventManager* te = addTracks(event, ltracks);
        tEvents.emplace_back(te);
    }
    
    afterRun();
    visualize(tEvents);
}

void EsbEveManager::visualize(std::vector<TEveEventManager*>&  eventList)
{
    TEveManager::Create();

    TFile::SetCacheFileDir(".");
    gGeoManager = gEve->GetGeometry(finputGeomFile);
    gGeoManager->DefaultColors();

    auto node1 = gGeoManager->GetTopNode();
    TEveGeoTopNode* inn = new TEveGeoTopNode(gGeoManager, node1);
    gEve->AddGlobalElement(inn);

    gEve->FullRedraw3D(kTRUE);

    auto it = eventList.rbegin();
    while(it != eventList.rend())
    {
        gEve->AddEvent(*it);
        ++it;
    }

    // EClipType not exported to CINT (see TGLUtil.h):
    // 0 - no clip, 1 - clip plane, 2 - clip box
    auto v = gEve->GetDefaultGLViewer();
    v->GetClipSet()->SetClipType(TGLClip::EType(1));
    v->RefreshPadEditor(v);

    v->CurrentCamera().RotateRad(-.7, 0.5);
    v->DoDraw();
}

TEveEventManager* EsbEveManager::addTracks(int eventId, std::vector<ITrack>& tracks)
{
    TDatabasePDG* tDb =	TDatabasePDG::Instance();
    TEveTrackList* list = new TEveTrackList();
    std::string trackListName = "TEveTrackList event = " + std::to_string(eventId);
    list->SetName(trackListName.c_str());
    TEveTrackPropagator* prop =  list->GetPropagator();
    prop->SetFitDaughters(kFALSE);

    for(int i = 0; i < tracks.size(); ++i)
    {
        

        ITrack& itrack = tracks[i];
        
        if(itrack.getPoints().empty()) continue;

        const ITrackPoint* firstPoint = &itrack.getPoints()[0];
        TParticlePDG * tPar = tDb->GetParticle(firstPoint->GetPgd());

        if(tPar == nullptr) continue;
        
        TEveRecTrackD* rc = new TEveRecTrackD();
        rc->fV.Set(firstPoint->GetPosition().X()
                    , firstPoint->GetPosition().Y()
                    , firstPoint->GetPosition().Z());

        rc->fP.Set(firstPoint->GetMomentum().X()
                    , firstPoint->GetMomentum().Y()
                    , firstPoint->GetMomentum().Z());
        rc->fSign = tPar->Charge();

        TEveTrack* track = new TEveTrack(rc, prop);
        track->SetName(Form("Charge %d", tPar->Charge()));
        
        for(int ti = 0; ti < itrack.getPoints().size(); ++ti)
        {
            TEvePathMarkD* pm1 = new TEvePathMarkD(TEvePathMarkD::kLineSegment);
            const ITrackPoint* point = &itrack.getPoints()[ti];
            pm1->fV.Set(point->GetPosition().X()
                    , point->GetPosition().Y()
                    , point->GetPosition().Z());

            pm1->fP.Set(point->GetMomentum().X()
                    , point->GetMomentum().Y()
                    , point->GetMomentum().Z());
            track->AddPathMark(*pm1);
        }

        track->SetMarkerStyle(4);
        list->SetLineColor(kMagenta);
        list->AddElement(track);
        track->MakeTrack();
    }
    
    TEveEventManager* currentEvent = new TEveEventManager();
    currentEvent->AddElement(list);

    return currentEvent;
}

void EsbEveManager::addEvent(IEvent* event)
{
    fIEvents.emplace_back(event);
}

bool EsbEveManager::Init()
{
//    if(fReadItem.fEntries < fEvents){
//        LOG(fatal) << "Input file  " << finputFile << " has fewer entries than requested" 
//                    << "[ Requested " << fEvents << " , available " << fReadItem.fEntries;
//        return false;
//    }

    bool rc{true};
    for(int i = 0; rc && (i < fIEvents.size()); ++i)
    {
        rc = fIEvents[i]->Init();
        if(!rc){
            LOG(fatal) << "Event  " << fIEvents[i]->getName() << " failed to initialize";
        }
    }
    return rc;
}

void EsbEveManager::beforeRun()
{
    for(int i = 0; i < fIEvents.size(); ++i){
        fIEvents[i]->beforeRun();
    }
}

void EsbEveManager::afterRun()
{
    for(int i = 0; i < fIEvents.size(); ++i){
        fIEvents[i]->afterRun();
    }
    io::EsbWriterPersistency::Instance().writeData();
}

void EsbEveManager::setOutputFile(const std::string& outPath)
{
    io::EsbWriterPersistency::Instance().setOutFile(outPath);
}

} // namespace eve
} // namespace core
} // namespace esbroot
