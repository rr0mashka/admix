#ifndef AcceleratorPipe_h
#define AcceleratorPipe_h 1

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


class AcceleratorPipe  {
public:
    AcceleratorPipe(G4String, G4double, G4LogicalVolume*, G4double);
  ~AcceleratorPipe();

  G4double AcceleratorPipe_Diamter = 169 * mm;//169
  G4double AcceleratorPipe_Width =     16 * mm;//16

};

#endif
