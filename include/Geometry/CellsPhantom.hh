#ifndef CellsPhantom_h
#define CellsPhantom_h 1

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


class CellsPhantom  {
public:
  CellsPhantom(G4String, G4double, G4LogicalVolume*, G4double);
  ~CellsPhantom();

G4double PhantomDisk_Diamter = 220.0 * mm;
G4double PhantomDisk_Width = 50 * mm;

};

#endif
