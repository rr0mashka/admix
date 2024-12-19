#include "Geometry/CellsPhantom.hh"
#include "G4VSolid.hh"
#include "G4ThreeVector.hh"
#include "G4MultiUnion.hh"
#include "G4Transform3D.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4UserLimits.hh"
#include "CustomMaterials.hh"

CellsPhantom::CellsPhantom (G4String name, G4double z_pos, G4LogicalVolume* mother_volume_log, G4double maxStep)
{

    G4NistManager* nist = G4NistManager::Instance();
    G4Material* CellsPhantomMaterial= nist->FindOrBuildMaterial("G4_PLEXIGLASS");

    CustomMaterials* pCustomMaterials = CustomMaterials::Instance();
    G4Material* CellsMaterial = pCustomMaterials->GetMaterial("SoftTissueWithBoron");

  z_pos = z_pos + 0.5*PhantomDisk_Width; // moving origin to the front face

 G4double Phi_seg_frac = 2.0;

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
    
    auto fStepLimit = new G4UserLimits(maxStep);
    pPhantomDisk_Log ->SetUserLimits(fStepLimit);
    
    
    // Пробирки с клетками
        G4double holeRadius = 8 * mm;
        G4double holeDepth = 40 * mm;
        G4double outerRadius = 50 * mm;
        G4double innerRadius = 25 * mm;
        int numOuterHoles = 16;
        int numInnerHoles = 8;

        for (int i = 0; i < numOuterHoles; ++i) {
            G4double angle = 2 * CLHEP::pi * i / numOuterHoles;
            G4double x = outerRadius * std::cos(angle);
            G4double y = outerRadius * std::sin(angle);

            G4Tubs* pHole_sol = new G4Tubs(
                "Hole_sol_outer_" + std::to_string(i),
                0,
                holeRadius,
                holeDepth * 0.5,
                0,
                2 * CLHEP::pi
            );

            G4LogicalVolume* pHole_Log = new G4LogicalVolume(
                pHole_sol,
                CellsMaterial,
                "Hole_Log_outer_" + std::to_string(i)
            );

            new G4PVPlacement(
                nullptr,
                G4ThreeVector(x, y, PhantomDisk_Width - 1.5 * holeDepth),
                pHole_Log,
                "Hole_Phys_outer_" + std::to_string(i),
                pPhantomDisk_Log,
                false,
                i,
                true
            );
        }

        for (int i = 0; i < numInnerHoles; ++i) {
            G4double angle = 2 * CLHEP::pi * i / numInnerHoles;
            G4double x = innerRadius * std::cos(angle);
            G4double y = innerRadius * std::sin(angle);

            G4Tubs* pHole_sol = new G4Tubs(
                "Hole_sol_inner_" + std::to_string(i),
                0,
                holeRadius,
                holeDepth * 0.5,
                0,
                2 * CLHEP::pi
            );

            G4LogicalVolume* pHole_Log = new G4LogicalVolume(
                pHole_sol,
                CellsMaterial,
                "Hole_Log_inner_" + std::to_string(i)
            );
            
            new G4PVPlacement(
                nullptr,
                G4ThreeVector(x, y, PhantomDisk_Width - 1.5 * holeDepth),
                pHole_Log,
                "Hole_Phys_inner_" + std::to_string(i),
                pPhantomDisk_Log,
                false,
                i + numOuterHoles,
                true
            );
        }
    }
