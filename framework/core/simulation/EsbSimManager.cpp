#include "EsbSimManager.hpp"
ClassImp(esbroot::core::simulation::EsbSimManager)

#include "TEveManager.h"
#include "TEveGeoNode.h"
#include "TEveViewer.h"
#include "TSystem.h"
#include "TGLClip.h"
#include "TGLViewer.h"

#include <fairlogger/Logger.h>
#include "EsbActionInitializer.hpp"
#include "ESSnusbPhysicsList.hpp"

#include "EsbWriterPersistency.hpp"

namespace esbroot {
namespace core {
namespace simulation {


EsbSimManager::EsbSimManager()
{
}

EsbSimManager::~EsbSimManager()
{
}

void EsbSimManager::setLoggerSeverity(Severity sev){
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


void EsbSimManager::run()
{
    if(!validate()){
        return;
    }

    auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);
    try{
        //runManager->SetVerboseLevel(10);

        // set mandatory initialization classes
        runManager->SetUserInitialization(new detector::EsbDetectorConstructor(fWorkindDir, fDetectors));
        runManager->SetUserInitialization(new physicsList::ESSnusbPhysicsList());

        // set user actions
        runManager->SetUserInitialization(new EsbActionInitializer(fIGenerator, fDetectors)); 
        

        if(fuseVis == G4_vis::none){
            // initialize G4 kernel
            runManager->Initialize();

            // start a run
            runManager->BeamOn(fEvents);
        } else{
            // Detect interactive mode (if no arguments) and define UI session
            G4int argc = 1;
            char **argv{nullptr};
            int visId = static_cast<int>(fuseVis);
            char* type = const_cast<char*>(fvisSessionTypes[visId].c_str());
            argv = &type;
            G4UIExecutive* ui = 0;
            if ( argc == 1 ) {
                ui = new G4UIExecutive(argc, argv);
            }

            // Initialise visualization
            auto* visManager = new G4VisExecutive;
            visManager->Initialize();

            // Get the Pointer to the UI Manager
            auto* UImanager = G4UImanager::GetUIpointer();

            // User interactions
            // Define (G)UI for interactive mode
            if(argc==1)
            {
                std::string path = "/control/macroPath " + fvisMacroPath;
                UImanager->ApplyCommand(path.c_str());

                std::string file = "/control/execute " + fvisMacro;
                UImanager->ApplyCommand(file.c_str());
                ui->SessionStart();
                delete ui;
            }
            else  // Batch mode
            {
                G4String command = "/control/execute ";
                G4String fileName = argv[1];
                UImanager->ApplyCommand(command+fileName);
            }
            delete visManager;
        }
    }
    catch(...){
        LOG(error) << "runManager encountered an error";
    }

    // job termination
    delete runManager;
    return;
}

void EsbSimManager::displayGeometry()
{
    for(detector::IDetector* d : fDetectors)
    {
        d->ConstructGeometry(); // Every detector constructs its volumes and add it to the Top volume
    }

    TEveManager::Create();

    TFile::SetCacheFileDir(".");
    gGeoManager->DefaultColors();

    auto node1 = gGeoManager->GetTopNode();
    TEveGeoTopNode* inn = new TEveGeoTopNode(gGeoManager, node1);
    gEve->AddGlobalElement(inn);

    gEve->FullRedraw3D(kTRUE);

    // EClipType not exported to CINT (see TGLUtil.h):
    // 0 - no clip, 1 - clip plane, 2 - clip box
    auto v = gEve->GetDefaultGLViewer();
    v->GetClipSet()->SetClipType(TGLClip::EType(1));
    v->RefreshPadEditor(v);

    v->CurrentCamera().RotateRad(-.7, 0.5);
    v->DoDraw();

}

void EsbSimManager::setWorkingDir(const std::string& dirPath)
{
    fWorkindDir = dirPath;
}

void EsbSimManager::setTopVolume(TGeoVolume *vol)
{
    gGeoManager->SetTopVolume(vol);
}

void EsbSimManager::AddDetector(detector::IDetector* d)
{
    fDetectors.emplace_back(d);
}

bool EsbSimManager::validate(){
    if(fIGenerator == nullptr){
        LOG(error) << "G4VUserPrimaryGeneratorAction is not set";
        return false;
    }

    if(fWorkindDir.empty()){
        LOG(error) << "Working directory is not set";
        return false;
    }

    return true;
}

void EsbSimManager::setOutputFile(const std::string& outPath)
{
    io::EsbWriterPersistency::Instance().setOutFile(outPath);
}

} // namespace simulation
} // namespace core
} // namespace esbroot
