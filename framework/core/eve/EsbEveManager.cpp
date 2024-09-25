#include "EsbEveManager.hpp"
ClassImp(esbroot::core::eve::EsbEveManager)

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

    TEveManager::Create();
    gEve->AddEvent(&m_esbEveEventManager);

    beforeRun();

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

        if(rc){
            m_esbEveEventManager.AddEventTracks(event, ltracks);
        }
    }
    
    afterRun();
}


void EsbEveManager::goToEvent(int eventDIsplayID)
{
    m_esbEveEventManager.GotoEvent(eventDIsplayID);

}

void EsbEveManager::visualize()
{
    TFile::SetCacheFileDir(".");
    gGeoManager = gEve->GetGeometry(finputGeomFile);
    gGeoManager->DefaultColors();

    auto node1 = gGeoManager->GetTopNode();
    TEveGeoTopNode* inn = new TEveGeoTopNode(gGeoManager, node1);
    gEve->AddGlobalElement(inn);

    gEve->FullRedraw3D(kTRUE);

    // EClipType not exported to CINT (see TGLUtil.h):
    // 0 - no clip, 1 - clip plane, 2 - clip box
    // auto v = gEve->GetDefaultGLViewer();
    // v->GetClipSet()->SetClipType(TGLClip::EType(1));
    // v->RefreshPadEditor(v);

    // v->CurrentCamera().RotateRad(-.7, 0.5);
    // v->DoDraw();

    TEveViewer *ev = gEve->GetDefaultViewer();
    TGLViewer  *gv = ev->GetGLViewer();
    gv->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kFALSE, nullptr);
    
    gEve->Redraw3D(kTRUE);
    gSystem->ProcessEvents();
    
    gv->CurrentCamera().RotateRad(-0.5, 1.4);
    gv->RequestDraw();
}

void EsbEveManager::addEvent(IEvent* event)
{
    fIEvents.emplace_back(event);
}

bool EsbEveManager::Init()
{
   if(fReadItem.fEntries < fEvents){
       LOG(fatal) << "Input file  " << finputFile << " has fewer entries than requested" 
                   << "[ Requested " << fEvents << " , available " << fReadItem.fEntries;
       return false;
   }

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
