//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1/src/RunAction.cc
/// \brief Implementation of the B1::RunAction class

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"
#include "G4RunManager.hh"
#include "G4PrimaryVertex.hh"
#include "G4AccumulableManager.hh"
#include "TFile.h"
#include "TTree.h"

RunAction::RunAction()
{
  auto man = G4AnalysisManager::Instance();

 // CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);
  //CLHEP::HepRandom::setTheSeed(time(NULL));
 // CLHEP::HepRandom::setTheSeed((unsigned)clock());

  G4RunManager::GetRunManager()->SetPrintProgress(0);

  man->SetVerboseLevel(0);
  man->SetNtupleMerging(true);
// Fluences for all particles, for a given particle easy to get taking a projection on a particle type

  man->CreateNtuple("Fluences", "Fluences");
  man->CreateNtupleIColumn("Event");
  man->CreateNtupleDColumn("X");
  man->CreateNtupleDColumn("Y");
  man->CreateNtupleDColumn("Zsurf");
  man->CreateNtupleDColumn("Energy");
  man->CreateNtupleIColumn("particle_id");
  man->CreateNtupleSColumn("particle_name");
  man->CreateNtupleSColumn("material_name_end");
  man->FinishNtuple(0);

// Differential energy for the primary particle (step will be an input parameter)

  man->CreateNtuple("Produced particles", "Produced particles");
  man->CreateNtupleDColumn("En");
  man->CreateNtupleSColumn("Particle_Name");
  man->CreateNtupleIColumn("Particle_Id");
  man->CreateNtupleDColumn("X");
  man->CreateNtupleDColumn("Y");
  man->CreateNtupleDColumn("Z");
  man->CreateNtupleDColumn("Distance_fr_pr_vertex");
  man->FinishNtuple(1);

  /*if (IsMaster()){
    man->CreateNtuple("Doses in phantom", "Doses in phantom");
    man->CreateNtupleDColumn("Edep");
    man->CreateNtupleDColumn("EdepBoron");
    man->CreateNtupleSColumn("VolumeName");
    man->CreateNtupleDColumn("dose");
    man->CreateNtupleDColumn("dose_boron");
    man->CreateNtupleDColumn("X");
    man->CreateNtupleDColumn("Y");
    man->CreateNtupleDColumn("Z");
    man->FinishNtuple(2);

    man->CreateNtuple("Hits in detector", "Hits in detector");
    man->CreateNtupleDColumn("Edep");
    man->CreateNtupleSColumn("VolumeName");
    man->CreateNtupleDColumn("dose");
    man->CreateNtupleDColumn("X");
    man->CreateNtupleDColumn("Y");
    man->CreateNtupleDColumn("Z");
    man->FinishNtuple(3);
  }*/

  fEnergyCube = VectorAccumulable<G4double>();

  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(&fEnergyCube);
  accumulableManager->RegisterAccumulable(&fDoseCube);
  accumulableManager->RegisterAccumulable(fNumberOfNeutrons);
}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    fOutputfile = "output.root";
    man->OpenFile(fOutputfile);
    const DetectorConstruction* detConstruction = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fEnergyCube.SetVectorLength(detConstruction->GetNPhantoms());
    fDoseCube.SetVectorLength(detConstruction->GetNPhantoms());
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Reset();

    // set printing event number per each event
}

G4Run* RunAction::GenerateRun(){
  fRun = new Run();
  return fRun;
}

void RunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Merge();
    if (IsMaster()){
      G4String output2 = "Run_" + fOutputfile;
      fRun->SetFileName(output2);
      fRun->EndOfRun();
      std::vector<G4double> EnCubes = fEnergyCube.GetVector();
      std::vector<G4double> DoseCubes = fDoseCube.GetVector();
      const DetectorConstruction* detConstruction = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    /*  G4String output2 = "Run_" + fOutputfile;
      TFile *file = new TFile(output2,"recreate");
      G4int VolumeId;
      G4double X, Y, Z;
      G4double Edep, dose;
      TTree *treecub = new TTree("Dose_in_volume_N","Dose_in_volume_N");
      treecub->Branch("Edep", &Edep,"Edep/D");
      treecub->Branch("VolumeId",&VolumeId,"VolumeId/I");
      treecub->BranDose_in_volume_Nch("dose",&dose,"dose/D");
      treecub->Branch("X",&X,"X/D");
      treecub->Branch("Y",&Y,"Y/D");
      treecub->Branch("Z",&Z,"Z/D");
      for (uint i=0;i<EnCubes.size();i++){
        Edep = EnCubes.at(i);
        VolumeId = i;
        dose = DoseCubes.at(i);
        X =  detConstruction->vPos_X[i];
        Y  = detConstruction->vPos_Y[i];
        Z  = detConstruction->vPos_Z[i];
        treecub->Fill();
      }
      treecub->Write();
      file->Close();*/
      std::cout << "Number of produced neutrons in this run " << fNumberOfNeutrons.GetValue() << std::endl;
    };

    man->Write();
    man->CloseFile();
}

void RunAction::AddEdepCube(std::vector<G4double> encube){
  if (encube.size()!=fEnergyCube.GetVector().size()){
    std::cout << "cubes vector sizes don't match, something wrong" << std::endl;
    return;
  }
  for (int i=0;i<fEnergyCube.GetVector().size();i++){
    fEnergyCube.AddValue(i,encube.at(i));
  };
}

void RunAction::AddDoseCube(std::vector<G4double> dosecube){
  if (dosecube.size()!=fDoseCube.GetVector().size()){
    std::cout << "cubes vector sizes don't match, something wrong" << std::endl;
    return;
  }
  for (int i=0;i<fDoseCube.GetVector().size();i++){
    fDoseCube.AddValue(i,dosecube.at(i));
  };
}

void RunAction::AddNeutrons(G4int i){
    fNumberOfNeutrons+= i;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
