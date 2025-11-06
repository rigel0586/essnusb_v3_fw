#ifndef ESBROOT_GENERATOR_GENERIC_IFLUXNEXTGENERATOR_H
#define ESBROOT_GENERATOR_GENERIC_IFLUXNEXTGENERATOR_H 1

#include <memory>
#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include <vector>

namespace esbroot {

namespace generators {

namespace generic {


class IFluxNextGenerator : public TObject {

public:
  
    // Return values are interpreted as meters!
    virtual TVector3 NextVertexPosition() = 0;

    virtual TLorentzVector NextVertexMomentum() = 0;
    virtual int NextPdgNuCode() = 0;
    
private:

    ClassDef(IFluxNextGenerator, 6)
};

class CompositeIFluxNextGenerator: public IFluxNextGenerator, public TObject
{
public:
  
    CompositeIFluxNextGenerator(std::vector<IFluxNextGenerator*> fluxes) : fFluxes(fluxes){};
    
    virtual TVector3 NextVertexPosition() override;
    virtual TLorentzVector NextVertexMomentum() override;
    virtual int NextPdgNuCode() override;

    void increment();
private:
    int idx{0};
    int counter{0};
    std::vector<IFluxNextGenerator*> fFluxes;
    ClassDef(CompositeIFluxNextGenerator, 6)
};


inline TVector3 CompositeIFluxNextGenerator::NextVertexPosition()
{
    if(fFluxes.empty()) return TVector3{};
    return fFluxes[idx]->NextVertexPosition();
}

inline TLorentzVector CompositeIFluxNextGenerator::NextVertexMomentum()
{
    if(fFluxes.empty()) return TLorentzVector{};
    return fFluxes[idx]->NextVertexMomentum();
}

inline int CompositeIFluxNextGenerator::NextPdgNuCode()
{
    if(fFluxes.empty()) return 0;
    return fFluxes[idx]->NextPdgNuCode();
}

inline void CompositeIFluxNextGenerator::increment()
{
    if(fFluxes.empty()) return;
    ++counter;
    idx = counter % fFluxes.size();
}

} // namespace generic

} // namespace generators

} // namespace esbroot
#endif
