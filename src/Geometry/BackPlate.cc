#include "Geometry/BackPlate.hh"
#include "G4VSolid.hh"
#include "G4ThreeVector.hh"
#include "G4MultiUnion.hh"
#include "G4Transform3D.hh"

BackPlate::BackPlate (G4String name, G4double z_pos, G4LogicalVolume* mother_volume_log )
{

  G4NistManager* nist = G4NistManager::Instance();
  G4Material* GrooveMaterial= nist->FindOrBuildMaterial("G4_WATER");
  G4Material* BackPlateMaterial = nist->FindOrBuildMaterial("G4_Cu");


         G4Tubs *pBackPlateDisk_sol = new G4Tubs
         (
            "BackPlateDisk_sol",
             0,
             BackPlate_Radius,
             BackPlate_Width,
             0,
             2*CLHEP::pi
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
            2*CLHEP::pi
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
             2*CLHEP::pi
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
  //============================================================================


 //======= central pit filled with water ====================================



 //G4double Pit_offse =  ( BackPlate_Width -  Groove_Width ); // this alignns backplate part at the rear plane


         G4Tubs *pPit_sol = new G4Tubs
         (
            "Pit_sol",
             0,
             10.0,
             5.0,
             0,
             2*CLHEP::pi
          );


         G4LogicalVolume *pPit_Log = new G4LogicalVolume
         (
             pPit_sol,  // its solid
             GrooveMaterial,// its material
             "Pit_Log"
           );


        G4VPhysicalVolume  *pPit_Phys = new G4PVPlacement
        (
          nullptr,  // no rotation
          G4ThreeVector(0, 0, -50*mm),                          // at (0,0,0)
          pPit_Log,                                // its logical volume
          "Pit_Phys",                                  // its name
          mother_volume_log,                                   // its mother  volume
          false,                                     // no boolean operation
          0,                                         // copy number
          true                                       // overlaps checking
        );



        G4Torus *pTorus_sol_1 = new G4Torus
        (
          "Torus_sol_1",
           0*mm,
           5*mm,
           5.01*mm,
           0,
           2*CLHEP::pi
        );

        G4LogicalVolume *pTorus_Log_1 = new G4LogicalVolume
        (
           pTorus_sol_1,    // its solid
           GrooveMaterial,  // its material
           "Torus_Log_1"
         );

        G4VPhysicalVolume  *pTorus_Phys_1 = new G4PVPlacement
        (
          nullptr,  // no rotation
          G4ThreeVector(0, 0, -0*mm),                          // at (0,0,0)
          pTorus_Log_1,                                // its logical volume
          "Torus_Phys_1",                                  // its name
          mother_volume_log,                                   // its mother  volume
          false,                                     // no boolean operation
          0,                                         // copy number
          false                                       // overlaps checking
        );


  G4VSolid* union1 = new G4UnionSolid("pTorus_sol_1 + pPit_sol", pTorus_sol_1, pPit_sol);


 //=============================================================================


}
