#pragma once

#ifndef EsbReaderPersistency_hpp
#define EsbReaderPersistency_hpp 1

#include <string>
#include <vector>
#include <map>

#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"


namespace esbroot {
namespace core {
namespace io {

class EsbReaderPersistency : public TObject
{
public:
    EsbReaderPersistency();
    ~EsbReaderPersistency();

    static EsbReaderPersistency& Instance();

    void setInFile(const std::string& path);

    TClonesArray* Register(const char* treeName, const char* branchName);
    void Clear();

private:
    std::string fInpath{""};

    TFile* fInTFile{nullptr};

    ClassDef(EsbReaderPersistency, 2);
};

} // namespace io
} // namespace core
} // namespace esbroot
#endif
