#include "Geometry/NeutronModerator.hh"
#include "G4VSolid.hh"
#include "G4ThreeVector.hh"
#include "G4MultiUnion.hh"
#include "G4Transform3D.hh"

NeutronModerator::NeutronModerator (G4String name, G4double z_pos, G4LogicalVolume* mother_volume_log )
{

  G4NistManager* nist = G4NistManager::Instance();
  //G4Material* ModeratorMaterial= nist->FindOrBuildMaterial("“G4_PLEXIGLASS");

  G4Material* ModeratorMaterial= nist->FindOrBuildMaterial("G4_WATER");

  z_pos = z_pos + GlassDisk_Width; // moving origin to the front face

 G4double Phi_seg_frac = 2.0;

         G4Tubs *pGlassDisk_sol = new G4Tubs
         (
            "ModeratorGlassDisk_sol",
             0,
             GlassDisk_Radius,
             GlassDisk_Width,
             0,
             Phi_seg_frac*CLHEP::pi
          );

         G4LogicalVolume *pGlassDisk_Log = new G4LogicalVolume
         (
             pGlassDisk_sol,  // its solid
             ModeratorMaterial,// its material
             "ModeratorGlassDisk_Log"
           );


        G4VPhysicalVolume  *pGlassDisk_Phys = new G4PVPlacement
        (
          nullptr,  // no rotation
          G4ThreeVector(0, 0, z_pos),                          // at (0,0,0)
          pGlassDisk_Log,                                // its logical volume
          "ModeratorGlassDisk_Phys",                                  // its name
          mother_volume_log,                                   // its mother  volume
          false,                                     // no boolean operation
          0,                                         // copy number
          true                                       // overlaps checking
        );




}
