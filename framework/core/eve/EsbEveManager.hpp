#pragma once

#ifndef EsbEveManager_hpp
#define EsbEveManager_hpp 1

#include <vector>
#include <string>

#include "IEvent.hpp"
#include "EsbReaderPersistency.hpp"

namespace esbroot {
namespace core {
namespace eve {

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

class EsbEveManager : public TObject
{
public:
    EsbEveManager(const std::string& inputGeomFile
                    , const std::string& inputFile
                    , const std::string& tree
                    , const std::string& branch);
    ~EsbEveManager();

    void setLoggerSeverity(Severity sev);
    void setNumberOfEvents(int n) {fEvents = n;}
    void setStartOfEvents(int n) {fStartEvents = n;}
    void addEvent(IEvent* event);

    void setOutputFile(const std::string& outPath);
    void run();
    void visualize();

private:

    int fEvents{0};
    int fStartEvents{0};
    
    std::vector<IEvent*> fIEvents;
    const std::string finputFile;
    const std::string finputGeomFile;
    const std::string fTree;
    const std::string fBranch;

    io::EsbReaderPersistency::ReadItem fReadItem;

    bool Init();

    void beforeRun();
    void afterRun();
    
    ClassDef(EsbEveManager, 2);
};

} // namespace eve
} // namespace core
} // namespace esbroot
#endif
