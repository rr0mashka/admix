//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1/include/DetectorConstruction.hh
/// \brief Definition of the B1::DetectorConstruction class

#ifndef B1DetectorConstruction_h
#define B1DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
//#include "Geometry/SphericalPhantom.hh"
#include "Geometry/CellsPhantom.hh"
#include <vector>
#include "Messengers/DetectorMessenger.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4VSensitiveDetector;

/// Detector construction class to define materials and geometry.


class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction();

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

    std::vector<G4VPhysicalVolume*> GetScoringVolumes() const { return fScoringVolumes; }
    G4String GetWorldLogName() const{
      if (!logicWorld) return "";
      return logicWorld->GetName();
    }
    std::vector<G4double> vPos_X;
    std::vector<G4double> vPos_Y;
    std::vector<G4double> vPos_Z;
    G4double zpos_phantom;
    void SetSensMotherPhys(G4String log, G4String phys){
      fMotherPhys[log] = phys;
    }
    void SetBoronConcentration(G4double);
    void SetLithiumGammaFlag(G4bool);

    G4double GetBoronConcentration() const{
      return fBoronConcentration;
    };

    G4double GetLithiumGammaFlag() const{
      return fLithiumGammaFlag;
    };

    G4String GetSensMotherPhys(G4String log) const{
      std::map<G4String, G4String>::const_iterator it = fMotherPhys.find(log);
      if (it!=fMotherPhys.end()) return it->second;
      return "";
    };

    std::vector<G4LogicalVolume*> GetPhantomLVs() const {
        if (!pCellsPhantom) return {};
        return pCellsPhantom->GetSLVs();
    };
    G4int GetNPhantoms() const {
      return vPos_X.size();
    }

    std::map<G4String, G4double> fEdepMap;
  protected:
    std::vector<G4VPhysicalVolume*> fScoringVolumes = {};

  private:
    CellsPhantom* pCellsPhantom = nullptr;
    G4LogicalVolume* logicWorld = nullptr;
    std::map<G4String, G4String> fMotherPhys;
    G4double fBoronConcentration;
    G4bool fLithiumGammaFlag;
    DetectorMessenger *fDetectorMessenger;
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
