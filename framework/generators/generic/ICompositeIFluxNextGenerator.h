#ifndef ESBROOT_GENERATOR_GENERIC_IFLUXNEXTCOMPOSITEGENERATOR_H
#define ESBROOT_GENERATOR_GENERIC_IFLUXNEXTCOMPOSITEGENERATOR_H 1

#include <memory>
#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include <vector>

#include "generators/generic/IFluxNextGenerator.h"

namespace esbroot {

namespace generators {

namespace generic {

class CompositeIFluxNextGenerator: public IFluxNextGenerator
{
public:
  
    CompositeIFluxNextGenerator(std::vector<IFluxNextGenerator*> fluxes) : fFluxes(fluxes){};
    
    virtual bool NextPosMomPdg(TVector3& position, TLorentzVector& momentum, int& pdgCode) override;
    virtual void WriteEvent(const genie::EventRecord *event) override;

    void increment();
private:
    int idx{0};
    int counter{0};
    std::vector<IFluxNextGenerator*> fFluxes;
    ClassDef(CompositeIFluxNextGenerator, 6)
};

inline bool CompositeIFluxNextGenerator::NextPosMomPdg(TVector3& position, TLorentzVector& momentum, int& pdgCode) 
{
    if(fFluxes.empty()) return false;
    return fFluxes[idx]->NextPosMomPdg(position, momentum, pdgCode);
}

inline void CompositeIFluxNextGenerator::WriteEvent(const genie::EventRecord *event)
{
    if(fFluxes.empty()) return;
    return fFluxes[idx]->WriteEvent(event);
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
