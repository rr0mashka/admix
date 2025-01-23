#ifndef SphericalPhantom_h
#define SphericalPhantom_h 1

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "DetectorConstruction.hh"
#include <vector>

class SphericalPhantom {
public:
    SphericalPhantom(G4String name, G4double z_pos, G4LogicalVolume* mother_volume_log, G4double maxStep,  G4double tissueradius, G4double bonelayer, G4double skinlayer);
    ~SphericalPhantom();

    G4double ScoringCell_sizeXY = 2.0 * mm;   // Размер кубического вокселя в XY
    G4double ScoringCell_sizeZ = 2.0 * mm;    // Размер кубического вокселя в Z

    // Добавление векторов для координат
    std::vector<G4double> vPos_X;
    std::vector<G4double> vPos_Y;
    std::vector<G4double> vPos_Z;

    std::vector<G4VPhysicalVolume*> GetScoringCubes() const {
        return fScoringCubes;
    };

    void SetDetector(DetectorConstruction*);

private:
    std::vector<G4VPhysicalVolume*> fScoringCubes;
    DetectorConstruction* fdet = nullptr;
};

#endif
