#pragma once

#ifndef EsbSimManager_hpp
#define EsbSimManager_hpp 1

#include <string>
#include "TObject.h"
#include "TGeoManager.h"

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"

#include "ESSnusbPhysicsList.hpp"
#include "EsbDetectorConstructor.hpp"

namespace esbroot {
namespace core {
namespace simulation {

class EsbSimManager : public TObject
{
public:
    EsbSimManager();
    ~EsbSimManager();

    detector::EsbDetectorConstructor* getDetectorConstructor(){return fDetectorConstructor;}

    void setNumberOfEvents(int n) {fEvents = n;}
    void setGenerator(G4VUserPrimaryGeneratorAction* g) {fGenerator = g;}
    void setTopVolume(TGeoVolume *vol);
    void setWorkingDir(const std::string& dirPath);

    void AddDetector(IDetector* d);

    void run();

private:

    bool validate();

    int fEvents{0};

    detector::EsbDetectorConstructor* fDetectorConstructor{nullptr};
    bool isEsbDtrAdded{false};
    physicsList::ESSnusbPhysicsList* fPhysicsList{nullptr};
    bool isEsbPhListAdded{false};

    G4VUserPrimaryGeneratorAction* fGenerator{nullptr};
    
    ClassDef(EsbSimManager, 2);
};

} // namespace simulation
} // namespace core
} // namespace esbroot
#endif
