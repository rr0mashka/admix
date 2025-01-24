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


  /*for (auto& mapElement : (*evtMap->GetMap())) {
    auto edep = *(mapElement.second);
    if (edep > eThreshold) nbOfFired++;
    // auto copyNb  = mapElement.first;
    // G4cout << "\n  cryst" << copyNb << ": " << edep/keV << " keV ";
  }
  if (nbOfFired == 2) fGoodEvents++;

  // Dose deposit in patient
  //
  G4double dose = 0.;

  evtMap = static_cast<G4THitsMap<G4double>*>(HCE->GetHC(fCollID_patient));

  for (auto& mapElement : (*evtMap->GetMap())) {
    dose += *(mapElement.second);
    // auto copyNb  = mapElement.first;
    // G4cout << "\n  patient" << copyNb << ": " << G4BestUnit(dose,"Dose");
  }
  fSumDose += dose;
  fStatDose += dose;
*/
  G4Run::RecordEvent(event);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::Merge(const G4Run* run)
{
  const Run* localRun = static_cast<const Run*>(run);
//  fGoodEvents += localRun->fGoodEvents;
//  fSumDose += localRun->fSumDose;
//  fStatDose += localRun->fStatDose;
//  G4Run::Merge(run);
}
