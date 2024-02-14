#include "EsbSimManager.hpp"
ClassImp(esbroot::core::simulation::EsbSimManager)

#include <fairlogger/Logger.h>
#include "EsbActionInitializer.hpp"
#include "ESSnusbPhysicsList.hpp"

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
        

        // initialize G4 kernel
        runManager->Initialize();

        // start a run
        runManager->BeamOn(fEvents);
    }
    catch(...){
        LOG(error) << "runManager encountered an error";
    }

    // job termination
    delete runManager;
    return;
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

} // namespace simulation
} // namespace core
} // namespace esbroot
