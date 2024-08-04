#ifndef CubicPhantom_h
#define CubicPhantom_h 1

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


class CubicPhantom  {
public:
  CubicPhantom(G4String, G4double, G4LogicalVolume*);
  ~CubicPhantom();

  G4double PhantomSize_sizeXY = 100.0 * mm;//200
  G4double PhantomSize_sizeZ = 50.0  * mm;//200

  G4double ScoringCell_sizeXY = 10.0 * mm;//200
  G4double ScoringCell_sizeZ = 10.0  * mm;//200

};

#endif
