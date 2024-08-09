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
#include "Geometry/FrontPlate.hh"
#include "Geometry/NeutronModerator.hh"
#include "Geometry/CubicPhantom.hh"
//Geometry


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
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material* copperM0 = nist->FindOrBuildMaterial("G4_Cu");
  G4Material* LiMat = nist->FindOrBuildMaterial("G4_Li");


// %%%%% scales  %%%%%%

  const G4double world_sizeXY = 80*cm;
  const G4double world_sizeZ  = 80*cm;

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

//  =======  placements of various parts ================================================
auto pFrontPlate = new FrontPlate ("FrontPlate",6.75*mm, logicWorld );
auto pBackPlate = new BackPlate ("BackPlate", 20*mm, logicWorld );
auto pNeutronModerator = new NeutronModerator ("GlassDisk1", 30*mm, logicWorld );

// will change later, manually for now

zpos_phantom = 200*mm;
auto pCubicPhantom = new CubicPhantom ("CubicPhantom", zpos_phantom, logicWorld );
fScoringVolumes = pCubicPhantom->GetScoringCubes();
vPos_X = pCubicPhantom->vPos_X;
vPos_Y = pCubicPhantom->vPos_Y;
vPos_Z = pCubicPhantom->vPos_Z;

  //always return the physical World
  //
  return physWorld;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
