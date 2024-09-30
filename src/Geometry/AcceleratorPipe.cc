#include "Geometry/AcceleratorPipe.hh" // Создать AcceleratorPipe.hh
#include "G4VSolid.hh"
#include "G4ThreeVector.hh"
#include "G4MultiUnion.hh"
#include "G4Transform3D.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4UserLimits.hh"
#include "CustomMaterials.hh"

AcceleratorPipe::AcceleratorPipe (G4String name, G4double z_pos, G4LogicalVolume* mother_volume_log, G4double maxStep)
{

  CustomMaterials* pCustomMaterials = new CustomMaterials();
  G4Material* pAMG6_mat  = pCustomMaterials->GetMaterial("AMG6");

  z_pos = z_pos - 0.5*AcceleratorPipe_Width; // moving origin to the front face

 G4double Phi_seg_frac = 2.0;

         G4Tubs *pAcceleratorPipe = new G4Tubs
         (
            "AcceleratorPipe",
             61,
             72,
             8,
             0,
             Phi_seg_frac*CLHEP::pi
          );

        G4Tubs *pAcceleratorPipe1 = new G4Tubs
        (
         "AcceleratorPipe1",
         61,
         64,
         52.5,
         0,
         Phi_seg_frac*CLHEP::pi
       );

    G4Tubs *pAcceleratorPipe2 = new G4Tubs
    (
     "AcceleratorPipe2",
     50,
     82.5,
     17.5,
     0,
     Phi_seg_frac*CLHEP::pi
   );

    G4Tubs *pAcceleratorPipe3 = new G4Tubs
    (
     "AcceleratorPipe3",
     65.5,
     82.5,
     22,
     0,
     Phi_seg_frac*CLHEP::pi
   );

    G4Tubs *pAcceleratorPipe4 = new G4Tubs
    (
     "AcceleratorPipe4",
     50,
     82.5,
     17.5,
     0,
     Phi_seg_frac*CLHEP::pi
   );

    G4Tubs *pAcceleratorPipe5 = new G4Tubs
    (
     "AcceleratorPipe5",
     50,
     52.5,
     5,
     0,
     Phi_seg_frac*CLHEP::pi
   );

    G4Tubs *pAcceleratorPipe6 = new G4Tubs
    (
     "AcceleratorPipe5",
     50,
     67,
     6,
     0,
     Phi_seg_frac*CLHEP::pi
   );

         G4LogicalVolume *pAcceleratorPipe_Log = new G4LogicalVolume
         (
             pAcceleratorPipe,  // its solid
            pAMG6_mat,// its material
             "AcceleratorPipe_Log"
           );

        G4LogicalVolume *pAcceleratorPipe1_Log = new G4LogicalVolume
        (
         pAcceleratorPipe1,  // its solid
        pAMG6_mat,// its material
         "AcceleratorPipe1_Log"
         );

    G4LogicalVolume *pAcceleratorPipe2_Log = new G4LogicalVolume
    (
     pAcceleratorPipe2,  // its solid
    pAMG6_mat,// its material
     "AcceleratorPipe2_Log"
     );

    G4LogicalVolume *pAcceleratorPipe3_Log = new G4LogicalVolume
    (
     pAcceleratorPipe3,  // its solid
    pAMG6_mat,// its material
     "AcceleratorPipe3_Log"
     );

    G4LogicalVolume *pAcceleratorPipe4_Log = new G4LogicalVolume
    (
     pAcceleratorPipe4,  // its solid
    pAMG6_mat,// its material
     "AcceleratorPipe4_Log"
     );

    G4LogicalVolume *pAcceleratorPipe5_Log = new G4LogicalVolume
    (
     pAcceleratorPipe5,  // its solid
    pAMG6_mat,// its material
     "AcceleratorPipe5_Log"
     );

    G4LogicalVolume *pAcceleratorPipe6_Log = new G4LogicalVolume
    (
     pAcceleratorPipe6,  // its solid
    pAMG6_mat,// its material
     "AcceleratorPipe6_Log"
     );

        G4VPhysicalVolume  *pAcceleratorPipe_Phys = new G4PVPlacement
        (
          nullptr,  // no rotation
          G4ThreeVector(0, 0, z_pos),                          // at (0,0,0)
          pAcceleratorPipe_Log,                                // its logical volume
          "AcceleratorPipe_Phys",                    // its name
          mother_volume_log,                                   // its mother  volume
          false,                                     // no boolean operation
          0,                                         // copy number
          true                                       // overlaps checking
        );

    G4VPhysicalVolume  *pAcceleratorPipe1_Phys = new G4PVPlacement
    (
      nullptr,  // no rotation
      G4ThreeVector(0, 0, z_pos-60.5),                          // at (0,0,0)
      pAcceleratorPipe1_Log,                                // its logical volume
      "AcceleratorPipe1_Phys",                    // its name
      mother_volume_log,                                   // its mother  volume
      false,                                     // no boolean operation
      0,                                         // copy number
      true                                       // overlaps checking
    );

    G4VPhysicalVolume  *pAcceleratorPipe2_Phys = new G4PVPlacement
    (
      nullptr,  // no rotation
      G4ThreeVector(0, 0, z_pos-130.5),                          // at (0,0,0)
      pAcceleratorPipe2_Log,                                // its logical volume
      "AcceleratorPipe2_Phys",                    // its name
      mother_volume_log,                                   // its mother  volume
      false,                                     // no boolean operation
      0,                                         // copy number
      true                                       // overlaps checking
    );

    G4VPhysicalVolume  *pAcceleratorPipe3_Phys = new G4PVPlacement
    (
      nullptr,  // no rotation
      G4ThreeVector(0, 0, z_pos-159),                          // at (0,0,0)
      pAcceleratorPipe3_Log,                                // its logical volume
      "AcceleratorPipe3_Phys",                    // its name
      mother_volume_log,                                   // its mother  volume
      false,                                     // no boolean operation
      0,                                         // copy number
      true                                       // overlaps checking
    );

    G4VPhysicalVolume  *pAcceleratorPipe4_Phys = new G4PVPlacement
    (
      nullptr,  // no rotation
      G4ThreeVector(0, 0, z_pos-187.5),                          // at (0,0,0)
      pAcceleratorPipe4_Log,                                // its logical volume
      "AcceleratorPipe4_Phys",                    // its name
      mother_volume_log,                                   // its mother  volume
      false,                                     // no boolean operation
      0,                                         // copy number
      true                                       // overlaps checking
    );

    G4VPhysicalVolume  *pAcceleratorPipe5_Phys = new G4PVPlacement
    (
      nullptr,  // no rotation
      G4ThreeVector(0, 0, z_pos-210),                          // at (0,0,0)
      pAcceleratorPipe5_Log,                                // its logical volume
      "AcceleratorPipe5_Phys",                    // its name
      mother_volume_log,                                   // its mother  volume
      false,                                     // no boolean operation
      0,                                         // copy number
      true                                       // overlaps checking
    );

    G4VPhysicalVolume  *pAcceleratorPipe6_Phys = new G4PVPlacement
    (
      nullptr,  // no rotation
      G4ThreeVector(0, 0, z_pos-221),                          // at (0,0,0)
      pAcceleratorPipe6_Log,                               // its logical volume
      "AcceleratorPipe6_Phys",                    // its name
      mother_volume_log,                                   // its mother  volume
      false,                                     // no boolean operation
      0,                                         // copy number
      true                                       // overlaps checking
    );
        auto fStepLimit = new G4UserLimits(maxStep);
        pAcceleratorPipe_Log ->SetUserLimits(fStepLimit);



}
