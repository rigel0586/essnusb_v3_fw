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

    class ReadItem
    {
        public:
            std::string fTreeName{""};
            std::string fBranchName{""};
            int fEntries{0};
            TTree * fTree{nullptr};
            TClonesArray* fColl{nullptr};
    };

    static EsbReaderPersistency& Instance();

    void setInFile(const std::string& path);

    void Register(const char* treeName
                                , const char* branchName
                                , EsbReaderPersistency::ReadItem* outItem);
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
