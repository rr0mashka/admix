#ifndef B1GenCont_h
#define B1GenCont_h 1

#include "globals.hh"
#include "G4TwoVector.hh"

class G4MultiUnion;

class GeneralTubsConcatenation  {
public:
// cent -- the center of the spiral begining positin; Napprox -- number of approximation points, Winding -- phi of ending point in polar coordinates
  GeneralTubsConcatenation(G4String, std::vector<G4TwoVector>, G4TwoVector,
  G4double, G4double, G4double, G4double);

  ~GeneralTubsConcatenation();

  inline G4MultiUnion* GetSolid() const {
    return fSolid;
  };

private:
  void ConstructSolids();
  G4String fName;
  G4double fPhiBeg, fPhiEnd;
  G4double fRMin, fRMax;
  G4TwoVector fFirstNormal;
  std::vector<G4TwoVector> fSolidPoints;
  G4MultiUnion *fSolid;
};

#endif
