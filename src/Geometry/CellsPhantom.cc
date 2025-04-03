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
    int slices = 5;

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
         ScoringCell_radius,
         0.5 *  ScoringCell_sizeZ,
         0,
         Phi_seg_frac*CLHEP::pi
    );

    template_Cells_Log = new G4LogicalVolume(
         template_ScoringCell_Sol, CellsMaterial, "template_Cells_Log");

    fLogvolumes.push_back(template_Cells_Log);

    G4int N = 1;
    G4int n = 1; //capThickness/ScoringCell_sizeZ;

    G4String temp_str;

    // Параметры для размещения вокселей
    G4VPhysicalVolume* phys_vol;

    G4double Cell_posX, Cell_posY, Cell_posZ;

    // внешние без крышек
    for (int k = 0; k < numOuterHoles; ++k) {
      for(G4int i = 0; i<N; i++){
        G4double angle = 2 * CLHEP::pi * k / numOuterHoles;
        Cell_posX = outerRadius * std::cos(angle);
        Cell_posY = outerRadius * std::sin(angle);
        Cell_posZ = 0.1*PhantomDisk_Width;
        temp_str = "Voxel1_" + std::to_string(i)+ "_"+ std::to_string(k);
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
    for(G4int i = 0; i < n; i++){
      G4double angle = 2 * CLHEP::pi * k / numOuterHoles;
      Cell_posX = outerRadius * std::cos(angle);
      Cell_posY = outerRadius * std::sin(angle);
      Cell_posZ = z_pos + 0.5*PhantomDisk_Width+(i+0.5)*ScoringCell_sizeZ + 5.0;
      temp_str = "Voxel2_" + std::to_string(i) + "_"+ std::to_string(k);
      phys_vol = new G4PVPlacement(
          nullptr,
          G4ThreeVector(Cell_posX, Cell_posY, Cell_posZ),
          template_Cells_Log,
          temp_str,
          mother_volume_log,
          false,
          0,
          true);
      vPos_X.push_back(Cell_posX);
      vPos_Y.push_back(Cell_posY);
      vPos_Z.push_back(Cell_posZ);
    }
  }

    // внутренние без крышек
    for (int k = 0; k < numInnerHoles; ++k) {
    for(G4int i = 0; i<N; i++){
        G4double angle = 2 * CLHEP::pi * k / numInnerHoles;
        Cell_posX = innerRadius * std::cos(angle);
        Cell_posY = innerRadius * std::sin(angle);
        Cell_posZ = 0.1*PhantomDisk_Width;
        temp_str = "Voxel3_" + std::to_string(N+i)+ "_"+ std::to_string(k);
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

      for(G4int i = 0; i<n; i++){
      G4double angle = 2 * CLHEP::pi * k / numInnerHoles;
      Cell_posX = innerRadius * std::cos(angle);
      Cell_posY = innerRadius * std::sin(angle);
      Cell_posZ = z_pos + 0.5*PhantomDisk_Width+(i+0.5)*ScoringCell_sizeZ+ 5.0;
      temp_str = "Voxel4_" + std::to_string(N+i)+ "_"+ std::to_string(k);
      phys_vol = new G4PVPlacement(
          nullptr,
          G4ThreeVector(Cell_posX, Cell_posY, Cell_posZ),
          template_Cells_Log,
          temp_str,
          mother_volume_log,
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
    template_Cells_Log->SetUserLimits(fStepLimit);

    }
