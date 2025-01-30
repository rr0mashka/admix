#include "Geometry/SphericalPhantom.hh"
#include "G4VSolid.hh"
#include "G4ThreeVector.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Sphere.hh"
#include "CustomMaterials.hh"
#include "Geometry/SensitiveVolumes/PhantomVolume.hh"
#include "G4SDManager.hh"
#include <cmath>

SphericalPhantom::SphericalPhantom(G4String name, G4double z_pos, G4LogicalVolume* mother_volume_log, G4double maxStep, G4double tissueradius, G4double bonelayer, G4double skinlayer) {
    G4double z_pos_cent = z_pos; // Смещение по Z

    CustomMaterials* pCustomMaterials = CustomMaterials::Instance();
    G4Material* PhantomMaterial = pCustomMaterials->GetMaterial("CatBrain");
    G4Material* BonesMaterial = pCustomMaterials->GetMaterial("skeleton");
    G4Material* SkinMaterial = pCustomMaterials->GetMaterial("soft_tissue");

    // Создание внутренней сферы (мозга)
    G4Box* template_ScoringCell_Sol = new G4Box("Cube",
        0.5 * ScoringCell_sizeXY,
        0.5 * ScoringCell_sizeXY,
        0.5 * ScoringCell_sizeZ);

    template_BrainCell_Log = new G4LogicalVolume(
        template_ScoringCell_Sol, PhantomMaterial, "template_BrainCell_Log");

    template_BoneCell_Log = new G4LogicalVolume(
        template_ScoringCell_Sol, PhantomMaterial, "template_BoneCell_Log");

    template_SkinCell_Log = new G4LogicalVolume(
        template_ScoringCell_Sol, PhantomMaterial, "template_SkinCell_Log");

    fLogvolumes.push_back(template_BrainCell_Log);
    fLogvolumes.push_back(template_BoneCell_Log);
    fLogvolumes.push_back(template_SkinCell_Log);

    G4int N_tissuecells_XY = (int(tissueradius/ScoringCell_sizeXY)%2 == 0)? int(tissueradius/ScoringCell_sizeXY) : int(tissueradius/ScoringCell_sizeXY)+1;
    G4int N_tissuecells_Z = (int(tissueradius / ScoringCell_sizeZ)%2 == 0)? int(tissueradius / ScoringCell_sizeZ) : int(tissueradius / ScoringCell_sizeZ)+1;

    G4double boneradius =  bonelayer + tissueradius;
    G4int N_bonecells_XY = (int(boneradius/ScoringCell_sizeXY)%2 == 0)? int(boneradius/ScoringCell_sizeXY) : int(boneradius/ScoringCell_sizeXY)+1;
    G4int N_bonecells_Z = (int(boneradius/ScoringCell_sizeZ)%2 == 0)? int(boneradius/ScoringCell_sizeZ) : int(boneradius/ScoringCell_sizeZ)+1;

    G4double skinradius = skinlayer + bonelayer + tissueradius;

    G4int N_skincells_XY = (int(skinradius/ScoringCell_sizeXY)%2 == 0)? int(skinradius/ScoringCell_sizeXY) : int(skinradius/ScoringCell_sizeXY)+1;
    G4int N_skincells_Z = (int(skinradius/ScoringCell_sizeZ)%2 == 0)? int(skinradius/ScoringCell_sizeZ) : int(skinradius/ScoringCell_sizeZ)+1;

    G4String temp_str;
    const G4String pref = "Voxel_"; // Измененный префикс

    // Параметры для размещения вокселей
    G4VPhysicalVolume* phys_vol;

    G4double Cell_posX,Cell_posY, Cell_posZ;
    // Создание вокселей внутри фантома
    G4int sgni, sgnj, sgnk;
    G4double sqradiuscheck;
    G4double sqinnercheck;

    for (G4int i = -N_tissuecells_XY; i < N_tissuecells_XY; i++) {
        for (G4int j = -N_tissuecells_XY; j < N_tissuecells_XY; j++) {
            for (G4int k = -N_tissuecells_XY; k < N_tissuecells_XY; k++) {

                Cell_posX =  ScoringCell_sizeXY * i + 0.5*ScoringCell_sizeXY;//-0.5*ScoringCell_sizeXY + ScoringCell_sizeXY * i;
                Cell_posY =  ScoringCell_sizeXY * j + 0.5*ScoringCell_sizeXY;;//-0.5*ScoringCell_sizeXY + ScoringCell_sizeXY * j;
                Cell_posZ =  ScoringCell_sizeZ * k + z_pos_cent + 0.5*ScoringCell_sizeZ;//-0.5*ScoringCell_sizeZ + ScoringCell_sizeZ * k + z_pos_cent;

                // Проверка, находится ли воксель внутри сферы
                sgni = (i>0) ? 1: -1;
                sgnj = (j>0) ? 1: -1;
                sgnk = (k>0) ? 1: -1;
                sqradiuscheck = (Cell_posX+sgni*0.5*ScoringCell_sizeXY)*(Cell_posX+sgni*0.5*ScoringCell_sizeXY) + (Cell_posY + sgnj*0.5*ScoringCell_sizeXY) * (Cell_posY + sgnj*0.5*ScoringCell_sizeXY) + (Cell_posZ - z_pos_cent + sgnk*0.5*ScoringCell_sizeZ) * (Cell_posZ - z_pos_cent + sgnk*0.5*ScoringCell_sizeZ);

                if (std::sqrt(sqradiuscheck) <= tissueradius) {
                    temp_str = pref + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k);

                    phys_vol = new G4PVPlacement(
                        nullptr,
                        G4ThreeVector(Cell_posX, Cell_posY, Cell_posZ),
                        template_BrainCell_Log,
                        temp_str,
                        mother_volume_log,
                        false,
                        0,
                        false);

//                    fScoringCubes.push_back(phys_vol);
                    // Заполнение векторов позиций
                    vPos_X.push_back(Cell_posX);
                    vPos_Y.push_back(Cell_posY);
                    vPos_Z.push_back(Cell_posZ);
                }
            }
        }
    }

    // Создание вокселей черепа, окружающих фантом мозга

   for (G4int i = -N_bonecells_Z; i < N_bonecells_XY; i++) {
        for (G4int j = -N_bonecells_XY; j < N_bonecells_XY; j++) {
            for (G4int k = -N_bonecells_Z; k < N_bonecells_Z; k++) {
              Cell_posX =  ScoringCell_sizeXY * i + 0.5*ScoringCell_sizeXY;//-0.5*ScoringCell_sizeXY + ScoringCell_sizeXY * i;
              Cell_posY =  ScoringCell_sizeXY * j + 0.5*ScoringCell_sizeXY;//-0.5*ScoringCell_sizeXY + ScoringCell_sizeXY * j;
              Cell_posZ =  ScoringCell_sizeZ * k + z_pos_cent + 0.5*ScoringCell_sizeZ;
              sgni = (i>0) ? 1: -1;
              sgnj = (j>0) ? 1: -1;
              sgnk = (k>0) ? 1: -1;

              // Проверка, находится ли воксель на границе сферической оболочки
              sqinnercheck = std::sqrt(Cell_posX * Cell_posX + Cell_posY * Cell_posY + (Cell_posZ - z_pos_cent) * (Cell_posZ - z_pos_cent));
              sqradiuscheck = (Cell_posX+sgni*0.5*ScoringCell_sizeXY)*(Cell_posX+sgni*0.5*ScoringCell_sizeXY) + (Cell_posY + sgnj*0.5*ScoringCell_sizeXY) * (Cell_posY + sgnj*0.5*ScoringCell_sizeXY) + (Cell_posZ - z_pos_cent + sgnk*0.5*ScoringCell_sizeZ) * (Cell_posZ - z_pos_cent + sgnk*0.5*ScoringCell_sizeZ);
                // Проверка, находится ли воксель между внутренним и внешним радиусом
                if ((std::sqrt(sqradiuscheck) <= boneradius) && (sqinnercheck > tissueradius)) {
                    temp_str = "Bones_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k);

                    phys_vol = new G4PVPlacement(
                        nullptr,
                        G4ThreeVector(Cell_posX, Cell_posY, Cell_posZ),
                        template_BoneCell_Log,
                        temp_str,
                        mother_volume_log,
                        false,
                        0,
                        false);

//                    fScoringCubes.push_back(phys_vol);
                    // Заполнение векторов позиций
                    vPos_X.push_back(Cell_posX);
                    vPos_Y.push_back(Cell_posY);
                    vPos_Z.push_back(Cell_posZ);
                }
            }
        }
    };

    // Создание вокселей из кожи

    for (G4int i = -N_skincells_XY; i < N_skincells_XY; i++) {
        for (G4int j = -N_skincells_XY; j < N_skincells_XY; j++) {
            for (G4int k = -N_skincells_Z; k < N_skincells_Z; k++) {
              Cell_posX =  ScoringCell_sizeXY * i + 0.5*ScoringCell_sizeXY;//-0.5*ScoringCell_sizeXY + ScoringCell_sizeXY * i;
              Cell_posY =  ScoringCell_sizeXY * j + 0.5*ScoringCell_sizeXY;;//-0.5*ScoringCell_sizeXY + ScoringCell_sizeXY * j;
              Cell_posZ =  ScoringCell_sizeZ * k + z_pos_cent + 0.5*ScoringCell_sizeZ;
              sgni = (i>0) ? 1: -1;
              sgnj = (j>0) ? 1: -1;
              sgnk = (k>0) ? 1: -1;

              // Проверка, находится ли воксель на границе сферической оболочки
              sqinnercheck = std::sqrt(Cell_posX * Cell_posX + Cell_posY * Cell_posY + (Cell_posZ - z_pos_cent) * (Cell_posZ - z_pos_cent));
              sqradiuscheck = (Cell_posX+sgni*0.5*ScoringCell_sizeXY)*(Cell_posX+sgni*0.5*ScoringCell_sizeXY) + (Cell_posY + sgnj*0.5*ScoringCell_sizeXY) * (Cell_posY + sgnj*0.5*ScoringCell_sizeXY) + (Cell_posZ - z_pos_cent + sgnk*0.5*ScoringCell_sizeZ) * (Cell_posZ - z_pos_cent + sgnk*0.5*ScoringCell_sizeZ);
                // Проверка, находится ли воксель между внутренним и внешним радиусом
                if ((std::sqrt(sqradiuscheck) <= skinradius) && (sqinnercheck > boneradius)) {
                    temp_str = "Skin_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k);

                    phys_vol = new G4PVPlacement(
                        nullptr,
                        G4ThreeVector(Cell_posX, Cell_posY, Cell_posZ),
                        template_SkinCell_Log,
                        temp_str,
                        mother_volume_log,
                        false,
                        0,
                        false);


//                    fScoringCubes.push_back(phys_vol);
                    // Заполнение векторов позиций
                    vPos_X.push_back(Cell_posX);
                    vPos_Y.push_back(Cell_posY);
                    vPos_Z.push_back(Cell_posZ);
                }
            }
        }
    };


    // Установка ограничений
//  Set sensitive volumes

    auto fStepLimit = new G4UserLimits(maxStep);
    template_BrainCell_Log->SetUserLimits(fStepLimit);
    template_BoneCell_Log->SetUserLimits(fStepLimit);
    template_SkinCell_Log->SetUserLimits(fStepLimit);




};
