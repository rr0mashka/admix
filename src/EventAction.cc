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
/// \file EventAction.cc
/// \brief Implementation of the B1::EventAction class

#include "EventAction.hh"
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"


#include "G4Event.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction, PrimaryGeneratorAction* generatorAction)
: fRunAction(runAction),
  fGeneratorAction(generatorAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
// initialize passed distance dE/dx

  distdEdx = 0.;

// get primary vertex
  xprime = fGeneratorAction->GetParticleGun()->GetParticlePosition().x();
  yprime = fGeneratorAction->GetParticleGun()->GetParticlePosition().y();
  zprime = fGeneratorAction->GetParticleGun()->GetParticlePosition().z();

  vdEdz = InitializeZVector(fRunAction->MinZ, fRunAction->MaxZ, fRunAction->stepfordEdz);
  vEn = InitializeEnVector(fRunAction->MinZ, fRunAction->MaxZ, fRunAction->stepforfluence);
  const DetectorConstruction* detConstruction = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  fEdepV = Initialize_EinVol_Vector(detConstruction->GetScoringVolumes().size());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void EventAction::EndOfEventAction(const G4Event*)
{
  const DetectorConstruction* detConstruction = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4double dose;
  G4double mass = 0.;

  G4AnalysisManager *man = G4AnalysisManager::Instance();
  // accumulate statistics in run action
  std::vector<G4double> doses = {};
  for (uint i=0;i<fEdepV.size();i++){
  // calculate mass to conver to grays
       if (detConstruction->GetScoringVolumes().at(i)->GetLogicalVolume()->GetName()== "template_ScoringCell_Log"){
         mass = detConstruction->GetScoringVolumes().at(i)->GetLogicalVolume()->GetMass();
       };

      // dose = (fEdepV.at(i)/CLHEP::eV)/(mass*e_SI);
       doses.push_back(((fEdepV.at(i)/CLHEP::eV)*e_SI)/(mass/kg));
       //std::cout<<"MASS  "<<mass<<"    "<<kg<<"   "<<e_SI<<"   "<< "   E: "<<fEdepV.at(i)<<std::endl;
  /*     man->FillNtupleDColumn(1,0,fEdepV.at(i));
       man->FillNtupleIColumn(1,1,i);
       man->FillNtupleIColumn(1,2,G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
       man->FillNtupleDColumn(1,3,dose);
       man->FillNtupleDColumn(1,4, detConstruction->vPos_X[i]);
       man->FillNtupleDColumn(1,5, detConstruction->vPos_Y[i]);
       man->FillNtupleDColumn(1,6, detConstruction->vPos_Z[i]);
       man->AddNtupleRow(1);*/
       //std::cout<<"       2nd tuple FiLLINg    "<<detConstruction->vPos_X[i]<< "   "
       //<<detConstruction->vPos_Y[i]<< "   " <<detConstruction->vPos_Z[i]<< std::endl;
  };
  fRunAction->AddDoseCube(doses);
  fRunAction->AddEdepCube(fEdepV);


}


std::vector<G4double> EventAction::InitializeZVector(G4double Min_Z,G4double Max_Z, G4double step){
    G4double diff = Max_Z - Min_Z;
    std::vector<G4double> temp = {};
    for (int j = 0; j<int(diff/step); j++){
      temp.push_back(0);
    };
    return temp;
}

std::vector<G4double> EventAction::InitializeEnVector(G4double Min_Z,G4double Max_Z, G4double step){
    G4double diff = Max_Z - Min_Z;
    std::vector<G4double> temp = {};
    for (int j = 0; j<int(diff/step); j++){
      temp.push_back(-1.);
    };
    return temp;
}

std::vector<G4double> EventAction::Initialize_EinVol_Vector(G4int N){
  std::vector<G4double> temp = {};
  for (int j = 0; j<N; j++){
    temp.push_back(0.);
  };
  return temp;
}
