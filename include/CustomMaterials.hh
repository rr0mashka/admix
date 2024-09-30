#ifndef CustomMaterials_H
#define CustomMaterials_H 1

#include "globals.hh"
class G4Material;



class CustomMaterials
{
public:
  CustomMaterials();
  ~ CustomMaterials();

public:
  G4Material* matH2O;
  G4Material* soft;
  G4Material* skeleton;
  G4Material* lung;
  G4Material* adipose;
  G4Material* glandular;
  G4Material* adipose_glandular;
  void  DefineMaterials();
  G4Material* GetMaterial(G4String); //returns the material

private:

  //G4Material* Titanium;
  //G4Material*Vacuum;

};
#endif
