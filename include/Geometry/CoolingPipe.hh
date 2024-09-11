#ifndef CoolingPipe_h
#define CoolingPipe_h 1

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


class CoolingPipe  {
public:
  CoolingPipe(G4String, G4double, G4double, G4double, G4LogicalVolume*, G4double);
  ~CoolingPipe();

  G4double Pipe_InnerDiamter = 25.4 * mm;
  G4double WallThickness     =  2.0 * mm;
  G4double PipeLength        =  100 * mm;

  G4double  FittingPipeWallThickness = 2.5*mm;
  G4double FittingPipeLength        =  30 * mm;

};

#endif
