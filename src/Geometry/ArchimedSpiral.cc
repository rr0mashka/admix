#include "Geometry/ArchimedSpiral.hh"
#include "G4VSolid.hh"
#include "G4ThreeVector.hh"
#include "G4MultiUnion.hh"
#include "G4Transform3D.hh"
#include "G4CutTubs.hh"

ArchimedSpiral::ArchimedSpiral(G4String name, G4int Napprox, G4double edgebeg, G4double edgeend, G4double deltawinding,
   // profile
   G4double pradmin, G4double pradmax, G4double phibeg, G4double phiend){
// initializations
   if (Napprox < 1) {
    std::cout << "Napprox should be >=1; set to 1" << std::endl;
    Napprox = 1;
   };
   if (edgebeg>edgeend){
     std::cout << "edgebeg should be < than edgeend" << std::endl;
     return;
   }
   fName = name; fNapp = Napprox; fEdgeBeg = edgebeg;  fEdgeEnd = edgeend; fDeltaWinding = deltawinding;
// profile
   fRMin = pradmin; fRMax = pradmax;
   fPhiBeg = phibeg; fPhiEnd = phiend;

   fAlpha = GetAlpha();
   fSpiral = new G4MultiUnion(name);
   fspiralpoints = {};
   G4ThreeVector a;
   G4double ph, r, x, y;
   G4double windingbeg = fEdgeBeg/fAlpha;
   G4double phistep = deltawinding/fNapp;
   for (int i = 0; i<fNapp+1;i++){
      ph = windingbeg + phistep*i;
      r = ph*fAlpha;
      y = r*sin(ph);
      x = r* cos(ph);
      fspiralpoints.push_back(G4TwoVector(y,x));
   };
   fSpiral = new G4MultiUnion(fName);
   ConstructSolids();

};


void ArchimedSpiral::ConstructSolids(){
// find normals
  G4String name;
  G4double phi, deltaphinext, deltaphi;
  auto deltar = G4TwoVector();
  G4CutTubs* tub;
  G4TwoVector deltarnext;
  if (fspiralpoints.size()<=2) {
    std::cout << "here!!!!!!!!!!!!!!!1" << std::endl;
    name = std::to_string(1);
    deltar = (fspiralpoints.at(1) - fspiralpoints.at(0));
    std::cout << deltar.mag() << std::endl;
    tub = new G4CutTubs(name,fRMin,fRMax,deltar.mag(),fPhiBeg,fPhiEnd,G4ThreeVector(0,0,-1),G4ThreeVector(0,0,1));
    fSpiral->AddNode(*tub,G4Transform3D());
    delete tub;
    fSpiral -> Voxelize();
    return;
  }
  phi = 0;
  deltaphi = 0;
  G4ThreeVector axis = G4ThreeVector(1,0,0);
  G4Transform3D tr =  G4Rotate3D(0, axis);
  G4Transform3D trrotate =  G4Rotate3D(0, axis);
  G4Transform3D trtranslate =  G4Translate3D(0,0,0);
  for (int i=0; i<fspiralpoints.size()-2;i++){
    name = std::to_string(i);
    deltar = fspiralpoints.at(i+1) - fspiralpoints.at(i);
    deltarnext = fspiralpoints.at(i+2) - fspiralpoints.at(i+1);
    deltaphinext = acos(deltar*deltarnext/(deltar.mag()*deltarnext.mag()));
    std::cout << deltaphinext/CLHEP::deg << std::endl;
    //tubs.push_back(new G4CutTubs(name,fRMin,fRMax,fPhiBeg,fPhiEnd,deltar.mag(),G4ThreeVector(0,0,-1),G4TreeVector(0,sin(deltaphi),cos(deltaphi))));
    tub = new G4CutTubs(name,fRMin,fRMax,deltar.mag()/2,fPhiBeg,fPhiEnd,G4ThreeVector(0,sin(deltaphi/2),-cos(deltaphi/2)),G4ThreeVector(0,sin(deltaphinext/2),cos(deltaphinext/2)));
    fSpiral->AddNode(*tub,tr);
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
  fSpiral -> Voxelize();
}

G4double ArchimedSpiral::GetAlpha(){
 return (fEdgeEnd - fEdgeBeg)/fDeltaWinding;
}
