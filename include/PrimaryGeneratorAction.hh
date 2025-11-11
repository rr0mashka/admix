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
/// \file PrimaryGeneratorAction.hh
/// \brief Definition of the B1::PrimaryGeneratorAction class

#ifndef B1PrimaryGeneratorAction_h
#define B1PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <numeric>

class G4Event;
class G4Box;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;
    void GeneratePrimaries(G4Event*) override;
    G4ParticleGun* GetParticleGun() { return fParticleGun; }

  private:
    G4ParticleGun* fParticleGun;
    G4Box* fEnvelopeBox = nullptr;
    struct MatrixSampler {
            int NE = 0, NTh = 0;
            double Emin = 0, Emax = 0, Thmin = 0, Thmax = 0;
            std::vector<std::vector<double>> I;
            std::vector<double> E, Th;

        bool loadFromFile(const std::string& filename) {
            std::ifstream file(filename);
            if (!file.is_open()) return false;
            
            file >> NE >> Emin >> Emax;
            file >> NTh >> Thmin >> Thmax;
            
            I.clear();
            E.clear();
            Th.clear();
            
            I.resize(NE, std::vector<double>(NTh));
            E.resize(NE);
            Th.resize(NTh);
            
            double dE  = (Emax - Emin) / NE;
            double dTh = (Thmax - Thmin) / NTh;
            
            for (int i = 0; i < NE; ++i)   E[i]  = (i + 0.5) * dE + Emin;
            for (int j = 0; j < NTh; ++j) Th[j] = (j + 0.5) * dTh + Thmin;
            
            for (int i = 0; i < NE; ++i){
                for (int j = 0; j < NTh; ++j){
                    file >> I[i][j];}}

                file.close();
                return true;
            }

            int chooseWeightedIndex(const std::vector<double>& w) const {
                double total = std::accumulate(w.begin(), w.end(), 0.0);
                double x = G4UniformRand() * total;
                double acc = 0.0;
                for (size_t i = 0; i < w.size(); ++i) {
                    acc += w[i];
                    if (x < acc) return static_cast<int>(i);
                }
                return static_cast<int>(w.size() - 1);
            }
        };

        MatrixSampler sampler;
};


#endif
