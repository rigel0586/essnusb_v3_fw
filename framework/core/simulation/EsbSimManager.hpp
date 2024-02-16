#pragma once

#ifndef EsbSimManager_hpp
#define EsbSimManager_hpp 1

#include <vector>
#include <string>

#include "TObject.h"
#include "TGeoManager.h"

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4RunManager.hh"

#include "ESSnusbPhysicsList.hpp"
#include "EsbDetectorConstructor.hpp"
#include "IGenerator.hpp"

namespace esbroot {
namespace core {
namespace simulation {

enum class Severity : int
{
    nolog = 0,
    trace = 1,
    debug4 = 2,
    debug3 = 3,
    debug2 = 4,
    debug1 = 5,
    debug = 6,
    detail = 7,
    info = 8,
    state = 9,
    warn = 10,
    important = 11,
    alarm = 12,
    error = 13,
    fatal = 14,
};

class EsbSimManager : public TObject
{
public:
    EsbSimManager();
    ~EsbSimManager();

    void setNumberOfEvents(int n) {fEvents = n;}
    void setGenerator(generator::IGenerator* g) {fIGenerator = g;}
    void setTopVolume(TGeoVolume *vol);
    void setWorkingDir(const std::string& dirPath);

    void AddDetector(detector::IDetector* d);

    void setLoggerSeverity(Severity sev);
    void setOutputFile(const std::string& outPath);

    void run();

private:

    bool validate();
    int fEvents{0};
    
    std::vector<detector::IDetector*> fDetectors;
    std::string fWorkindDir{""};
    generator::IGenerator* fIGenerator{nullptr};
    
    ClassDef(EsbSimManager, 2);
};

} // namespace simulation
} // namespace core
} // namespace esbroot
#endif
