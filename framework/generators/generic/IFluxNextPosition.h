#ifndef ESBROOT_GENERATOR_GENERIC_IFLUXNEXTPOSITION_H
#define ESBROOT_GENERATOR_GENERIC_IFLUXNEXTPOSITION_H 1

#include <memory>
#include "TObject.h"
#include "TVector3.h"
#include <vector>
#include "generators/generic/IWriteEvent.h"

namespace esbroot {

namespace generators {

namespace generic {


class IFluxNextPosition : public TObject, public IWriteEvent
{

public:
  
    // Return values are interpreted as meters!
    virtual TVector3 NextVertexPosition() = 0;
    virtual void WriteEvent(const genie::EventRecord *event) override {}

private:

    ClassDef(IFluxNextPosition, 6)
};

} // namespace generic

} // namespace generators

} // namespace esbroot
#endif
