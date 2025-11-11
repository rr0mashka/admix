#include "Geometry/ArchimedSpiral.hh"
#include "Geometry/FrontPlate.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4UnionSolid.hh"
#include "G4GDMLParser.hh"
#include "G4MultiUnion.hh"
#include "G4Transform3D.hh"


FrontPlate::FrontPlate (G4String name, G4double z_pos, G4LogicalVolume* mother_volume_log, G4double maxStep )
{
  z_pos = z_pos + FrontPlateDisc_Width;
  G4double pi = CLHEP::pi;
  G4bool checkOverlaps = true;
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* DrainMaterial= nist->FindOrBuildMaterial("G4_WATER");
  G4Material* FrontPlateMaterial = nist->FindOrBuildMaterial("G4_Cu");
  G4Material* LitiumDiskMaterial = nist->FindOrBuildMaterial("G4_Li");
  G4Tubs *solidM0disk = new G4Tubs("SolM0disc", 0, FrontPlateDisc_Radius,FrontPlateDisc_Width,0,2*pi);
  G4LogicalVolume *M0discLog = new G4LogicalVolume(solidM0disk,  // its solid
      FrontPlateMaterial,// its material
      "LM0disc");


 /* G4VPhysicalVolume *phydisk = new G4PVPlacement(0,                    //no rotation
       G4ThreeVector(0, 0, z_pos),
       M0discLog,                //its logical volume
       "M0discPhys",                     //its name
       mother_volume_log,                     //its mother  volume
       false,                        //no boolean operation
       0,                            //copy number
       checkOverlaps);                      //overlaps checking*/

   auto spiral1 = new ArchimedSpiral("firstspiral",50,FrontPlateSpiralDrain_InnerR,FrontPlateSpiralDrain_OuterR, FrontPlateSpiralWinding,
   0.*mm,FrontPlateSpiralDrain_Depth,3*pi/2,2*pi/2);

   G4MultiUnion* sp1 = spiral1->GetSpiral();
   G4LogicalVolume *lsp1 =  new G4LogicalVolume(sp1,  // its solid
     DrainMaterial,                                       // its material
     "LSpiralDrain");

   G4ThreeVector axis1 = G4ThreeVector(0,1,0);
   G4ThreeVector axis2 = G4ThreeVector(0,0,1);
   G4ThreeVector axis3 = G4ThreeVector(1,0,0);

   G4Transform3D tr = G4Translate3D(0*mm, 0*mm, FrontPlateDisc_Width+0.1*mm);//+z_pos+0.1*mm); // 0.01*mm to make visible
   G4Transform3D rot1 = G4Rotate3D(90*deg, axis1);
   G4Transform3D rot1_2 =  G4Rotate3D(0*deg, axis3);
   G4Transform3D rm1 =tr*rot1*rot1_2;

  /* G4VPhysicalVolume *physp1 = new G4PVPlacement(rm1,                    //no rotation
          lsp1,                //its logical volume
          "PhSp1",                     //its name
          M0discLog,                     //its mother  volume
          false,                        //no boolean operation
          0,                            //copy number
          false);                      //overlaps checking
*/

   G4Transform3D rot2_2 =  G4Rotate3D(90*deg, axis3);
   G4Transform3D rm2 = tr*rot1*rot2_2;

 /* G4VPhysicalVolume *physp2 = new G4PVPlacement(rm2,                    //no rotation
         lsp1,                //its logical volume
         "PhSp2",                     //its name
         M0discLog,                     //its mother  volume
         false,                        //no boolean operation
         0,                            //copy number
         false);
*/
   G4Transform3D rot3_2 =  G4Rotate3D(180*deg, axis3);
   G4Transform3D rm3 = tr*rot1*rot3_2;

 /* G4VPhysicalVolume *physp3 = new G4PVPlacement(rm3,                    //no rotation
         lsp1,                //its logical volume
         "PhSp3",                     //its name
         M0discLog,                     //its mother  volume
         false,                        //no boolean operation
         0,                            //copy number
         false);*/
//

   G4Transform3D rot4_2 =  G4Rotate3D(270*deg, axis3);
   G4Transform3D rm4 = tr*rot1*rot4_2;

 /*  G4VPhysicalVolume *physp4 = new G4PVPlacement(rm4,                    //no rotation
          lsp1,                //its logical volume
          "PhSp4",                     //its name
          M0discLog,                     //its mother  volume
          false,                        //no boolean operation
          0,                            //copy number
          false);*/


   G4Tubs *litplate = new G4Tubs("SolLitDisc", 0, LitiumDisc_Radius,LitiumDisc_Width,0,2*pi);
   G4LogicalVolume *LitiumDiscLog = new G4LogicalVolume(litplate,  // its solid
       LitiumDiskMaterial,// its material
       "LLitiumdisc");
   G4VPhysicalVolume *phlitdisk = new G4PVPlacement(0,                    //no rotation
        G4ThreeVector(0, 0, z_pos - FrontPlateDisc_Width - LitiumDisc_Width),
        LitiumDiscLog,                //its logical volume
        "LitiumDiscPhys",                     //its name
        mother_volume_log,                     //its mother  volume
        false,                        //no boolean operation
        0,                            //copy number
        checkOverlaps);

        auto fStepLimit = new G4UserLimits(maxStep);
        LitiumDiscLog -> SetUserLimits(fStepLimit);
        M0discLog ->SetUserLimits(fStepLimit);
        lsp1->SetUserLimits(fStepLimit);

}
