#pragma once

#ifndef ESSnusbPhysicsList_hpp
#define ESSnusbPhysicsList_hpp 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"


namespace esbroot {
namespace esbcore {

template<typename Data>
class EsbCoreHit : public G4VHit
{
public:
    EsbCoreHit();
    EsbCoreHit(const EsbCoreHit&) = default;
    ~EsbCoreHit() override;

    // operators
    EsbCoreHit& operator=(const EsbCoreHit&) = default;
    G4bool operator==(const EsbCoreHit&) const;
    inline void* operator new(size_t);
    inline void operator delete(void*);

    // methods from base class
    void Draw() override;
    void Print() override;

    // Set methods
    void SetTrackID (G4int track) { fTrackID = track; };
    void SetChamberNb(G4int chamb) { fChamberNb = chamb; };
    void SetEdep (G4double de) { fEdep = de; };
    void SetPos (G4ThreeVector xyz){ fPos = xyz; };
    // Generic set method
    void SetData(Data d){ m_data = d;}

    // Get methods
    G4int GetTrackID() const { return fTrackID; };
    G4int GetChamberNb() const { return fChamberNb; };
    G4double GetEdep() const { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };
    // Generic get method
    Data getData(Data d){ return m_data;}
private:
    G4int fTrackID = -1;
    G4int fChamberNb = -1;
    G4double fEdep = 0.;
    G4ThreeVector fPos;

    Data m_data;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

/*
    Every subclass must declare its own collection e.g.
    if SuperFgd is a subclass 
    typedef G4THitsCollection<SuperFgd<Data>> SuperFgdHitsCollection;

    template:
    typedef G4THitsCollection<EsbCoreHit<Data>> EsbCoreHitsCollection;
*/

template<class Data>
extern G4ThreadLocal G4Allocator<EsbCoreHit<Data>> *EsbCoreHitAllocator;
/*
    NOTE: Declare in the implementation file 
    G4ThreadLocal G4Allocator<EsbCoreHit<Data>> *EsbCoreHitAllocator=0;
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

template<typename Data>
inline void* EsbCoreHit<Data>::operator new(size_t)
{
  if (!EsbCoreHitAllocator<Data>)
    EsbCoreHitAllocator<Data> = new G4Allocator<EsbCoreHit>;
  return (void*) EsbCoreHitAllocator<Data>->MallocSingle();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

template<typename Data>
inline void EsbCoreHit<Data>::operator delete(void* aHit)
{
  EsbCoreHitAllocator<Data>->FreeSingle((EsbCoreHit*) aHit);
}

} // namespace esbcore
} // namespace esbroot

#endif

