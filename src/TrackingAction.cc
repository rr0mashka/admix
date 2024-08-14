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

#include "TrackingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "RunAction.hh"

#include "G4Track.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction(RunAction* runAc ,EventAction* eventAction)
:fRunAction(runAc),
 fEventAction(eventAction)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::~TrackingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
  G4double En,xpos,ypos,zpos,xpr, ypr, zpr, dist;
  G4String particle_name;
  G4int PDG_Id;

// particle characteristics

  G4AnalysisManager *man = G4AnalysisManager::Instance();

  particle_name=track->GetDefinition()->GetParticleName() ;
  PDG_Id = track->GetDefinition()->GetPDGEncoding() ;
  En = track->GetKineticEnergy() / CLHEP::MeV ;
  xpos = track->GetPosition().x();
  ypos = track->GetPosition().y();
  zpos = track->GetPosition().z();

// primary vertex characterics

  xpr = fEventAction->xprime;
  ypr = fEventAction->yprime;
  zpr = fEventAction->zprime;

// calculate distance from primary vertex
  dist = sqrt( (xpos-xpr)*(xpos-xpr) + (ypos-ypr)*(ypos-ypr) + (zpos-zpr)*(zpos-zpr));

  if(PDG_Id == 2112 ||PDG_Id == 2212){//keeping particles with energy> 100keV
//if(particle_id==22){//keeping electrons only
    man->FillNtupleDColumn(1,0,En);
    //man->FillNtupleSColumn(3,1,particle_name);
    man->FillNtupleIColumn(1,2,PDG_Id);
    man->FillNtupleDColumn(1,3,xpos);
    man->FillNtupleDColumn(1,4,ypos);
    man->FillNtupleDColumn(1,5,zpos);
    //man->FillNtupleDColumn(3,6,dist);
    man->AddNtupleRow(1);
  }
}
