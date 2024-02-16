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

EsbReaderPersistency::ReadItem EsbReaderPersistency::Register(const char* treeName, const char* branchName)
{
    ReadItem item;
    if(fInTFile != nullptr)
    {
        TTree *tree = (TTree*)fInTFile->Get(treeName);
        if(tree == nullptr) return item;

        item.fTreeName = treeName;
        item.fBranchName = branchName;
        item.fTree = tree;
        item.fEntries = tree->GetEntriesFast();

        tree->SetBranchAddress(branchName,&item.fColl);
    }

    return item;
}

} // namespace io
} // namespace core
} // namespace esbroot
