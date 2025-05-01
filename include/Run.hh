#ifndef Run_hhh
#define Run_hhh 1

#include "G4Run.hh"

#include "G4StatAnalysis.hh"
#include "globals.hh"

#include <chrono>
#include <ctime>

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

    static std::vector<std::string> splitString(std::string str, char splitter){
        std::vector<std::string> result;
        std::string current = "";
        for(int i = 0; i < str.size(); i++){
            if(str[i] == splitter){
                if(current != ""){
                    result.push_back(current);
                    current = "";
                }
                continue;
            }
            current += str[i];
        }
        if(current.size() != 0)
            result.push_back(current);
        return result;
    };

    static std::string GetCurrentDateTime(){
      time_t rawtime;
      struct tm * timeinfo;
      char buffer[80];
      time (&rawtime);
      timeinfo = localtime(&rawtime);
      strftime(buffer,sizeof(buffer),"%d%m%Y_%H%M%S",timeinfo);
      std::string str(buffer);
      return str;
    }

  //  G4int GetNbGoodEvents() const { return fGoodEvents; }
  //  G4double GetSumDose() const { return fSumDose; }
  //  G4StatAnalysis GetStatDose() const { return fStatDose; }

  private:
    std::map<G4String, G4double> fEdepMap;
    std::map<G4String, G4double> fEdepBoronMap;
    G4String fFileName = " ";
    G4String fWorldlogicalName;
};

#endif
