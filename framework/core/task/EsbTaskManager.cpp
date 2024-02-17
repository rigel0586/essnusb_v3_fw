#include "EsbTaskManager.hpp"
ClassImp(esbroot::core::task::EsbTaskManager)

#include <fairlogger/Logger.h>
#include "EsbWriterPersistency.hpp"

namespace esbroot {
namespace core {
namespace task {


EsbTaskManager::EsbTaskManager(const std::string& inputFile
                    , const std::string& tree
                    , const std::string& branch) 
    : finputFile(inputFile),
        fTree(tree),
        fBranch(branch)
{
    io::EsbReaderPersistency::Instance().setInFile(finputFile);
    io::EsbReaderPersistency::Instance().Register(fTree.c_str(), fBranch.c_str(), &fReadItem);
}

EsbTaskManager::~EsbTaskManager()
{
}

void EsbTaskManager::setLoggerSeverity(Severity sev){
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


void EsbTaskManager::run()
{
    if(!Init()){
        return;
    }

    beforeRun();

    bool rc{true};
    for(int event = fStartEvents; rc && (event < fEvents); ++event){
        TTree* ptr_tree = fReadItem.fTree;
        ptr_tree->GetEntry(event);
        for(int i = 0; rc && (i < fTasks.size()); ++i){
            fTasks[i]->beforeEvent();
            rc = fTasks[i]->Exec(event, fReadItem.fColl);
            fTasks[i]->afterEvent();

            if(!rc){
                LOG(fatal) << "Task  " << fTasks[i]->getName() << " failed to Execute successfully";
            }
        }
    }
    
    afterRun();
}

void EsbTaskManager::addTask(ITask* task)
{
    fTasks.emplace_back(task);
}

bool EsbTaskManager::Init()
{
    if(fReadItem.fEntries < fEvents){
        LOG(fatal) << "Input file  " << finputFile << " has fewer entries than requested" 
                    << "[ Requested " << fEvents << " , available " << fReadItem.fEntries;
        return false;
    }

    bool rc{true};
    for(int i = 0; rc && (i < fTasks.size()); ++i)
    {
        rc = fTasks[i]->Init();
        if(!rc){
            LOG(fatal) << "Task  " << fTasks[i]->getName() << " failed to initialize";
        }
    }
    return rc;
}

void EsbTaskManager::beforeRun()
{
    for(int i = 0; i < fTasks.size(); ++i){
        fTasks[i]->beforeRun();
    }
}

void EsbTaskManager::afterRun()
{
    for(int i = 0; i < fTasks.size(); ++i){
        fTasks[i]->afterRun();
    }
    io::EsbWriterPersistency::Instance().writeData();
}

void EsbTaskManager::setOutputFile(const std::string& outPath)
{
    io::EsbWriterPersistency::Instance().setOutFile(outPath);
}

} // namespace task
} // namespace core
} // namespace esbroot
