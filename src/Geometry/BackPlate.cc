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
             BackPlateRadius,
             BackPlateWidth,
             0,
             2*CLHEP::pi
          );

         G4LogicalVolume *pBackPlateDisk_Log = new G4LogicalVolume
         (
             pBackPlateDisk_sol,  // its solid
             BackPlateMaterial,// its material
             "BackPlateDisk_Log"
           );

/*
         G4VPhysicalVolume *pBackPlateDisk_Phys = new G4PVPlacement(
              nullptr,  // no rotation                //no rotation
              G4ThreeVector(0, 0, z_pos),
              pBackPlateDisk_Log,                //its logical volume
              "BackPlateDisk_Phys",                     //its name
              mother_volume_log,                     //its mother  volume
              false,                        //no boolean operation
              0,                            //copy number
              true
            );                          //overlaps checking
*/

        G4VPhysicalVolume  *pBackPlateDisk_Phys = new G4PVPlacement
        (
          nullptr,  // no rotation
          G4ThreeVector(0, 0, z_pos),                          // at (0,0,0)
          pBackPlateDisk_Log,                                // its logical volume
          "BackPlateDisk_Phys",                                  // its name
          mother_volume_log,                                   // its mother  volume
          // "fdsfds"
          false,                                     // no boolean operation
          0,                                         // copy number
          true                                       // overlaps checking
        );


}
