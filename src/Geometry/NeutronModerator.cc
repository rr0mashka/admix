#include "Geometry/NeutronModerator.hh"
#include "G4VSolid.hh"
#include "G4ThreeVector.hh"
#include "G4MultiUnion.hh"
#include "G4Transform3D.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4UserLimits.hh"
#include "CustomMaterials.hh"


NeutronModerator::NeutronModerator (G4String name, G4double z_pos, G4LogicalVolume* mother_volume_log, G4double maxStep)
{

    CustomMaterials* pCustomMaterials = CustomMaterials::Instance();
    G4Material* pPolyBiz_mat  = pCustomMaterials->GetMaterial("PolyBiz");


  z_pos = z_pos + 0.5*GlassDisk_Width; // moving origin to the front face

 G4double Phi_seg_frac = 2.0;

         G4Tubs *pGlassDisk_sol = new G4Tubs
         (
            "ModeratorGlassDisk_sol",
             0,
             0.5 * GlassDisk_Diamter,
             0.5 * GlassDisk_Width,
             0,
             Phi_seg_frac*CLHEP::pi
          );

         G4LogicalVolume *pGlassDisk_Log = new G4LogicalVolume
         (
             pGlassDisk_sol,  // its solid
             pPolyBiz_mat,// its material
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


        auto fStepLimit = new G4UserLimits(maxStep);
        pGlassDisk_Log ->SetUserLimits(fStepLimit);

}
