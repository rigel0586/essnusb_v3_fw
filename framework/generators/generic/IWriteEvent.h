#ifndef ESBROOT_GENERATOR_GENERIC_IWRITE_EVENT_H
#define ESBROOT_GENERATOR_GENERIC_IWRITE_EVENT_H 1

#include <memory>
#include <vector>

#include "TObject.h"
#include "Framework/EventGen/EventRecord.h"

namespace esbroot {

namespace generators {

namespace generic {


class IWriteEvent : public TObject {

public:
  
    virtual void WriteEvent(const genie::EventRecord *event) {}
private:

    ClassDef(IWriteEvent, 6)
};

} // namespace generic

} // namespace generators

} // namespace esbroot
#endif
