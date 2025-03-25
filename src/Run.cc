#include "Run.hh"

#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4THitsMap.hh"
#include "Geometry/SensitiveVolumes/PhantomHit.hh"
#include "G4AnalysisManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "TFile.h"
#include "TTree.h"


void Run::RecordEvent(const G4Event* event)
{
  G4int evt = event->GetEventID();

  // Hits collections
  //
  G4HCofThisEvent* HCE = event->GetHCofThisEvent();
  if (!HCE) {
    std::cout << "wtf?" << std::endl;
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
  };
  G4Run::Merge(run);
}


void Run::EndOfRun(){
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    std::map<G4String, G4double>::const_iterator it;
    G4VPhysicalVolume* volume;
    G4PhysicalVolumeStore* volumeStore = G4PhysicalVolumeStore::GetInstance();
    G4String output2 = "Run_" + fFileName;
    TFile *file = new TFile(output2,"recreate");
    char VolumeName[25];
    G4double X, Y, Z;
    G4double Edep, EdepBoron, dose, mass, doseboron;
    TTree *treecub = new TTree("Doses in phantom","Doses in phantom");
    treecub->Branch("Edep", &Edep,"Edep/D");
    treecub->Branch("EdepBoron", &EdepBoron,"EdepBoron/D");
    treecub->Branch("VolumeName",&VolumeName, "VolumeName/C");
    treecub->Branch("dose",&dose,"dose/D");
    treecub->Branch("X",&X,"X/D");
    treecub->Branch("Y",&Y,"Y/D");
    treecub->Branch("Z",&Z,"Z/D");
    for (it = fEdepMap.begin(); it !=fEdepMap.end(); ++it) {
      volume = volumeStore->GetVolume(it->first);
      Edep = it->second;
      EdepBoron = fEdepBoronMap[it->first];
      mass = volume->GetLogicalVolume()->GetMass();
      dose = ((Edep/CLHEP::eV)*e_SI)/(mass/kg);
      doseboron = ((EdepBoron/CLHEP::eV)*e_SI)/(mass/kg);
      strcpy(VolumeName,it->first.c_str());
      X = volume->GetTranslation().x(); Y=volume->GetTranslation().y(); Z = volume->GetTranslation().z();
      treecub->Fill();
    };
    treecub->Write();
    file->Close();
};


void Run::SetFileName(G4String str){
  fFileName = str;
}
