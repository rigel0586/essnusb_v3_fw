#ifndef ESBROOT_GENERATOR_GENERIC_IFLUXCOMPOSITENEXTPOSITION_H
#define ESBROOT_GENERATOR_GENERIC_IFLUXCOMPOSITENEXTPOSITION_H 1

#include <memory>
#include "TObject.h"
#include "TVector3.h"
#include <vector>

#include "generators/generic/IFluxNextPosition.h"

namespace esbroot {

namespace generators {

namespace generic {

class CompositeIFluxNextPosition : public IFluxNextPosition
{
public:
  
    CompositeIFluxNextPosition(std::vector<IFluxNextPosition*> fluxes) : fFluxes(fluxes){};
    
    virtual TVector3 NextVertexPosition() override;
    virtual void WriteEvent(const genie::EventRecord *event) override;

    void increment();
private:
    int idx{0};
    int counter{0};
    std::vector<IFluxNextPosition*> fFluxes;
    ClassDef(CompositeIFluxNextPosition, 6)
};


inline TVector3 CompositeIFluxNextPosition::NextVertexPosition()
{
    if(fFluxes.empty()) return TVector3{};
    return fFluxes[idx]->NextVertexPosition();
}

inline void CompositeIFluxNextPosition::WriteEvent(const genie::EventRecord *event)
{
    if(fFluxes.empty()) return ;
    return fFluxes[idx]->WriteEvent(event);
}

inline void CompositeIFluxNextPosition::increment()
{
    if(fFluxes.empty()) return;
    ++counter;
    idx = counter % fFluxes.size();
}

} // namespace generic

} // namespace generators

} // namespace esbroot
#endif
