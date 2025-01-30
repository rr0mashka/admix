#ifndef Run_hhh
#define Run_hhh 1

#include "G4Run.hh"

#include "G4StatAnalysis.hh"
#include "globals.hh"


/// Run class
///
/// In RecordEvent() there is collected information event per event
/// from Hits Collections, and accumulated statistic for the run

class Run : public G4Run
{
  public:
    Run() = default;
    ~Run() override = default;

    void RecordEvent(const G4Event*) override;
    void Merge(const G4Run*) override;
    void EndOfRun();
    void SetFileName(G4String);

  public:
  //  G4int GetNbGoodEvents() const { return fGoodEvents; }
  //  G4double GetSumDose() const { return fSumDose; }
  //  G4StatAnalysis GetStatDose() const { return fStatDose; }

  private:
    std::map<G4String, G4double> fEdepMap;
    std::map<G4String, G4double> fEdepBoronMap;
    G4String fFileName = " ";
};

#endif
