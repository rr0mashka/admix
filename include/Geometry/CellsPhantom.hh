#ifndef CellsPhantom_h
#define CellsPhantom_h 1

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

class DetectorConstruction;

class CellsPhantom  {
public:
  CellsPhantom(G4String, G4double, G4LogicalVolume*, G4double);
  ~CellsPhantom();

G4double PhantomDisk_Diamter = 220.0 * mm;
G4double PhantomDisk_Width = 50 * mm;

G4double ScoringCell_radius = 8.0 * mm;   // Размер кубического вокселя в XY
G4double ScoringCell_sizeZ = 10 * mm;    // Размер кубического вокселя в Z

// Добавление векторов для координат
std::vector<G4double> vPos_X;
std::vector<G4double> vPos_Y;
std::vector<G4double> vPos_Z;

std::vector<G4VPhysicalVolume*> GetScoringCubes() const {
    return fScoringCubes;
};
std::vector<G4LogicalVolume*> GetSLVs() const {
    return fLogvolumes;
};


private:
std::vector<G4VPhysicalVolume*> fScoringCubes;
DetectorConstruction* fdet = nullptr;
G4LogicalVolume* template_Cells_Log;
std::vector<G4LogicalVolume*> fLogvolumes = {};
};


#endif
