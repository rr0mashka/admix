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
#include "G4StepLimiterPhysics.hh"
#include "G4UserLimits.hh"
#include <vector>


class CubicPhantom  {
public:
  CubicPhantom(G4String, G4double, G4LogicalVolume*,  G4double);
  ~CubicPhantom();

  G4double PhantomSize_sizeXY = 100.0 * mm;//200
  G4double PhantomSize_sizeZ = 10.0  * mm;//200

  G4double ScoringCell_sizeXY = 100 * mm;//200
  G4double ScoringCell_sizeZ = 1.0  * mm;//200
  std::vector<G4double> vPos_X;
  std::vector<G4double> vPos_Y;
  std::vector<G4double> vPos_Z;

  std::vector<G4VPhysicalVolume*> GetScoringCubes() const {
    return fScoringCubes;
  };

private:
  std::vector<G4VPhysicalVolume*> fScoringCubes = {};

};

#endif
