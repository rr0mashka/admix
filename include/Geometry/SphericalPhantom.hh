#ifndef SphericalPhantom_h
#define SphericalPhantom_h 1

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include <vector>

class SphericalPhantom {
public:
    SphericalPhantom(G4String name, G4double z_pos, G4LogicalVolume* mother_volume_log, G4double maxStep);
    ~SphericalPhantom();

    G4double PhantomSize_sizeXY = 100.0 * mm; // Размер фантома по XY
    G4double PhantomSize_sizeZ = 100.0 * mm;  // Размер фантома по Z
    G4double ScoringCell_sizeXY = 5.0 * mm;   // Размер кубического вокселя в XY
    G4double ScoringCell_sizeZ = 5.0 * mm;    // Размер кубического вокселя в Z

    // Добавление векторов для координат
    std::vector<G4double> vPos_X;
    std::vector<G4double> vPos_Y;
    std::vector<G4double> vPos_Z;

    std::vector<G4VPhysicalVolume*> GetScoringCubes() const {
        return fScoringCubes;
    };

private:
    std::vector<G4VPhysicalVolume*> fScoringCubes;
};

#endif
