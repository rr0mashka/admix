#include "Geometry/SensitiveVolumes/PhantomVolume.hh"

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4ios.hh"

PhantomVolume::PhantomVolume(G4String name):
G4VSensitiveDetector(name){

};
PhantomVolume::~PhantomVolume(){

}

void PhantomVolume::Initialize(G4HCofThisEvent* hitCollection){
  // Create hits collection
  fHitsCollection = new PhantomHitsCollection(SensitiveDetectorName, "PhantomHitsCollection");
  // Add this collection in hce

  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID("PhantomHitsCollection");
  hitCollection->AddHitsCollection(hcID, fHitsCollection);
}

G4bool PhantomVolume::ProcessHits(G4Step* step, G4TouchableHistory* history){
 return false;
}

void PhantomVolume::EndOfEvent(G4HCofThisEvent* hitCollection){

}
