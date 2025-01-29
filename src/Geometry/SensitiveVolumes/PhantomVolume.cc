#include "Geometry/SensitiveVolumes/PhantomVolume.hh"

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4ios.hh"

PhantomVolume::PhantomVolume(const G4String name):
G4VSensitiveDetector(name){
  fHitsCollectionName = "PhantomHitsCollection";
  collectionName.insert("PhantomHitsCollection");
};
PhantomVolume::~PhantomVolume(){

}

void PhantomVolume::Initialize(G4HCofThisEvent* hitCollection){
  // Create hits collection
  fHitsCollection = new PhantomHitsCollection(SensitiveDetectorName,fHitsCollectionName);
  // Add this collection in hce

  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollectionName);
  std::cout << "id    " << hcID << std::endl;
  hitCollection->AddHitsCollection(hcID, fHitsCollection);
}

G4bool PhantomVolume::ProcessHits(G4Step* step, G4TouchableHistory* history){
  G4double edep = step->GetTotalEnergyDeposit();

  if (edep == 0.) return false;

  auto newHit = new PhantomHit();

  G4VPhysicalVolume *phv = step->GetPostStepPoint()->GetPhysicalVolume();
  G4Track* mytrack =  step->GetTrack();
  if (!mytrack) return false;
  if (!phv) return false;
  G4String partname = mytrack->GetDefinition()->GetParticleName();

  if ((partname == "alpha") || (partname == "Li7")) {
    newHit->SetEdepBoron(edep);
  };
  newHit->SetVolName(phv->GetName());
  newHit->SetEdep(edep);
  newHit->SetPos(phv->GetObjectTranslation());

  fHitsCollection->insert(newHit);
  return true;
}

void PhantomVolume::EndOfEvent(G4HCofThisEvent* hitCollection){

}
