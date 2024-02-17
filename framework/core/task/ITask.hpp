#pragma once

#ifndef EsbITask_hpp
#define EsbITask_hpp 1

#include <string>
#include "TObject.h"
#include "TClonesArray.h"

namespace esbroot {
namespace core {
namespace task {

class ITask : public TObject
{
    public:
        ITask(std::string name) : fname(name){}
        virtual ~ITask() = default;

        virtual bool Init() = 0;

        virtual void beforeEvent(){};
        virtual void afterEvent(){};

        virtual bool Exec(int eventId, TClonesArray* data) = 0;

        virtual void beforeRun(){};
        virtual void afterRun(){};

        const std::string& getName(){return fname;}

private:
    std::string fname{""};

    ClassDef(ITask, 2);
};

} // namespace task
} // namespace core
} // namespace esbroot

#endif
