#pragma once

#ifndef EsbWriterPersistency_hpp
#define EsbWriterPersistency_hpp 1

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

class EsbWriterPersistency : public TObject
{
public:
    EsbWriterPersistency();
    ~EsbWriterPersistency();

    class RegisterItem
    {
        public:
            std::string fTreeName{""};
            std::string fBranchName{""};
            std::string tNamedObj{""};
            TClonesArray* fColl{nullptr};
    };

    static EsbWriterPersistency& Instance();

    void setOutFile(const std::string& path);

    TClonesArray* Register(const char* treeName, const char* branchName, TClass* CollObj);
    void Clear();

private:
    std::vector<RegisterItem> fItems;
    std::string fOutpath{""};

    TFile* fOutTFile{nullptr};

    ClassDef(EsbWriterPersistency, 2);
};

} // namespace io
} // namespace core
} // namespace esbroot
#endif
