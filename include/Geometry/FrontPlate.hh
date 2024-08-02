#ifndef FrontPlatee_h
#define FrontPlatee_h 1

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4CutTubs.hh"
#include "G4Torus.hh"

class FrontPlate  {
public:

  FrontPlate(G4String, G4double, G4LogicalVolume*);
  ~FrontPlate();

  G4double FrontPlateDisc_Radius = (143.0 / 2.0) * mm;
  G4double FrontPlateDisc_Width = (10. / 2.0) * mm;

  G4double FrontPlateSpiralDrain_InnerR = 4.7 * mm;
  G4double FrontPlateSpiralDrain_OuterR = 57 * mm;
  G4double FrontPlateSpiralDrain_Depth = 3 * mm;
  G4double FrontPlateSpiralWinding = 3.5*CLHEP::pi;
  G4int FrontPlateSpiral_Niter = 50;

  G4double LitiumDisc_Radius = (82.0 / 2.0) * mm;
  G4double LitiumDisc_Width = (60. / 2.0) * CLHEP::micrometer;



private:
};




#endif
