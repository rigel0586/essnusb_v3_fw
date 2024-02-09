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
    fDetectorConstructor = new detector::EsbDetectorConstructor();
    fPhysicsList = new physicsList::ESSnusbPhysicsList();
}

EsbSimManager::~EsbSimManager()
{
    // If added to geant4 manager, it will delete it
    if(!isEsbDtrAdded && fDetectorConstructor!=nullptr)
        delete fDetectorConstructor;

    if(!isEsbPhListAdded && fPhysicsList!=nullptr)
        delete fPhysicsList;
}

void EsbSimManager::run()
{
    if(!validate()){
        return;
    }

    EsbActionInitializer* actionInit = nullptr;
    // construct the default run manager
    auto runManager = G4RunManagerFactory::CreateRunManager();

    try{
        // set mandatory initialization classes
        runManager->SetUserInitialization(fDetectorConstructor);
        isEsbDtrAdded = true;
        
        runManager->SetUserInitialization(fPhysicsList);
        isEsbPhListAdded = true;

        actionInit = new EsbActionInitializer();
        actionInit->setGenerator(fGenerator);
        runManager->SetUserInitialization(actionInit);

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
    // delete actionInit;
    return;
}

void EsbSimManager::setWorkingDir(const std::string& dirPath)
{
    fDetectorConstructor->setWorkingDir(dirPath);
}

void EsbSimManager::setTopVolume(TGeoVolume *vol)
{
    gGeoManager->SetTopVolume(vol);
}

void EsbSimManager::AddDetector(IDetector* d)
{
    fDetectorConstructor->AddDetector(d);
}

bool EsbSimManager::validate(){
    if(fGenerator == nullptr){
        LOG(error) << "G4VUserPrimaryGeneratorAction is not set";
        return false;
    }

    if(fDetectorConstructor.getWorkingDir().empty()){
        LOG(error) << "Working directory is not set";
        return false;
    }

    return true;
}

} // namespace simulation
} // namespace core
} // namespace esbroot
