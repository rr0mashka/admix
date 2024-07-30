#ifndef BackPlate_h
#define BackPlate_h 1

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

class G4MultiUnion;
//class G4TwoVector;

class BackPlate  {
public:

  BackPlate(G4String, G4double, G4LogicalVolume*);
  ~BackPlate();

  G4double BackPlateRadius = (143.0 / 2.0) * mm;
  G4double BackPlateWidth = 16.5 * mm;

private:
  G4double GrooveWeight;
};

#endif
