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
/// \file B1/include/RunAction.hh
/// \brief Definition of the B1::RunAction class

#ifndef MyRunAction_HH
#define MyRunAction_HH 1

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "DetectorConstruction.hh"

#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "VectorAccumulable.hh"
#include "G4Accumulable.hh"
#include "Run.hh"

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    ~RunAction();

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

    G4Run* GenerateRun() override;


    const G4double MinZ = 0.*CLHEP::cm;
    const G4double MaxZ = 100*CLHEP::cm;

    const G4double stepfordEdz = 1.* CLHEP::mm;
    const G4double stepforfluence = 1.* CLHEP::mm;
    VectorAccumulable<G4double> fEnergyCube, fDoseCube;
    void AddDoseCube(std::vector<G4double>);
    void AddEdepCube(std::vector<G4double>);
    void AddNeutrons(G4int);
    G4String fOutputfile;
    G4Accumulable<G4int> fNumberOfNeutrons = 0.;

private:
    Run* fRun = nullptr;

};

#endif
