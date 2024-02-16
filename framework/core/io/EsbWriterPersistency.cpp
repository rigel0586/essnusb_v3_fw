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


void EsbWriterPersistency::writeData()
{
    if(fOutTFile == nullptr) return;
    
    for(RegisterItem it : fItems)
    {
        if(fOutTFile != nullptr && it.fTree != nullptr)
        {
            fOutTFile->WriteTObject(it.fTree);
        }
    }
    fOutTFile->Close();
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

WriterInfo EsbWriterPersistency::Register(const char* treeName, const char* branchName, TClass* CollObj)
{
    
    for(RegisterItem it : fItems)
    {
        if(it.fTreeName == std::string(treeName) 
            && it.fBranchName == std::string(branchName) 
            && it.tNamedObj == std::string(CollObj->GetName()) )
        {
            WriterInfo info;
            info.ftree = it.fTree;
            info.fdata = it.fColl;
            return info;
        }

        if(it.fTreeName == std::string(treeName) 
            && it.fBranchName == std::string(branchName) 
            && it.tNamedObj != std::string(CollObj->GetName()) )
        {
            LOG(warning) << " Attempting to create a new clonessarray for existing branch with different type";
            return WriterInfo{};
        }
    }

    RegisterItem item;
    item.fTreeName = std::string(treeName);
    item.fBranchName = std::string(branchName);
    item.tNamedObj = std::string(CollObj->GetName());

    TTree *ttree = new TTree(treeName, treeName);
    TClonesArray* tCA = new TClonesArray(CollObj);
    item.fColl = tCA;
    item.fTree = ttree;
    ttree->Branch(branchName,&tCA);
    fItems.emplace_back(item);

    WriterInfo info;
    info.ftree = item.fTree;
    info.fdata = item.fColl;
    return info;
}

} // namespace io
} // namespace core
} // namespace esbroot
