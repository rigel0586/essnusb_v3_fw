#ifndef ESBROOT_GENERATOR_GENERIC_IFLUXNEXTGENERATOR_H
#define ESBROOT_GENERATOR_GENERIC_IFLUXNEXTGENERATOR_H 1

#include <memory>
#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include <vector>
#include "generators/generic/IWriteEvent.h"

namespace esbroot {

namespace generators {

namespace generic {


class IFluxNextGenerator : public TObject, public IWriteEvent
{

public:

    virtual bool NextPosMomPdg(TVector3& position, TLorentzVector& momentum, int& pdgCode) = 0;
    virtual void WriteEvent(const genie::EventRecord *event) override {}

private:

    ClassDef(IFluxNextGenerator, 6)
};

} // namespace generic

} // namespace generators

} // namespace esbroot
#endif
