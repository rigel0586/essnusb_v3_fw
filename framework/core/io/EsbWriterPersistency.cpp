#include "EsbWriterPersistency.hpp"
ClassImp(esbroot::core::io::EsbWriterPersistency)

#include <fairlogger/Logger.h>

namespace esbroot {
namespace core {
namespace io {


EsbWriterPersistency::EsbWriterPersistency()
{
}

EsbWriterPersistency::~EsbWriterPersistency()
{
    Clear();
}

EsbWriterPersistency& EsbWriterPersistency::Instance()
{
    static EsbWriterPersistency fpersistency;
    return fpersistency;
}

void EsbWriterPersistency::setOutFile(const std::string& path) 
{ 
    if(fOutTFile != nullptr)
    {
        LOG(error) << "Already have existing path, first call Clear then set new output path";
        return;
    }
    
    fOutTFile = new TFile(path.c_str(), "RECREATE", "Data file writer from simulation" );
    fOutpath = path;
}

void EsbWriterPersistency::Clear()
{
    if(fOutTFile != nullptr)
    {
        delete fOutTFile;
        fOutTFile = nullptr;
    }
}

TClonesArray* EsbWriterPersistency::Register(const char* treeName, const char* branchName, TClass* CollObj)
{
    for(RegisterItem it : fItems)
    {
        if(it.fTreeName == std::string(treeName) 
            && it.fBranchName == std::string(branchName) 
            && it.tNamedObj == std::string(CollObj->GetName()) )
        {
            return it.fColl;
        }

        if(it.fTreeName == std::string(treeName) 
            && it.fBranchName == std::string(branchName) 
            && it.tNamedObj != std::string(CollObj->GetName()) )
        {
            LOG(warning) << " Attempting to create a new clonessarray for existing branch with different type";
            return nullptr;
        }
    }

    RegisterItem item;
    item.fTreeName = std::string(treeName);
    item.fBranchName = std::string(branchName);
    item.tNamedObj = std::string(CollObj->GetName());

    TTree *ttree = new TTree(treeName, treeName);
    TClonesArray* tCA = new TClonesArray(CollObj);
    item.fColl = tCA;
    ttree->Branch(branchName,&tCA);
    fItems.emplace_back(item);

    return tCA;
}

} // namespace io
} // namespace core
} // namespace esbroot
