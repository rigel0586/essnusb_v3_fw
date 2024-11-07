#pragma once

#ifndef EsbIEvent_hpp
#define EsbIEvent_hpp 1

#include <string>
#include <vector>

#include "TObject.h"
#include "TClonesArray.h"
#include "ITrack.hpp"

namespace esbroot {
namespace core {
namespace eve {

class IEvent: public TObject
{
    public:
        IEvent() {}

        IEvent(const std::string& tree, const std::string& branch) : fTree(tree), fBranch(branch)  {}
        
        virtual bool Init() {return true;};

        virtual void beforeEvent(){};
        virtual void afterEvent(){};

        void AddTrackPoint(ITrackPoint point){ /* TODO */}
        virtual bool Exec(int eventId, TClonesArray* data, std::vector<ITrack>& tracks) = 0;

        virtual void beforeRun(){};
        virtual void afterRun(){};

        void  setName(std::string n){fname = n;}
        std::string  getName(){return fname;}

        std::string  getTree(){return fTree;}
        void  setTree(std::string n){fTree = n;}

        std::string  getBranch(){return fBranch;}
        void  setBranch(std::string n){fBranch = n;}

private:
    std::string fname;
    std::string fTree;
    std::string fBranch;

    ClassDef(IEvent, 2);
};

} // namespace eve
} // namespace core
} // namespace esbroot

#endif
