//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1/src/DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4CutTubs.hh"
#include "Geometry/ArchimedSpiral.hh"
#include "Geometry/BackPlate.hh"

// GDML
#include "G4GDMLParser.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4bool checkOverlaps = true;
  // %%%%% materials %%%%%%
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  //
  const G4String water_mat_name = "G4_WATER";
  G4Material* water_mat = nist->FindOrBuildMaterial(water_mat_name);
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* copperM0 = nist->FindOrBuildMaterial("G4_Cu");
  G4Material* LiMat = nist->FindOrBuildMaterial("G4_Li");


// %%%%% scales  %%%%%%

  const G4double world_sizeXY = 20*cm;
  const G4double world_sizeZ  = 20*cm;


  const G4double M0discwidth = 10*mm;
  const G4double M0discradius = 143.*0.5*mm;

// world %%%%


  auto solidWorld = new G4Box("World",                           // its name
    0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size

  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
    world_mat,                                       // its material
    "World");                                        // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),                           // at (0,0,0)
    logicWorld,                                // its logical volume
    "World",                                   // its name
    nullptr,                                   // its mother  volume
    false,                                     // no boolean operation
    0,                                         // copy number
    checkOverlaps);                            // overlaps checking
// %%%%%%%%%%%%%%%%%%%%%%%%%%


    //
    // AMG6
    //
  G4double pi = CLHEP::pi;

/*
  G4Tubs *solidM0disk = new G4Tubs("SolM0disc", 0, M0discradius,M0discwidth,0,2*pi);
  G4LogicalVolume *M0discLog = new G4LogicalVolume(solidM0disk,  // its solid
      copperM0,// its material
      "logicM0disc");


  G4VPhysicalVolume *phydisk = new G4PVPlacement(0,                    //no rotation
       G4ThreeVector(0, 0, 0),
       M0discLog,                //its logical volume
       "M0discPhys",                     //its name
       logicWorld,                     //its mother  volume
       false,                        //no boolean operation
       0,                            //copy number
       checkOverlaps);                      //overlaps checking
*/


// spirals
/*
    auto spiral1 = new ArchimedSpiral("firstspiral",30,6.5*mm,52*cm, 4.5*pi,
    0,3*mm,pi/2,pi);

    G4MultiUnion* sp1 = spiral1->GetSpiral();
    G4LogicalVolume *lsp1 =  new G4LogicalVolume(sp1,  // its solid
      water_mat,                                       // its material
      "Spiral1");

    G4RotationMatrix* rm1 = new G4RotationMatrix();
    rm1->rotateY(90.*deg);

    G4VPhysicalVolume *physp1 = new G4PVPlacement(rm1,                    //no rotation
           G4ThreeVector(0, 0, 40*mm),
           lsp1,                //its logical volume
           "M0discPhys",                     //its name
           logicWorld,                     //its mother  volume
           false,                        //no boolean operation
           0,                            //copy number
           false);                      //overlaps checking

    G4RotationMatrix* rm2 = new G4RotationMatrix();
    rm2->rotateY(90.*deg);
    rm2->rotateX(90.*deg);

   G4VPhysicalVolume *physp2 = new G4PVPlacement(rm2,                    //no rotation
          G4ThreeVector(0, 0, 40*mm),
          lsp1,                //its logical volume
          "M0discPhys",                     //its name
          logicWorld,                     //its mother  volume
          false,                        //no boolean operation
          0,                            //copy number
          false);

   G4RotationMatrix* rm3 = new G4RotationMatrix();
   rm3->rotateY(90.*deg);
   rm3->rotateX(180.*deg);

   G4VPhysicalVolume *physp3 = new G4PVPlacement(rm3,                    //no rotation
          G4ThreeVector(0, 0, 40*mm),
          lsp1,                //its logical volume
          "M0discPhys",                     //its name
          logicWorld,                     //its mother  volume
          false,                        //no boolean operation
          0,                            //copy number
          false);

//

    G4RotationMatrix* rm4 = new G4RotationMatrix();
    rm4->rotateY(90.*deg);
    rm4->rotateX(270.*deg);

    G4VPhysicalVolume *physp4 = new G4PVPlacement(rm4,                    //no rotation
           G4ThreeVector(0, 0, 40*mm),
           lsp1,                //its logical volume
           "M0discPhys",                     //its name
           logicWorld,                     //its mother  volume
           false,                        //no boolean operation
           0,                            //copy number
           false);
*/
//  ======= BackPlate placement ================================================
auto pBackPlate = new BackPlate ("BackPlate", 80*mm, logicWorld );
  //
  //always return the physical World
  //
  return physWorld;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
