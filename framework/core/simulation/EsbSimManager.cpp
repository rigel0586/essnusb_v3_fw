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
    fRunManager = G4RunManagerFactory::CreateRunManager();
    // set mandatory initialization classes
    fRunManager->SetUserInitialization(new detector::EsbDetectorConstructor());
    fRunManager->SetUserInitialization(new physicsList::ESSnusbPhysicsList());

    // set user actions
    fRunManager->SetUserInitialization(new EsbActionInitializer());
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

    try{
        const G4VUserActionInitialization *  userAIL_const = fRunManager->GetUserActionInitialization();
        G4VUserActionInitialization * userAIL = const_cast<G4VUserActionInitialization*>(userAIL_const);
        EsbActionInitializer* esbAIL = static_cast<EsbActionInitializer*>(userAIL);
        esbAIL->setGenerator(fGenerator);

        const G4VUserDetectorConstruction *  detecCtr_const = fRunManager->GetUserDetectorConstruction();
        G4VUserDetectorConstruction * detecCtr = const_cast<G4VUserDetectorConstruction*>(detecCtr_const);
        detector::EsbDetectorConstructor* esbCtr = static_cast<detector::EsbDetectorConstructor*>(detecCtr);
        esbCtr->setWorkingDir(fWorkindDir);
        for(detector::IDetector* d : fDetectors){
            esbCtr->AddDetector(d);
        }


        // initialize G4 kernel
        fRunManager->Initialize();
        // start a run
        fRunManager->BeamOn(fEvents);
    }
    catch(...){
        LOG(error) << "runManager encountered an error";
    }
    
    // job termination
    delete fRunManager;
    fRunManager = nullptr;

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
    if(fGenerator == nullptr){
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
