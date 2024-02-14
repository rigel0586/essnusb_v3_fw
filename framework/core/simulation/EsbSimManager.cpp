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
    // TODO fix severity
    fair::Logger::SetConsoleSeverity(fair::Severity::info);
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
