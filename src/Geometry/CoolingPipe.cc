#include "Geometry/CoolingPipe.hh"
#include "G4VSolid.hh"
#include "G4ThreeVector.hh"
#include "G4MultiUnion.hh"
#include "G4Transform3D.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4UserLimits.hh"

CoolingPipe::CoolingPipe (G4String name, G4double x_pos, G4double y_pos,  G4double z_pos,  G4LogicalVolume* mother_volume_log, G4double maxStep)
{

  G4NistManager* nist = G4NistManager::Instance();
  G4Material* PipeShellMaterial= nist->FindOrBuildMaterial("G4_POLYVINYLIDENE_CHLORIDE");
  G4Material* WaterInThePipe= nist->FindOrBuildMaterial("G4_WATER");

  //========AGM6 material =======================

  G4double AMG6_density =  2.640*g/cm3;
  G4int ncomp = 6;

  G4Material* BackPlateMaterial  = new G4Material("AMG6", AMG6_density, ncomp);
  G4Material* pAl = nist->FindOrBuildMaterial("G4_Al");
  G4Material* pMg = nist->FindOrBuildMaterial("G4_Mg");
  G4Material* pMn = nist->FindOrBuildMaterial("G4_Mn");
  G4Material* pTi = nist->FindOrBuildMaterial("G4_Ti");
  G4Material* pNa = nist->FindOrBuildMaterial("G4_Na");
  G4Material* pCu = nist->FindOrBuildMaterial("G4_Na");

  G4double frac_Cu = 0.001;
  G4double frac_Na = 0.0001;
  G4double frac_Ti = 0.0006;
  G4double frac_Mn = 0.007;
  G4double frac_Mg = 0.062;
  G4double frac_Al = 1 - frac_Mg - frac_Mn - frac_Ti - frac_Na  - frac_Cu;

  BackPlateMaterial->AddMaterial(pAl, frac_Al);
  BackPlateMaterial->AddMaterial(pMg, frac_Mg);
  BackPlateMaterial->AddMaterial(pMn, frac_Mn);
  BackPlateMaterial->AddMaterial(pTi, frac_Ti);
  BackPlateMaterial->AddMaterial(pNa, frac_Na);
  BackPlateMaterial->AddMaterial(pCu, frac_Cu);

  //============================================================================

  z_pos = z_pos - 0.5 * PipeLength; // moving origin to the rear face

 G4double Phi_seg_frac = 2.0;

         G4Tubs *pPipeShell_sol = new G4Tubs
         (
            "PipeShell_sol",
             0.5 * Pipe_InnerDiamter,
             0.5 * Pipe_InnerDiamter + WallThickness,
             0.5 * PipeLength,
             0,
             Phi_seg_frac*CLHEP::pi
          );

         G4LogicalVolume *pPipeShell_Log = new G4LogicalVolume
         (
             pPipeShell_sol,  // its solid
             PipeShellMaterial,// its material
             "PipeShell_Log"
           );


        G4VPhysicalVolume  *pPipeShell_Phys = new G4PVPlacement
        (
          nullptr,  // no rotation
          G4ThreeVector(x_pos, y_pos, z_pos),                          // at (0,0,0)
          pPipeShell_Log,                                // its logical volume
          "PipeShell_Phys",                                  // its name
          mother_volume_log,                                   // its mother  volume
          false,                                     // no boolean operation
          0,                                         // copy number
          true                                       // overlaps checking
        );


//======= filling Water ===========================================

G4Tubs *pWaterInPipe_sol = new G4Tubs
(
   "PipeShell_sol",
    0.0,
    0.5 * Pipe_InnerDiamter,
    0.5 * PipeLength,
    0,
    Phi_seg_frac*CLHEP::pi
 );

G4LogicalVolume *pWaterInPipe_Log = new G4LogicalVolume
(
    pWaterInPipe_sol,  // its solid
    WaterInThePipe ,// its material
    "WaterInPipe_Log"
  );


G4VPhysicalVolume  *pWaterInPipe_Phys = new G4PVPlacement
(
 nullptr,  // no rotation
 G4ThreeVector(x_pos, y_pos, z_pos),                          // at (0,0,0)
 pWaterInPipe_Log,                                // its logical volume
 "WaterInPipe_Phys",                                  // its name
 mother_volume_log,                                   // its mother  volume
 false,                                     // no boolean operation
 0,                                         // copy number
 true                                       // overlaps checking
);
//==============================================================================

//========= Fitting pipe made of AMG6 ==========================================


G4Tubs *pFittingPipe_sol = new G4Tubs
(
   "PipeShell_sol",
    0.5 * Pipe_InnerDiamter - FittingPipeWallThickness,
    0.5 * Pipe_InnerDiamter,
    0.5 * FittingPipeLength ,
    0,
    Phi_seg_frac*CLHEP::pi
 );

G4LogicalVolume *pFittingPipe_Log = new G4LogicalVolume
(
    pFittingPipe_sol,  // its solid
    BackPlateMaterial,// its material
    "FittingPipe_Log"
  );

G4double z_pos_fitting = 0.5 * ( PipeLength- FittingPipeLength); //local position inside the mother local (water) volume
G4VPhysicalVolume  *pFittingPipe_Phys = new G4PVPlacement
(
 nullptr,  // no rotation
 G4ThreeVector(0, 0, z_pos_fitting),                          // at (0,0,0)
 pFittingPipe_Log,                                // its logical volume
 "FittingPipe_Phys",                                  // its name
 pWaterInPipe_Log,                                   // its mother  volume
 false,                                     // no boolean operation
 0,                                         // copy number
 true                                       // overlaps checking
);
//==============================================================================

        auto fStepLimit = new G4UserLimits(maxStep);
        pPipeShell_Log -> SetUserLimits(fStepLimit);
        pWaterInPipe_Log -> SetUserLimits(fStepLimit);

}
