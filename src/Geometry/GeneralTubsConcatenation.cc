#include "Geometry/GeneralTubsConcatenation.hh"
#include "G4VSolid.hh"
#include "G4ThreeVector.hh"
#include "G4MultiUnion.hh"
#include "G4Transform3D.hh"
#include "G4CutTubs.hh"

GeneralTubsConcatenation::GeneralTubsConcatenation(G4String name, std::vector<G4TwoVector> edgecenters, G4TwoVector firstnormal,
   // profile
   G4double pradmin, G4double pradmax, G4double phibeg, G4double phiend){
   fName = name;
// profile
   fRMin = pradmin; fRMax = pradmax;
   fPhiBeg = phibeg; fPhiEnd = phiend;

   fSolid = new G4MultiUnion(name);
   fSolidPoints = edgecenters;
   fFirstNormal = firstnormal;
   ConstructSolids();

};


void GeneralTubsConcatenation::ConstructSolids(){
// find normals
  G4String name;
  G4double phi, deltaphinext, deltaphi;
  auto deltar = G4TwoVector();
  G4CutTubs* tub;
  G4TwoVector deltarnext;
  if (fSolidPoints.size()<=2) {
    std::cout << "here!!!!!!!!!!!!!!!1" << std::endl;
    name = std::to_string(1);
    deltar = (fSolidPoints.at(1) - fSolidPoints.at(0));
    std::cout << deltar.mag() << std::endl;
    tub = new G4CutTubs(name,fRMin,fRMax,deltar.mag(),fPhiBeg,fPhiEnd,G4ThreeVector(0,0,-1),G4ThreeVector(0,0,1));
    fSolid->AddNode(*tub,G4Transform3D());
    delete tub;
    fSolid -> Voxelize();
    return;
  }
  phi = 0;
  deltaphi = 2*asin(fFirstNormal.x()/fFirstNormal.mag());
  G4ThreeVector axis = G4ThreeVector(1,0,0);
  G4Transform3D tr =  G4Rotate3D(0, axis);
  G4Transform3D trrotate =  G4Rotate3D(0, axis);
  G4Transform3D trtranslate =  G4Translate3D(0,0,0);
  for (int i=0; i<fSolidPoints.size()-2;i++){
    name = std::to_string(i);
    deltar = fSolidPoints.at(i+1) - fSolidPoints.at(i);
    deltarnext = fSolidPoints.at(i+2) - fSolidPoints.at(i+1);
    deltaphinext = acos(deltar*deltarnext/(deltar.mag()*deltarnext.mag()));
    //tubs.push_back(new G4CutTubs(name,fRMin,fRMax,fPhiBeg,fPhiEnd,deltar.mag(),G4ThreeVector(0,0,-1),G4TreeVector(0,sin(deltaphi),cos(deltaphi))));
    tub = new G4CutTubs(name,fRMin,fRMax,deltar.mag()/2,fPhiBeg,fPhiEnd,G4ThreeVector(0,sin(deltaphi/2),-cos(deltaphi/2)),G4ThreeVector(0,sin(deltaphinext/2),cos(deltaphinext/2)));
    fSolid->AddNode(*tub,tr);
    trtranslate = G4Translate3D(0,deltar.mag()*sin(phi)/2 + deltarnext.mag()*sin(phi+deltaphinext)/2,deltar.mag()*cos(phi)/2+deltarnext.mag()*cos(phi+deltaphinext)/2)*trtranslate;
    trrotate =  G4Rotate3D(-deltaphinext, axis)*trrotate;
    tr = trtranslate*trrotate;
    deltaphi = deltaphinext;
    phi += deltaphinext;
  };
  //name = std::to_string(fspiralpoints.size()-1);
  //tub = new G4CutTubs(name,fRMin,fRMax,deltarnext.mag()/2,fPhiBeg,fPhiEnd,G4ThreeVector(0,sin(deltaphi/2),-cos(deltaphi/2)),G4ThreeVector(0,0,1));
  //fSpiral->AddNode(*tub,tr);
//  delete tub;

//test

/*  G4String name;
  G4Transform3D tr = G4Transform3D();
  G4ThreeVector axis = G4ThreeVector(1,0,0);
  name = std::to_string(0);
  auto deltar = G4TwoVector(0,10*CLHEP::cm);
  auto deltarnext = G4TwoVector(20*CLHEP::cm*sin(10*CLHEP::deg),20*CLHEP::cm*cos(10*CLHEP::deg));
  G4CutTubs* tub;
  tub = new G4CutTubs(name,fRMin,fRMax,deltar.mag()/2,fPhiBeg,fPhiEnd,G4ThreeVector(0,0,-1),G4ThreeVector(0,0,1));
  fSpiral->AddNode(*tub,tr);
  name = std::to_string(1);
  tub = new G4CutTubs(name,fRMin,fRMax,deltarnext.mag()/2,fPhiBeg,fPhiEnd,G4ThreeVector(0,0,-1),G4ThreeVector(0,0,1));
  tr = G4Translate3D(0,deltar.x()/2 + deltarnext.x()/2,deltar.y()/2+deltarnext.y()/2)*G4Rotate3D(-10*CLHEP::deg,axis)*tr;
  fSpiral->AddNode(*tub,tr);*/
  fSolid -> Voxelize();
}
