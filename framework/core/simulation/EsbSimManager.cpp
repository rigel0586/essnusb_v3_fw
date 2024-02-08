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
//     if(fDetectorConstructor!=nullptr)
//         delete fDetectorConstructor;

//     if(fPhysicsList!=nullptr)
//         delete fPhysicsList;
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
        runManager->SetUserInitialization(new physicsList::ESSnusbPhysicsList());

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

bool EsbSimManager::validate(){
    if(fGenerator == nullptr){
        LOG(error) << "G4VUserPrimaryGeneratorAction is not set";
        return false;
    }

    return true;
}

} // namespace simulation
} // namespace core
} // namespace esbroot
