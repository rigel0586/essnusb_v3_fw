#ifndef ESBROOT_GENERATOR_GENERIC_IFLUXNEXTPOSITION_H
#define ESBROOT_GENERATOR_GENERIC_IFLUXNEXTPOSITION_H 1

#include <memory>
#include "TObject.h"
#include "TVector3.h"
#include <vector>

namespace esbroot {

namespace generators {

namespace generic {


class IFluxNextPosition : public TObject {

public:
  
    virtual TVector3 NextVertexPosition() = 0;
private:

    ClassDef(IFluxNextPosition, 6)
};

class CompositeIFluxNextPosition : public IFluxNextPosition, public TObject
{
public:
  
    CompositeIFluxNextPosition(std::vector<IFluxNextPosition*> fluxes) : fFluxes(fluxes){};
    
    virtual TVector3 NextVertexPosition() override;

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
