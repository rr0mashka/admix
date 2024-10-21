#ifndef MiceWoodDisk_h
#define MiceWoodDisk_h 1

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
#include "globals.hh"
#include "G4TwoVector.hh"


class MiceWoodDisk  {
public:
    MiceWoodDisk(G4String, G4double, G4LogicalVolume*, G4double);
  ~MiceWoodDisk();

G4double MiceWoodDisk_Diamter = 160.0 * mm;
G4double MiceWoodDisk_Width = 80 * mm;

};

#endif
