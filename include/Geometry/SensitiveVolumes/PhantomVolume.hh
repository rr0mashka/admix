#ifndef B1Phantomvolume_h
#define B1PhantomVolume_h 1

#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "globals.hh"
#include "G4VSensitiveDetector.hh"
#include "PhantomHit.hh"

class PhantomVolume : public G4VSensitiveDetector
 {
  public:
    PhantomVolume(G4String);
    ~PhantomVolume() override = default;



    // methods from base class
    void Initialize(G4HCofThisEvent* hitCollection) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void EndOfEvent(G4HCofThisEvent* hitCollection) override;


  private:
      PhantomHitsCollection* fHitsCollection = nullptr;
      G4String fHitsCollectionName;
};


#endif
