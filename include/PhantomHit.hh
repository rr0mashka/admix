#ifndef PhantomHit_h
#define PhantomHit_h 1

#include "G4VHit.hh"

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4Threading.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"


class PhantomHit : public G4VHit
{
  public:
    PhantomHit() = default;
    PhantomHit(const PhantomHit&) = default;
    ~PhantomHit() override = default;

    // operators
    PhantomHit& operator=(const PhantomHit&) = default;
    G4bool operator==(const PhantomHit&) const;

    inline void* operator new(size_t);
    inline void operator delete(void*);

    // methods from base class
    void Draw() override;
    void Print() override;

    // Set methods
    void SetEdep(G4double de) { fEdep = de; };
    void SetPos(G4ThreeVector xyz) { fPos = xyz; };
    void SetVolName(G4String name) {fVolName = name;};
    void SetEdepBoron(G4double de) { fEdepBoron = de; };

    // Get methods
    G4double GetEdep() const { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };
    G4String GetVolName() const {return fVolName;};
    G4double GetEdepBoron() { return fEdepBoron; };

  private:
    G4double fEdep = 0.;
    G4double fEdepBoron = 0;
    G4ThreeVector fPos;
    G4String fVolName;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using PhantomHitsCollection = G4THitsCollection<PhantomHit>;

extern G4ThreadLocal G4Allocator<PhantomHit>* PhantomHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* PhantomHit::operator new(size_t)
{
  if (!PhantomHitAllocator) PhantomHitAllocator = new G4Allocator<PhantomHit>;
  return (void*)PhantomHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void PhantomHit::operator delete(void* hit)
{
  PhantomHitAllocator->FreeSingle((PhantomHit*)hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif
