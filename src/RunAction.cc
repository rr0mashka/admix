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

RunAction::RunAction()
{
  auto man = G4AnalysisManager::Instance();

  CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);
  //CLHEP::HepRandom::setTheSeed(time(NULL));
  CLHEP::HepRandom::setTheSeed((unsigned)clock());

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

  man -> CreateNtuple("dEdz","dEdz");
  man->CreateNtupleDColumn("Edep_MeV");
  man->CreateNtupleDColumn("Step");
  man->CreateNtupleDColumn("Z");
  man->CreateNtupleDColumn("En");
  man->CreateNtupleIColumn("Event");
  man->FinishNtuple(1);

  man->CreateNtuple("Dose_in_volume_N", "Dose_in_volume_N");
  man->CreateNtupleDColumn("Edep_MeV");
  man->CreateNtupleIColumn("VolumeId");
  man->CreateNtupleIColumn("Event");
  man->CreateNtupleDColumn("dose");
  man->CreateNtupleDColumn("X");
  man->CreateNtupleDColumn("Y");
  man->CreateNtupleDColumn("Z");
  man->FinishNtuple(2);

  man->CreateNtuple("Produced particles", "Produced particles");
  man->CreateNtupleDColumn("En");
  man->CreateNtupleSColumn("Particle_Name");
  man->CreateNtupleIColumn("Particle_Id");
  man->CreateNtupleDColumn("X");
  man->CreateNtupleDColumn("Y");
  man->CreateNtupleDColumn("Z");
  man->CreateNtupleDColumn("Distance_fr_pr_vertex");
  man->FinishNtuple(3);

}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->OpenFile("output.root");


    // set printing event number per each event
}


void RunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
