#ifndef NeutronModerator_h
#define NeutronModerator_h 1

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


class NeutronModerator  {
public:
  NeutronModerator(G4String, G4double, G4LogicalVolume*, G4double);
  ~NeutronModerator();

  G4double GlassDisk_Diamter = 200.0 * mm;//200
  G4double GlassDisk_Width =     72 * mm;//72

};

#endif
