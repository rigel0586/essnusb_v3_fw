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
#include "EmptyPhysicsList.hpp"
#include "EsbIO.hpp"

#include "EsbWriterPersistency.hpp"

#include "EmptyPrimaryGenerator.hpp"

#include "G4UIQt.hh"

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
        runManager->SetUserInitialization(new detector::EsbDetectorConstructor(fWorkindDir, fDetectors, fConverter));
        runManager->SetUserInitialization(new physicsList::ESSnusbPhysicsList(fcustomProcesses));

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

void EsbSimManager::displayGeometry(DisplayOption opt)
{
    if(opt == DisplayOption::ROOT_OGL || opt == DisplayOption::ROOT_TEVE )
        constructDisplayGeometryRoot(opt);

    if(opt == DisplayOption::GEANT4)
        constructDisplayGeometryG4();

    return;
}

void EsbSimManager::displayGeometry_fromFile(DisplayOption opt, const std::string& file)
{
    if(opt == DisplayOption::ROOT_OGL || opt == DisplayOption::ROOT_TEVE )
        displayGeometryUsingRoot(file, opt);

    if(opt == DisplayOption::GEANT4)
    {
        detector::EsbDetectorConstructor* dc = new detector::EsbDetectorConstructor(file);
        displayGeometryUsingG4(dc);
    }

    return;
}

void EsbSimManager::constructDisplayGeometryG4()
{
    detector::EsbDetectorConstructor* dc = new detector::EsbDetectorConstructor(fWorkindDir, fDetectors, fConverter);
    displayGeometryUsingG4(dc);
}


void EsbSimManager::displayGeometryUsingG4(detector::EsbDetectorConstructor* dc)
{
    G4RunManager* rm = new G4RunManager;
    dc->isView(true);
    rm->SetUserInitialization(dc);
    rm->SetUserInitialization(new physicsList::EmptyPhysicsList());
    rm->SetUserAction(new generator::EmptyPrimaryGenerator());
    rm->Initialize();

    // Visualization
    G4VisManager* vm = new G4VisExecutive("quiet");
    vm->Initialize();

    G4int argc = 1;
    char **argv{nullptr};
    int visId = static_cast<int>(core::simulation::G4_vis::qt);
    char* type = const_cast<char*>(fvisSessionTypes[visId].c_str());
    argv = &type;

    // Start user interface
    G4UIQt* ui = new G4UIQt(argc, argv);
    ui->GetUserInterfaceWidget()->setVisible(false);
    ui->GetCoutDockWidget()->setVisible(false);
    G4UImanager* um = G4UImanager::GetUIpointer();
    um->ApplyCommand("/vis/open OGLSQt 1200x800");
    um->ApplyCommand("/vis/drawVolume worlds");
    um->ApplyCommand("/vis/scene/add/axes 0 0 0");
    ui->AddMenu("views", "Views");
    ui->AddButton("views", "Front view (+Z)", "/vis/viewer/set/viewpointThetaPhi 180   0 deg");
    ui->AddButton("views", "Rear view (-Z)",  "/vis/viewer/set/viewpointThetaPhi   0   0 deg");
    ui->AddButton("views", "Right view (+X)", "/vis/viewer/set/viewpointThetaPhi +90 180 deg");
    ui->AddButton("views", "Left view (-X)",  "/vis/viewer/set/viewpointThetaPhi -90 180 deg");
    ui->AddButton("views", "Bottom view (+Y)","/vis/viewer/set/viewpointThetaPhi -90  90 deg");
    ui->AddButton("views", "Top view (-Y)",   "/vis/viewer/set/viewpointThetaPhi +90  90 deg");
    ui->AddMenu("options", "Options");
    ui->AddButton("options", "NbOfSides 24","/vis/viewer/set/lineSegmentsPerCircle 24");
    ui->AddButton("options", "NbOfSides 360","/vis/viewer/set/lineSegmentsPerCircle 360");
    ui->AddMenu("axes", "Axes");
    ui->AddButton("axes", "Add axes", "/vis/scene/add/axes 0 0 0");
    ui->AddButton("axes", "Hide axes", "/vis/scene/activateModel G4AxesModel false");
    ui->AddIcon("Front view (+Z)",  "user_icon", "/vis/viewer/set/viewpointThetaPhi 180   0 deg \n /vis/viewer/set/upVector 0 1 0", "TechDraw_ProjFront.xpm");
    ui->AddIcon("Rear view (-Z)",   "user_icon", "/vis/viewer/set/viewpointThetaPhi   0   0 deg \n /vis/viewer/set/upVector 0 1 0", "TechDraw_ProjRear.xpm");
    ui->AddIcon("Right view (+X)",  "user_icon", "/vis/viewer/set/viewpointThetaPhi +90 180 deg \n /vis/viewer/set/upVector 0 1 0", "TechDraw_ProjRight.xpm");
    ui->AddIcon("Left view (-X)",   "user_icon", "/vis/viewer/set/viewpointThetaPhi -90 180 deg \n /vis/viewer/set/upVector 0 1 0", "TechDraw_ProjLeft.xpm");
    ui->AddIcon("Bottom view (+Y)", "user_icon", "/vis/viewer/set/viewpointThetaPhi -90  90 deg \n /vis/viewer/set/upVector 1 0 0", "TechDraw_ProjBottom.xpm");
    ui->AddIcon("Top view (-Y)",    "user_icon", "/vis/viewer/set/viewpointThetaPhi +90  90 deg \n /vis/viewer/set/upVector 1 0 0", "TechDraw_ProjTop.xpm");
    ui->SessionStart();

    delete ui;
    delete vm;
    delete rm;
}



void EsbSimManager::constructDisplayGeometryRoot(DisplayOption opt)
{
    detector::EsbDetectorConstructor* dc = new detector::EsbDetectorConstructor(fWorkindDir, fDetectors, fConverter);
    G4VPhysicalVolume* g4worldVol = dc->Create();

    if(g4worldVol == nullptr)
    {
         LOG(error) << "Unable to create root geometry for display...";
         return;
    }

    displayGeometryUsingRoot(dc->getRootGeomFile(), opt);
}


void EsbSimManager::displayGeometryUsingRoot(const std::string& file, DisplayOption opt)
{
    io::EsbIO fio;
    // std::string fileRoot = fWorkindDir + "/" + fDisplayFile;
    // fio.ExportG4VolumeVGM(fileRoot, g4worldVol); // Empty path exports into root only, no file
    if(!fio.ImportTGeoVolume(file))
    {
         LOG(error) << "Unable to create root geometry for display...";
         return;
    }

    if(opt == DisplayOption::ROOT_TEVE)
    {
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

    if(opt == DisplayOption::ROOT_OGL)
    {
        TGeoVolume *top = gGeoManager->GetTopVolume();
        top->Draw("ogl");
    }
    

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

void EsbSimManager::AddCustomPhysicsList(G4VPhysicsConstructor* pr)
{
    fcustomProcesses.emplace_back(pr);
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
