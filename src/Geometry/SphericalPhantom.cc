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
#include <cmath>

SphericalPhantom::SphericalPhantom(G4String name, G4double z_pos, G4LogicalVolume* mother_volume_log, G4double maxStep) {
    G4double z_pos_cent = z_pos + 0.5 * PhantomSize_sizeZ; // Смещение по Z

    CustomMaterials* pCustomMaterials = CustomMaterials::Instance();
    G4Material* PhantomMaterial = pCustomMaterials->GetMaterial("CatBrain");
    G4Material* BonesMaterial = pCustomMaterials->GetMaterial("skeleton");
    G4Material* SkinMaterial = pCustomMaterials->GetMaterial("soft_tissue");

    // Создание внутренней сферы (мозга)
    G4Box* template_ScoringCell_Sol = new G4Box("Cube",
        0.5 * ScoringCell_sizeXY,
        0.5 * ScoringCell_sizeXY,
        0.5 * ScoringCell_sizeZ);

    G4LogicalVolume* template_ScoringCell_Log = new G4LogicalVolume(
        template_ScoringCell_Sol, PhantomMaterial, "template_ScoringCell_Log");

    G4int N_cells_XY = int(PhantomSize_sizeXY / ScoringCell_sizeXY);
    G4int N_cells_Z = int(PhantomSize_sizeZ / ScoringCell_sizeZ);

    G4String temp_str;
    const G4String pref = "Voxel_"; // Измененный префикс

    // Параметры для размещения вокселей
    G4VPhysicalVolume* phys_vol;

    // Радиус внутренней сферы
    G4double inner_radius = 0.5 * PhantomSize_sizeXY; // Радиус существующей сферы

    // Создание вокселей внутри фантома
    for (G4int i = 0; i < N_cells_XY; i++) {
        for (G4int j = 0; j < N_cells_XY; j++) {
            for (G4int k = 0; k < N_cells_Z; k++) {

                G4double Cell_posX = -0.5 * (PhantomSize_sizeXY - ScoringCell_sizeXY) + ScoringCell_sizeXY * i;
                G4double Cell_posY = -0.5 * (PhantomSize_sizeXY - ScoringCell_sizeXY) + ScoringCell_sizeXY * j;
                G4double Cell_posZ = -0.5 * (PhantomSize_sizeZ - ScoringCell_sizeZ) + ScoringCell_sizeZ * k + z_pos_cent;

                // Проверка, находится ли воксель внутри сферы
                if (std::sqrt(Cell_posX * Cell_posX + Cell_posY * Cell_posY + (Cell_posZ - z_pos_cent) * (Cell_posZ - z_pos_cent)) <= inner_radius) {
                    temp_str = pref + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k);

                    phys_vol = new G4PVPlacement(
                        nullptr,
                        G4ThreeVector(Cell_posX, Cell_posY, Cell_posZ),
                        template_ScoringCell_Log,
                        temp_str,
                        mother_volume_log,
                        false,
                        0,
                        true);

                    fScoringCubes.push_back(phys_vol);
                    // Заполнение векторов позиций
                    vPos_X.push_back(Cell_posX);
                    vPos_Y.push_back(Cell_posY);
                    vPos_Z.push_back(Cell_posZ);
                }
            }
        }
    }

    // Создание вокселей черепа, окружающих фантом мозга
    G4int N_cells_Bones_XY = int((inner_radius + 0.5 * ScoringCell_sizeXY) / ScoringCell_sizeXY);
    G4int N_cells_Bones_Z = int((inner_radius + 0.5 * ScoringCell_sizeZ) / ScoringCell_sizeZ);

    // Создание вокселей черепа
    for (G4int i = -N_cells_Bones_XY; i <= N_cells_Bones_XY; i++) {
        for (G4int j = -N_cells_Bones_XY; j <= N_cells_Bones_XY; j++) {
            for (G4int k = -N_cells_Bones_Z; k <= N_cells_Bones_Z; k++) {
                G4double Cell_posX = ScoringCell_sizeXY * i;
                G4double Cell_posY = ScoringCell_sizeXY * j;
                G4double Cell_posZ = z_pos_cent + ScoringCell_sizeZ * k;

                // Проверка, находится ли воксель на границе сферической оболочки
                G4double distance_to_inner = std::sqrt(Cell_posX * Cell_posX + Cell_posY * Cell_posY + (Cell_posZ - z_pos_cent) * (Cell_posZ - z_pos_cent));

                // Проверка, находится ли воксель между внутренним и внешним радиусом
                if (distance_to_inner <= (inner_radius + 0.5 * ScoringCell_sizeXY) && distance_to_inner > inner_radius) {
                    temp_str = "Bones_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k);

                    phys_vol = new G4PVPlacement(
                        nullptr,
                        G4ThreeVector(Cell_posX, Cell_posY, Cell_posZ),
                        new G4LogicalVolume(template_ScoringCell_Sol, BonesMaterial, "BonesVoxelLog"),
                        temp_str,
                        mother_volume_log,
                        false,
                        0,
                        true);

                    // Установка атрибутов видимости для черепа
                    G4VisAttributes* visAttributesBones = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.2));
                    visAttributesBones->SetVisibility(true);
                    phys_vol->GetLogicalVolume()->SetVisAttributes(visAttributesBones);

                    fScoringCubes.push_back(phys_vol);
                }
            }
        }
    }

    // Создание вокселей из кожи, окружающей череп
    G4int N_cells_Skin_XY = int((inner_radius + ScoringCell_sizeXY) / ScoringCell_sizeXY);
    G4int N_cells_Skin_Z = int((inner_radius + ScoringCell_sizeZ) / ScoringCell_sizeZ);

    // Создание вокселей из кожи
    for (G4int i = -N_cells_Skin_XY; i <= N_cells_Skin_XY; i++) {
        for (G4int j = -N_cells_Skin_XY; j <= N_cells_Skin_XY; j++) {
            for (G4int k = -N_cells_Skin_Z; k <= N_cells_Skin_Z; k++) {
                G4double Cell_posX = ScoringCell_sizeXY * i;
                G4double Cell_posY = ScoringCell_sizeXY * j;
                G4double Cell_posZ = z_pos_cent + ScoringCell_sizeZ * k;

                // Проверка, находится ли воксель кожи на границе черепа
                G4double distance_to_outer = std::sqrt(Cell_posX * Cell_posX + Cell_posY * Cell_posY + (Cell_posZ - z_pos_cent) * (Cell_posZ - z_pos_cent));

                // Проверка, находится ли воксель между внешним радиусом черепа и внешним радиусом кожи
                if (distance_to_outer <= (inner_radius + ScoringCell_sizeXY) && distance_to_outer > (inner_radius + 0.5 * ScoringCell_sizeXY)) {
                    temp_str = "Skin_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k);

                    phys_vol = new G4PVPlacement(
                        nullptr,
                        G4ThreeVector(Cell_posX, Cell_posY, Cell_posZ),
                        new G4LogicalVolume(template_ScoringCell_Sol, SkinMaterial, "SkinVoxelLog"),
                        temp_str,
                        mother_volume_log,
                        false,
                        0,
                        true);

                    // Установка атрибутов видимости для кожи
                    G4VisAttributes* visAttributesSkin = new G4VisAttributes(G4Colour(1.0, 0.8, 0.6, 0.1));
                    visAttributesSkin->SetVisibility(true);
                    phys_vol->GetLogicalVolume()->SetVisAttributes(visAttributesSkin);

                    fScoringCubes.push_back(phys_vol);
                }
            }
        }
    }

    // Установка ограничений
    auto fStepLimit = new G4UserLimits(maxStep);
    template_ScoringCell_Log->SetUserLimits(fStepLimit);
}
