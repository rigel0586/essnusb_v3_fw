#include "EsbReaderPersistency.hpp"
ClassImp(esbroot::core::io::EsbReaderPersistency)

#include <fairlogger/Logger.h>

namespace esbroot {
namespace core {
namespace io {


EsbReaderPersistency::EsbReaderPersistency()
{
}

EsbReaderPersistency::~EsbReaderPersistency()
{
    Clear();
}

EsbReaderPersistency& EsbReaderPersistency::Instance()
{
    static EsbReaderPersistency fpersistency;
    return fpersistency;
}

void EsbReaderPersistency::setInFile(const std::string& path) 
{ 
    if(fInTFile != nullptr)
    {
        LOG(error) << "Already have existing path, first call Clear then set new output path";
        return;
    }
    
    fInTFile = new TFile(path.c_str());
    fInpath = path;
}

void EsbReaderPersistency::Clear()
{
    if(fInTFile != nullptr)
    {
        delete fInTFile;
        fInTFile = nullptr;
    }
}

TClonesArray* EsbReaderPersistency::Register(const char* treeName, const char* branchName)
{
    if(fInTFile != nullptr)
    {
        TTree *tree = (TTree*)fInTFile->Get(treeName);
        if(tree == nullptr) return nullptr;

        TClonesArray* arr = nullptr;
        tree->SetBranchAddress(branchName,&arr);
        return arr;
    }

    return nullptr;
}

} // namespace io
} // namespace core
} // namespace esbroot
