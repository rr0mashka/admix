#include "Run.hh"

#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4THitsMap.hh"
#include "PhantomHit.hh"


void Run::RecordEvent(const G4Event* event)
{
  G4int evt = event->GetEventID();

  // Hits collections
  //
  G4HCofThisEvent* HCE = event->GetHCofThisEvent();
  if (!HCE) {
    std::cout << "here" << std::endl;
    return;
  }
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID("PhantomHitsCollection");

  PhantomHitsCollection* hitsc = static_cast<PhantomHitsCollection*>(HCE->GetHC(hcID));

  for (int i=0; i<hitsc->entries(); i++) {
    PhantomHit* hit = static_cast<PhantomHit*>((*hitsc)[i]);
    fEdepMap[hit->GetVolName()] += hit->GetEdep();
    fEdepBoronMap[hit->GetVolName()] += hit->GetEdepBoron();
  };

  G4Run::RecordEvent(event);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::Merge(const G4Run* run)
{
  G4String volname;
  G4double edep;
  const Run* localRun = static_cast<const Run*>(run);
  std::map<G4String, G4double>::const_iterator it;
  for (it = localRun->fEdepMap.begin(); it != localRun->fEdepMap.end(); ++it) {
    volname = it->first;
    edep = it->second;
    fEdepMap[volname]+=edep;
  };
  for (it = localRun->fEdepBoronMap.begin(); it != localRun->fEdepBoronMap.end(); ++it) {
    G4String volname = it->first;
    G4int edep = it->second;
    fEdepBoronMap[volname]+=edep;
  }

  G4Run::Merge(run);
}
