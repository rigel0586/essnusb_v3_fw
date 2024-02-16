#pragma once

#ifndef EsbTaskManager_hpp
#define EsbTaskManager_hpp 1

#include <vector>
#include <string>

#include "ITask.hpp"

namespace esbroot {
namespace core {
namespace task {

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

class EsbTaskManager : public TObject
{
public:
    EsbTaskManager(const std::string& inputFile);
    ~EsbTaskManager();

    void setLoggerSeverity(Severity sev);
    void setNumberOfEvents(int n) {fEvents = n;}
    void addTask(ITask* task);

    void setOutputFile(const std::string& outPath);
    void run();

private:

    int fEvents{0};
    
    std::vector<ITask*> fTasks;
    const std::string finputFile;

    bool Init();

    void beforeRun();
    void afterRun();
    
    ClassDef(EsbTaskManager, 2);
};

} // namespace task
} // namespace core
} // namespace esbroot
#endif