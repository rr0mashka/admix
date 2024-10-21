#include "Geometry/MiceWoodDisk.hh"
#include "G4VSolid.hh"
#include "G4ThreeVector.hh"
#include "G4MultiUnion.hh"
#include "G4Transform3D.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4UserLimits.hh"
#include "CustomMaterials.hh"

MiceWoodDisk::MiceWoodDisk (G4String name, G4double z_pos, G4LogicalVolume* mother_volume_log, G4double maxStep)
{

    G4NistManager* nist = G4NistManager::Instance();
    G4double density = 0.75 * g / cm3;
    CustomMaterials* pCustomMaterials = CustomMaterials::Instance();
    G4Material* WoodDiskMaterial  = pCustomMaterials->GetMaterial("OakWood");
    G4Material* MiceDisk_mat= nist->FindOrBuildMaterial("G4_WATER");

  z_pos = z_pos + 0.5*MiceWoodDisk_Width; // moving origin to the front face

    G4double Phi_seg_frac = 2.0;

// ================= MiceDisk ==================== //
         G4Tubs *pMiceDisk_sol = new G4Tubs
         (
            "MiceDisk_sol",
             20, // inner radius * 0,5
             60, // outer radius * 0,5
             20, // width * 0,5
             0,
             Phi_seg_frac*CLHEP::pi
          );


         G4LogicalVolume *pMiceDisk_Log = new G4LogicalVolume
         (
             pMiceDisk_sol,  // its solid
             MiceDisk_mat,// its material
             "MiceDisk_Log"
           );

        G4VPhysicalVolume  *pMiceDisk_Phys = new G4PVPlacement
        (
          nullptr,  // no rotation
          G4ThreeVector(0, 0, z_pos),                          // at (0,0,0)
          pMiceDisk_Log,                                // its logical volume
          "MiceDisk_Phys",                    // its name
          mother_volume_log,                                   // its mother  volume
          false,                                     // no boolean operation
          0,                                         // copy number
          true                                       // overlaps checking
        );

    //============== wood disk ==================//
            G4Tubs *pWoodDisk_sol = new G4Tubs
            (
               "WoodDisk_sol",
                0,
                120,
                10,
                0,
                Phi_seg_frac*CLHEP::pi
             );

            G4LogicalVolume *pWoodDisk_Log = new G4LogicalVolume
            (
               pWoodDisk_sol,  // its solid
               WoodDiskMaterial,// its material
               "WoodDisk_Log"
              );

            G4double z_pos_new = z_pos + 30;
           G4VPhysicalVolume  *pWoodDisk_Phys = new G4PVPlacement
           (
             nullptr,  // no rotation
             G4ThreeVector(0, 0, z_pos_new),                          // at (0,0,0)
             pWoodDisk_Log,                                // its logical volume
             "WoodDisk_Phys",                                  // its name
             mother_volume_log,                                   // its mother  volume
             false,                                     // no boolean operation
             0,                                         // copy number
             true                                       // overlaps checking
           );


        auto fStepLimit = new G4UserLimits(maxStep);
        pMiceDisk_Log ->SetUserLimits(fStepLimit);
        pWoodDisk_Log ->SetUserLimits(fStepLimit);
}
