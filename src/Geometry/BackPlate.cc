#include "Geometry/BackPlate.hh"
#include "G4VSolid.hh"
#include "G4ThreeVector.hh"
#include "G4MultiUnion.hh"
#include "G4Transform3D.hh"

BackPlate::BackPlate (G4String name, G4double z_pos, G4LogicalVolume* mother_volume_log, G4double maxStep )
{

  G4NistManager* nist = G4NistManager::Instance();
  G4Material* GrooveMaterial= nist->FindOrBuildMaterial("G4_WATER");
  G4Material* BackPlateMaterial = nist->FindOrBuildMaterial("G4_Al");

 G4double Phi_seg_frac = 2.0;

         G4Tubs *pBackPlateDisk_sol = new G4Tubs
         (
            "BackPlateDisk_sol",
             0,
             BackPlate_Radius,
             BackPlate_Width,
             0,
             Phi_seg_frac*CLHEP::pi
          );

         G4LogicalVolume *pBackPlateDisk_Log = new G4LogicalVolume
         (
             pBackPlateDisk_sol,  // its solid
             BackPlateMaterial,// its material
             "BackPlateDisk_Log"
           );


        G4VPhysicalVolume  *pBackPlateDisk_Phys = new G4PVPlacement
        (
          nullptr,  // no rotation
          G4ThreeVector(0, 0, z_pos),                          // at (0,0,0)
          pBackPlateDisk_Log,                                // its logical volume
          "BackPlateDisk_Phys",                                  // its name
          mother_volume_log,                                   // its mother  volume
          false,                                     // no boolean operation
          0,                                         // copy number
          true                                       // overlaps checking
        );



//======= outer of part ( thicker ring) of  the backplate disk =================
G4double OD_offset =  ( BackPlateOD_Width - BackPlate_Width ); // this alignns backplate part at the rear plane


        G4Tubs *pBackPlateOD_sol = new G4Tubs
        (
           "BackPlateOuterDisk_sol",
            BackPlateOD_InnerRadius,
            BackPlateOD_OuterRadius,
            BackPlateOD_Width,
            0,
            Phi_seg_frac*CLHEP::pi
         );

        G4LogicalVolume *pBackPlateOD_Log = new G4LogicalVolume
        (
            pBackPlateOD_sol,  // its solid
            BackPlateMaterial,// its material
            "BackPlateOuterDisk_Log"
          );


       G4VPhysicalVolume  *pBackPlateOD_Phys = new G4PVPlacement
       (
         nullptr,  // no rotation
         G4ThreeVector(0, 0, z_pos - OD_offset),                          // at (0,0,0)
         pBackPlateOD_Log,                                // its logical volume
         "BackPlateOuterDisk_Phys",                                  // its name
         mother_volume_log,                                   // its mother  volume
         false,                                     // no boolean operation
         0,                                         // copy number
         true                                       // overlaps checking
       );
 //=============================================================================


 //======= ring groove filled with whater ======================================
 G4double Groove_offset =  ( BackPlate_Width -  Groove_Width ); // this alignns backplate part at the rear plane


         G4Tubs *pBackPlateGroove_sol = new G4Tubs
          (
            "BackPlateGroove_sol",
             Groove_InnerRadius,
             Groove_OuterRadius,
             Groove_Width,
             0,
             Phi_seg_frac*CLHEP::pi
          );

         G4LogicalVolume *pBackPlateGroove_Log = new G4LogicalVolume
          (
             pBackPlateGroove_sol,  // its solid
             GrooveMaterial,// its material
             "BackPlateGroove_Log"
          );


        G4VPhysicalVolume  *pBackPlateGroove_Phys = new G4PVPlacement
         (
          nullptr,  // no rotation
          G4ThreeVector(0, 0, -1.0*Groove_offset),                          // at (0,0,0)
          pBackPlateGroove_Log,                                // its logical volume
          "BackPlateGroove_Phys",                                  // its name
          pBackPlateDisk_Log,                                   // its mother  volume
          false,                                     // no boolean operation
          0,                                         // copy number
          true                                       // overlaps checking
         );
  //===========================================================================

//==============================================================================
//======== central pit filled with water =======================================

 // ====== Base of the pit =================================
 G4Tubs *pPitDisk1_sol = new G4Tubs
  (
    "PitDisk1",
     0,
     18.00,
     4.0,
     0,
     Phi_seg_frac*CLHEP::pi
   );

  G4Torus *pPitTorus1_sol= new G4Torus
   (
    "PitTorus1",
     0*mm,
     8.000*mm,
     18.0*mm,
     0,
     Phi_seg_frac*CLHEP::pi
   );

 G4VSolid* pPitBase_sol = new G4SubtractionSolid("PitBase", pPitDisk1_sol, pPitTorus1_sol, nullptr, G4ThreeVector(0, 0, 4));


 G4LogicalVolume *pPitBase_Log = new G4LogicalVolume
 (
   pPitBase_sol,    // its solid
   GrooveMaterial,  // its material
   "PitBase_Log"
 );


 G4VPhysicalVolume  *pPitBase_Phys = new G4PVPlacement
  (
   nullptr,  // no rotation
   //G4ThreeVector(60, 40*mm, -20*mm),
   //G4ThreeVector(0, 0, 0),                      // at (0,0,0)
   G4ThreeVector(0, 0, 4.0-BackPlate_Width),                          // at (0,0,0)
   pPitBase_Log,                                // its logical volume
   "PitBase_Phys",                                  // its name
   pBackPlateDisk_Log,                                   // its mother  volume
   false,                                     // no boolean operation
   0,                                         // copy number
   false                                       // overlaps checking
  );

 //=============================================================================


//=============  middle disk ===================================================
G4Tubs *pPitMiddleDisk_sol = new G4Tubs
 (
   "PitMiddleDisk",
    0,
    10,
    0.75,
    0,
    Phi_seg_frac*CLHEP::pi
 );

G4LogicalVolume *pPitMiddleDisk_Log = new G4LogicalVolume
 (
    pPitMiddleDisk_sol,  // its solid
    GrooveMaterial,// its material
    "PitMiddleDisk_Log"
 );


G4VPhysicalVolume  *pPitMiddleDisk_Phys = new G4PVPlacement
(
 nullptr,  // no rotation
 G4ThreeVector(0, 0, 8.0 + 0.75-BackPlate_Width),                          // at (0,0,0)
 pPitMiddleDisk_Log,                                // its logical volume
 "PitMiddleDisk_Phys",                                  // its name
 pBackPlateDisk_Log,                                   // its mother  volume
 false,                                     // no boolean operation
 0,                                         // copy number
 true                                       // overlaps checking
);
//==============================================================================



//  ====== pit cap =============================================================
         G4Tubs *pPitCapDisckTop_sol = new G4Tubs
          (
            "PitDiskTorusInner_sol",
             0,
             10.000,
             6.0,
             0,
             Phi_seg_frac*CLHEP::pi
           );


        G4Torus *pPitCapTorus_sol = new G4Torus
         (
          "PitCapTorus_sol",
           0,
           4.9,
           5.000,
           0,
           Phi_seg_frac*CLHEP::pi
         );



  G4VSolid* pPitCapTop_sol = new G4SubtractionSolid("PitCapTop",  pPitCapTorus_sol, pPitCapDisckTop_sol, nullptr, G4ThreeVector(0, 0, -6));

/*
  G4LogicalVolume *pPitCapTop_Log = new G4LogicalVolume
   (
     pPitCapTop_sol,    // its solid
     GrooveMaterial,  // its material
     "PitCapTop"
   );

  G4VPhysicalVolume  *pPitCapTop_Phys = new G4PVPlacement
   (
     nullptr,  // no rotation
     G4ThreeVector(0, 40*mm, -20*mm),                          // at (0,0,0)
     pPitCapTop_Log ,                                // its logical volume
     "pPitCapTop_Phys",                                  // its name
     mother_volume_log,                                   // its mother  volume
     false,                                     // no boolean operation
     0,                                         // copy number
     true                                       // overlaps checking
   );
*/




//===============================================================

   G4Tubs *pPitCapDisckBottom_sol = new G4Tubs
    (
      "PitCapDisckBottom_sol",
       0,
       5.0,
       2.5,
       0,
       Phi_seg_frac*CLHEP::pi
     );



G4VSolid* pPitCap_sol = new G4UnionSolid("PitCapTop", pPitCapTop_sol, pPitCapDisckBottom_sol,  nullptr, G4ThreeVector(0, 0, 2.5));


  G4LogicalVolume *pPitCap_Log = new G4LogicalVolume
   (
     pPitCap_sol,    // its solid
     GrooveMaterial,  // its material
     "PitCap_Log"
   );


  G4VPhysicalVolume  *pPitCap_Phys = new G4PVPlacement
   (
     nullptr,  // no rotation
     G4ThreeVector(0, 0, 8.0 + 1.5 - BackPlate_Width),                          // at (0,0,0)
     pPitCap_Log ,                                // its logical volume
     "pPitCap_Phys",                                  // its name
      pBackPlateDisk_Log,                                   // its mother  volume
     false,                                     // no boolean operation
     0,                                         // copy number
     true                                       // overlaps checking
   );



   auto fStepLimit = new G4UserLimits(maxStep);
   pBackPlateDisk_Log ->SetUserLimits(fStepLimit);
   pBackPlateOD_Log ->SetUserLimits(fStepLimit);
   pBackPlateGroove_Log ->SetUserLimits(fStepLimit);
   pPitBase_Log ->SetUserLimits(fStepLimit);
   pPitMiddleDisk_Log ->SetUserLimits(fStepLimit);
   pPitCap_Log ->SetUserLimits(fStepLimit);


}
