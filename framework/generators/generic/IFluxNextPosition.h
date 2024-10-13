#ifndef ESBROOT_GENERATOR_GENERIC_IFLUXNEXTPOSITION_H
#define ESBROOT_GENERATOR_GENERIC_IFLUXNEXTPOSITION_H 1

#include <memory>
#include "TObject.h"
#include "TVector3.h"

namespace esbroot {

namespace generators {

namespace generic {


class IFluxNextPosition : public TObject {

public:
  
    virtual TVector3 NextVertexPosition() = 0;
private:

    ClassDef(IFluxNextPosition, 6)
};

} // namespace generic

} // namespace generators

} // namespace esbroot
#endif
