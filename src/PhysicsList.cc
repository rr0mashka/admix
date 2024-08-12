#include "PhysicsList.hh"
/*
#include "G4VModularPhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4NuclideTable.hh"
//#include "HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronInelasticQBBC.hh"
#include "QBBC.hh"
#include "QGSP_BIC_HP.hh"

#include "G4HadronPhysicsINCLXX.hh"
#include "G4IonElasticPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4IonINCLXXPhysics.hh"
#include "G4StoppingPhysics.hh"

#include "G4EmStandardPhysics_option3.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4StepLimiterPhysics.hh"

#include "globals.hh"
*/



//G4VModularPhysicsList* physicsList = new QBBC;
//physicsList->SetVerboseLevel(1); // ?
//physicsList->RegisterPhysics(new G4StepLimiterPhysics()); //!!!!!!!!!!!!!!!
//physicsList->RegisterPhysics(new G4RadioactiveDecayPhysics);
//runManager->SetUserInitialization(physicsList);
// new implimentation of physics physicsList
//:G4VModularPhysicsList()

PhysicsList::PhysicsList()
:QGSP_BIC_HP()

{

  G4int verb = 1;
  SetVerboseLevel(verb);
  // mandatory for G4NuclideTable

  //

  G4StepLimiterPhysics* stepLimitPhys = new G4StepLimiterPhysics();
  stepLimitPhys->SetApplyToAll(true);
  RegisterPhysics(new G4StepLimiterPhysics()); //!!!



  const G4double meanLife = 1*nanosecond, halfLife = meanLife*std::log(2);

  G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(halfLife);




  // EM physics
  ////RegisterPhysics(new G4EmStandardPhysics_option3(verb));

  RegisterPhysics(new G4RadioactiveDecayPhysics());

}



PhysicsList::~PhysicsList()

{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()

{

  // SetCutValue(0*mm, "proton");
  // SetCutValue(10*km, "e-");
  // SetCutValue(10*km, "e+");
  // SetCutValue(10*km, "gamma");

}
