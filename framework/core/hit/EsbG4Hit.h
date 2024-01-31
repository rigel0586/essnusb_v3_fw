#pragma once

#ifndef EsbG4Hit_hpp
#define EsbG4Hit_hpp 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"


namespace esbroot {
namespace esbcore {

template<typename Data>
class EsbG4Hit : public G4VHit
{
public:
    EsbG4Hit();
    EsbG4Hit(const EsbG4Hit&) = default;
    ~EsbG4Hit() override;

    // operators
    EsbG4Hit& operator=(const EsbG4Hit&) = default;
    G4bool operator==(const EsbG4Hit&) const;
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
    typedef G4THitsCollection<EsbG4Hit<Data>> SuperFgdHitsCollection;

    template:
    typedef G4THitsCollection<EsbG4Hit<Data>> EsbG4HitsCollection;
*/

template<class Data>
extern G4ThreadLocal G4Allocator<EsbG4Hit<Data>> *EsbG4HitAllocator;
/*
    NOTE: Declare in the implementation file 
    G4ThreadLocal G4Allocator<EsbG4Hit<Data>> *EsbG4HitAllocator=0;
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

template<typename Data>
inline void* EsbG4Hit<Data>::operator new(size_t)
{
  if (!EsbG4HitAllocator)
    EsbG4HitAllocator = new G4Allocator<EsbG4Hit<Data>>;

  return (void*) EsbG4HitAllocator->MallocSingle();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

template<typename Data>
inline void EsbG4Hit<Data>::operator delete(void* aHit)
{
  EsbG4HitAllocator<Data>->FreeSingle((EsbG4Hit<Data>*) aHit);
}

} // namespace esbcore
} // namespace esbroot

#endif

