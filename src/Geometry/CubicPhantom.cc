#include "Geometry/CubicPhantom.hh"
#include "G4VSolid.hh"
#include "G4ThreeVector.hh"
#include "G4MultiUnion.hh"
#include "G4Transform3D.hh"

CubicPhantom::CubicPhantom (G4String name, G4double z_pos, G4LogicalVolume* mother_volume_log )
{

z_pos = z_pos + 0.5*PhantomSize_sizeZ; // moving origin to the front face

  G4NistManager *nist = G4NistManager::Instance();
  G4Material *PhantomMaterial = nist->FindOrBuildMaterial("G4_WATER");


        G4Box* template_ScoringCell_Sol =  new G4Box
        (
          "Cube",
          0.5*ScoringCell_sizeXY,
          0.5*ScoringCell_sizeXY,
          0.5*ScoringCell_sizeZ
        );

        G4LogicalVolume *template_ScoringCell_Log = new G4LogicalVolume
        (
          template_ScoringCell_Sol,          //its solid
          PhantomMaterial,           //its material
          "template_ScoringCell_Log"
        );


        G4int  N_cells_XY = int(PhantomSize_sizeXY/ScoringCell_sizeXY);
        G4int  N_cells_Z = int(PhantomSize_sizeZ/ScoringCell_sizeZ);

        G4String temp_str;
        const G4String pref = "Cube_";

        G4VPhysicalVolume* phys_vol;
        G4double Cell_posX, Cell_posY, Cell_posZ;


        for (G4int i = 0; i<N_cells_XY; i++){
          for (G4int j = 0; j<N_cells_XY; j++){
            for (G4int k = 0; k<N_cells_Z; k++){

                temp_str = pref +std::to_string(i)+"_"+std::to_string(j)+"_"+std::to_string(k);
                Cell_posX = -0.5*(PhantomSize_sizeXY - ScoringCell_sizeXY)+ ScoringCell_sizeXY*i;
                Cell_posY = -0.5*(PhantomSize_sizeXY - ScoringCell_sizeXY)+ ScoringCell_sizeXY*j;
                Cell_posZ = -0.5*(PhantomSize_sizeZ - ScoringCell_sizeZ)+ ScoringCell_sizeZ*k;

                vPos_X.push_back(Cell_posX);
                vPos_Y.push_back(Cell_posY);
                vPos_Z.push_back(Cell_posZ);

                //std::cout<<"       DetectorConstruction   " << pos_X << "   "
                //<<pos_Y<< "   " <<pos_Z<< std::endl;

                  phys_vol = new G4PVPlacement
                  (
                          0,                    //no rotation
                          G4ThreeVector(Cell_posX, Cell_posY, Cell_posZ + z_pos),
                          template_ScoringCell_Log,                //its logical volume
                          temp_str,                     //its name
                          mother_volume_log,                     //its mother  volume
                          false,                        //no boolean operation
                          0,                            //copy number
                          true
                  );                        //overlaps checking

                 fScoringCubes.push_back(phys_vol);

            }
          }
       }





}
