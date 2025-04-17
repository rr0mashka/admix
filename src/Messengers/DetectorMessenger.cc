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
/// \file medical/dna/AuNP/src/DetectorMessenger.cc
/// \brief Implementation of the DetectorMessenger class
//
// $Id: DetectorMessenger.cc 78723 2014-01-20 10:32:17Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "Messengers/DetectorMessenger.hh"
#include "DetectorConstruction.hh"


#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UnitsTable.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction * Det)
:G4UImessenger(),fDetector(Det),
 fDetDir(0),
 fLithiumGammaFlagCmd(0),
 fBoronConcentrationCmd(0)
{
  fDetDir = new G4UIdirectory("/BNCT/det/");
  fDetDir->SetGuidance("Detector construction commands");

  new G4UnitDefinition ( "mg/L" , "mg/L", "Concentration", mg/L );

  fBoronConcentrationCmd = new G4UIcmdWithADoubleAndUnit("/BNCT/det/setBoronConcentration",this);
  fBoronConcentrationCmd->SetGuidance("Set concentration of the natural boron in ppm");
  fBoronConcentrationCmd->SetParameterName("BoronConcentration",false);
  fBoronConcentrationCmd->SetRange("BoronConcentration>0.");
  fBoronConcentrationCmd->SetUnitCategory("mg/L");
  fBoronConcentrationCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fBoronConcentrationCmd->SetToBeBroadcasted(false);

  fLithiumGammaFlagCmd = new G4UIcmdWithABool("/BNCT/det/setLithiumGammaFlag",this);
  fLithiumGammaFlagCmd ->SetGuidance("set whether gamma rays in lithium will be produced");
  fLithiumGammaFlagCmd->SetParameterName("LithiumGammaFlag",false);
  fLithiumGammaFlagCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fLithiumGammaFlagCmd->SetToBeBroadcasted(false);



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fBoronConcentrationCmd;
  delete fLithiumGammaFlagCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == fBoronConcentrationCmd ){
     fDetector->SetBoronConcentration(fBoronConcentrationCmd->GetNewDoubleValue(newValue));
  }
  if( command == fLithiumGammaFlagCmd ){
     fDetector->SetLithiumGammaFlag(fLithiumGammaFlagCmd->GetNewBoolValue(newValue));
  }

}
