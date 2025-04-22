#include "Geometry/CellsPhantom.hh"
#include "G4VSolid.hh"
#include "G4ThreeVector.hh"
#include "G4MultiUnion.hh"
#include "G4Transform3D.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4UserLimits.hh"
#include "CustomMaterials.hh"
#include "Geometry/SensitiveVolumes/PhantomVolume.hh"

CellsPhantom::CellsPhantom (G4String name, G4double z_pos, G4LogicalVolume* mother_volume_log, G4double maxStep)
{

    G4NistManager* nist = G4NistManager::Instance();
    G4Material* CellsPhantomMaterial= nist->FindOrBuildMaterial("G4_PLEXIGLASS");
    G4Material* TubeMaterial= nist->FindOrBuildMaterial("G4_POLYPROPYLENE");
    G4Material* AirMaterial= nist->FindOrBuildMaterial("G4_AIR");


    CustomMaterials* pCustomMaterials = CustomMaterials::Instance();
    G4Material* CellsMaterial = pCustomMaterials->GetMaterial("SoftTissueWithBoron");

    z_pos = z_pos + 0.5*PhantomDisk_Width; // moving origin to the front face

    G4double Phi_seg_frac = 2.0;
    G4double holeRadius = 8 * mm;
    G4double holeDepth = 8 * mm;
    G4double capThickness = 40 * mm;
    G4double outerRadius = 50 * mm;
    G4double innerRadius = 25 * mm;
    int numOuterHoles = 16;
    int numInnerHoles = 8;

    G4Tubs *pPhantomGlassDisk_sol = new G4Tubs
    (
       "PhantomGlassDisk_sol",
        0,
        0.5 * PhantomDisk_Diamter,
        0.5 * PhantomDisk_Width,
        0,
        Phi_seg_frac*CLHEP::pi
     );

    G4LogicalVolume *pPhantomDisk_Log = new G4LogicalVolume
    (
        pPhantomGlassDisk_sol,  // its solid
        CellsPhantomMaterial,// its material
        "PhantomDisk_Log"
      );

     G4VPhysicalVolume  *pPhantomDisk_Phys = new G4PVPlacement
     (
       nullptr,  // no rotation
       G4ThreeVector(0, 0, z_pos),                          // at (0,0,0)
       pPhantomDisk_Log,                                // its logical volume
       "PhantomDisk_Phys",                                  // its name
       mother_volume_log,                                   // its mother  volume
       false,                                     // no boolean operation
       0,                                         // copy number
       true                                       // overlaps checking
     );

    G4Tubs* template_ScoringCell_Sol = new G4Tubs("Tub",
         0,
         ScoringCell_radius - Tube_Thickness,
         0.5 *  ScoringCell_sizeZ,
         0,
         Phi_seg_frac*CLHEP::pi
    );

    template_Cells_Log = new G4LogicalVolume(
         template_ScoringCell_Sol, CellsMaterial, "template_Cells_Log");

    fLogvolumes.push_back(template_Cells_Log);

    G4Tubs* Air_Sol = new G4Tubs("Air",
        0,
        ScoringCell_radius - Tube_Thickness,
        ScoringCell_sizeZ,
        0,
        Phi_seg_frac*CLHEP::pi
         );

    G4LogicalVolume *Air_Log = new G4LogicalVolume(
         Air_Sol, AirMaterial, "Air_Log");

    G4Tubs* Air_Sol_1 = new G4Tubs("Air_1",
         0,
         ScoringCell_radius - Tube_Thickness,
         ScoringCell_sizeZ*0.5,
         0,
         Phi_seg_frac*CLHEP::pi
      );

    G4LogicalVolume *Air_Log_1 = new G4LogicalVolume(
        Air_Sol_1, AirMaterial, "Air_Log_1");

    G4Tubs* Tube_Sol_1 = new G4Tubs("Tub_sol_1",
         ScoringCell_radius - Tube_Thickness,
         ScoringCell_radius,
         0.5 *  Tube_Length_1,
         0,
         Phi_seg_frac*CLHEP::pi
    );

    G4LogicalVolume *Tube_Log_1 = new G4LogicalVolume(
         Tube_Sol_1, TubeMaterial, "Tube_Log_1");

    G4Tubs* Tube_Sol_2 = new G4Tubs("Tub_sol_2",
         ScoringCell_radius - Tube_Thickness,
         ScoringCell_radius + 1*mm,
         0.5 *  Tube_Length_2,
         0,
         Phi_seg_frac*CLHEP::pi
    );

    G4LogicalVolume *Tube_Log_2 = new G4LogicalVolume(
         Tube_Sol_2, TubeMaterial, "Tube_Log_2");

    G4Tubs* Tube_Sol_3 = new G4Tubs("Tub_sol_3",
              ScoringCell_radius - Tube_Thickness,
              ScoringCell_radius,
              0.5 * Tube_Length_1 / 3.,
              0,
              Phi_seg_frac*CLHEP::pi
         );

    G4LogicalVolume *Tube_Log_3 = new G4LogicalVolume(
              Tube_Sol_3, TubeMaterial, "Tube_Log_1");

    G4Tubs* Tube_Disk_1= new G4Tubs("Tubе_disk_1",
          0,
          ScoringCell_radius,
          0.5 *  3*mm,
          0,
          Phi_seg_frac*CLHEP::pi
    );

    G4LogicalVolume *Tube_Disk_Log_1 = new G4LogicalVolume(
          Tube_Disk_1, TubeMaterial, "Tube_Disk_Log_1");

    G4Tubs* Tube_Disk_2= new G4Tubs("Tubе_disk_2",
                0,
                ScoringCell_radius + 1*mm,
                0.5 *  3*mm,
                0,
                Phi_seg_frac*CLHEP::pi
          );

    G4LogicalVolume *Tube_Disk_Log_2 = new G4LogicalVolume(
          Tube_Disk_2, TubeMaterial, "Tube_Disk_Log_2");

    G4String temp_str;

    // Параметры для размещения вокселей
    G4VPhysicalVolume* phys_vol;

    G4double Cell_posX, Cell_posY, Cell_posZ;
    G4double PosX, PosY, PosZ_1, PosZ_2, PosZ_3, PosZ_4;

    for (int k = 0; k < numOuterHoles; ++k) {
        G4double angle = 2 * CLHEP::pi * k / numOuterHoles;
        PosX = outerRadius * std::cos(angle);
        PosY = outerRadius * std::sin(angle);
        PosZ_1 = - 0.5*PhantomDisk_Width + 0.5*Tube_Length_1;
        PosZ_3 = - 0.5*PhantomDisk_Width + Tube_Length_1 + 0.5*3*mm;
        PosZ_2 = z_pos - 0.5*PhantomDisk_Width - 0.5*Tube_Length_2;
        PosZ_4 = z_pos - 0.5*PhantomDisk_Width - Tube_Length_2 - 0.5*3*mm;
        temp_str = "Voxel1_" +  std::to_string(k);
        phys_vol = new G4PVPlacement(
            nullptr,
            G4ThreeVector(PosX, PosY, PosZ_1),
            Tube_Log_1,
            temp_str,
            pPhantomDisk_Log,
            false,
            0,
            true);
        temp_str = "Voxel2_" + std::to_string(k);
        phys_vol = new G4PVPlacement(
            nullptr,
            G4ThreeVector(PosX, PosY, PosZ_2),
            Tube_Log_2,
            temp_str,
            mother_volume_log,
            false,
            0,
            true);
       temp_str = "Voxel3_" +  std::to_string(k);
        phys_vol = new G4PVPlacement(
            nullptr,
            G4ThreeVector(PosX, PosY, PosZ_3),
            Tube_Disk_Log_1,
            temp_str,
            pPhantomDisk_Log,
            false,
            0,
            true);
        temp_str = "Voxel4_" + std::to_string(k);
        phys_vol = new G4PVPlacement(
            nullptr,
            G4ThreeVector(PosX, PosY, PosZ_4),
            Tube_Disk_Log_2,
            temp_str,
            mother_volume_log,
            false,
            0,
            true);
        temp_str = "Voxel11_" +  std::to_string(k);
        phys_vol = new G4PVPlacement(
            nullptr,
            G4ThreeVector(PosX, PosY, -15*mm),
            Air_Log,
            temp_str,
            pPhantomDisk_Log,
            false,
            0,
            true);
        temp_str = "Voxel12_" +  std::to_string(k);
        phys_vol = new G4PVPlacement(
            nullptr,
            G4ThreeVector(PosX, PosY, PosZ_2),
            Air_Log_1,
            temp_str,
            mother_volume_log,
            false,
            0,
            true);
        temp_str = "Voxel14_" +  std::to_string(k);
        phys_vol = new G4PVPlacement(
            nullptr,
            G4ThreeVector(PosX, PosY, Tube_Length_1 - 0.5*PhantomDisk_Width + 2.5*mm),
            Tube_Log_3,
            temp_str,
            pPhantomDisk_Log,
            false,
            0,
            true);
    }

    G4int N=5;

    for (int k = 0; k < numInnerHoles; ++k) {
        G4double angle = 2 * CLHEP::pi * k / numInnerHoles;
        PosX = innerRadius * std::cos(angle);
        PosY = innerRadius * std::sin(angle);
        PosZ_1 = - 0.5*PhantomDisk_Width + 0.5*Tube_Length_1;
        PosZ_3 = - 0.5*PhantomDisk_Width + Tube_Length_1 + 0.5*3*mm;
        PosZ_2 = z_pos - 0.5*PhantomDisk_Width - 0.5*Tube_Length_2;
        PosZ_4 = z_pos - 0.5*PhantomDisk_Width - Tube_Length_2 - 0.5*3*mm;
        temp_str = "Voxel9_" +  std::to_string(k);
        phys_vol = new G4PVPlacement(
            nullptr,
            G4ThreeVector(PosX, PosY, PosZ_1),
            Tube_Log_1,
            temp_str,
            pPhantomDisk_Log,
            false,
            0,
            true);
        temp_str = "Voxel5_" + std::to_string(k);
        phys_vol = new G4PVPlacement(
            nullptr,
            G4ThreeVector(PosX, PosY, PosZ_2),
            Tube_Log_2,
            temp_str,
            mother_volume_log,
            false,
            0,
            true);
       temp_str = "Voxel6_" +  std::to_string(k);
        phys_vol = new G4PVPlacement(
            nullptr,
            G4ThreeVector(PosX, PosY, PosZ_3),
            Tube_Disk_Log_1,
            temp_str,
            pPhantomDisk_Log,
            false,
            0,
            true);
        temp_str = "Voxel7_" + std::to_string(k);
        phys_vol = new G4PVPlacement(
            nullptr,
            G4ThreeVector(PosX, PosY, PosZ_4),
            Tube_Disk_Log_2,
            temp_str,
            mother_volume_log,
            false,
            0,
            true);
        temp_str = "Voxel8_" + std::to_string(k);
        phys_vol = new G4PVPlacement(
            nullptr,
            G4ThreeVector(PosX, PosY, -15*mm),
            Air_Log,
            temp_str,
            pPhantomDisk_Log,
            false,
            0,
            true);
      temp_str = "Voxel13_" +  std::to_string(k);
      phys_vol = new G4PVPlacement(
            nullptr,
            G4ThreeVector(PosX, PosY, PosZ_2),
            Air_Log_1,
            temp_str,
            mother_volume_log,
            false,
            0,
            true);
      temp_str = "Voxel15_" +  std::to_string(k);
      phys_vol = new G4PVPlacement(
            nullptr,
            G4ThreeVector(PosX, PosY, Tube_Length_1 - 0.5*PhantomDisk_Width + 2.5*mm),
            Tube_Log_3,
            temp_str,
            pPhantomDisk_Log,
            false,
            0,
            true);
    }

    for (int k = 0; k < numOuterHoles; ++k) {
    for(int i=0; i<N; ++i){
        G4double angle = 2 * CLHEP::pi * k / numOuterHoles;
        Cell_posX = outerRadius * std::cos(angle);
        Cell_posY = outerRadius * std::sin(angle);
        Cell_posZ = Tube_Length_1  - 0.5*PhantomDisk_Width - (0.5+i)*ScoringCell_sizeZ;
        temp_str = "Voxel20_" + std::to_string(k) +"_"+ std::to_string(i);
        phys_vol = new G4PVPlacement(
            nullptr,
            G4ThreeVector(Cell_posX, Cell_posY, Cell_posZ),
            template_Cells_Log,
            temp_str,
            pPhantomDisk_Log,
            false,
            0,
            true);
        vPos_X.push_back(Cell_posX);
        vPos_Y.push_back(Cell_posY);
        vPos_Z.push_back(Cell_posZ);
    }
  }

    for (int k = 0; k < numInnerHoles; ++k) {
    for(int i=0; i<N; ++i){
        G4double angle = 2 * CLHEP::pi * k / numInnerHoles;
        Cell_posX = innerRadius * std::cos(angle);
        Cell_posY = innerRadius * std::sin(angle);
        Cell_posZ = Tube_Length_1 -0.5*PhantomDisk_Width - (0.5+i)*ScoringCell_sizeZ;
        temp_str = "Voxel10_" + std::to_string(k) +"_"+ std::to_string(i);
        phys_vol = new G4PVPlacement(
            nullptr,
            G4ThreeVector(Cell_posX, Cell_posY, Cell_posZ),
            template_Cells_Log,
            temp_str,
            pPhantomDisk_Log,
            false,
            0,
            true);
        vPos_X.push_back(Cell_posX);
        vPos_Y.push_back(Cell_posY);
        vPos_Z.push_back(Cell_posZ);
}
}
    auto fStepLimit = new G4UserLimits(maxStep);
    pPhantomDisk_Log->SetUserLimits(fStepLimit);
    Tube_Log_1->SetUserLimits(fStepLimit);
    Tube_Log_2->SetUserLimits(fStepLimit);
    Tube_Disk_Log_1->SetUserLimits(fStepLimit);
    Tube_Disk_Log_2->SetUserLimits(fStepLimit);
    template_Cells_Log->SetUserLimits(fStepLimit);

}
