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
#include "G4Torus.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4UserLimits.hh"


#include "globals.hh"
#include "G4TwoVector.hh"


class BackPlate  {
public:

  BackPlate(G4String, G4double, G4LogicalVolume*, G4double);
  ~BackPlate();

  G4double BackPlate_Radius = (143.0 / 2.0) * mm;
  G4double BackPlate_Width = (16.5 / 2.0) * mm;

  G4double BackPlateOD_InnerRadius = (143.0 / 2.0) * mm;
  G4double BackPlateOD_OuterRadius = (175.0 / 2.0) * mm;
  G4double BackPlateOD_Width = (19.5 / 2.0) * mm;

  G4double Groove_InnerRadius = (106.0 / 2.0) * mm;
  G4double Groove_OuterRadius = (126.0 / 2.0) * mm;
  G4double Groove_Width = (10.0 / 2.0) * mm;

private:

};

#endif
