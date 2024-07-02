#ifndef B1ArchimedSpiral_h
#define B1ArchimedSpiral_h 1

#include "globals.hh"
#include "G4TwoVector.hh"

class G4MultiUnion;
//class G4TwoVector;

class ArchimedSpiral  {
public:
// cent -- the center of the spiral begining positin; Napprox -- number of approximation points, Winding -- phi of ending point in polar coordinates
  ArchimedSpiral(G4String, G4int, G4double, G4double, G4double,
  G4double, G4double, G4double, G4double);

  ~ArchimedSpiral();

  inline G4MultiUnion* GetSpiral() const {
    return fSpiral;
  };

private:
  G4double GetAlpha();
  void ConstructSolids();
  G4String fName;
  G4double fDeltaWinding;
  G4double fPhiBeg, fPhiEnd;
  G4double fEdgeBeg, fEdgeEnd;
  G4double fRMin, fRMax;
  G4int fNapp;
  G4double fAlpha;
  std::vector<G4TwoVector> fspiralpoints;
  G4MultiUnion *fSpiral;
};

#endif
