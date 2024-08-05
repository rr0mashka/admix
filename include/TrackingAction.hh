#ifndef MyTrackingAction_h
#define MyTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class G4VPhysicalVolume;


/// Tracking action class
///

class EventAction;
class RunAction;

class TrackingAction : public G4UserTrackingAction
{
public:
  TrackingAction(RunAction *runaction,EventAction* eventAction);
  ~TrackingAction() override;

  // method from the base class
  void PreUserTrackingAction(const G4Track*) override;
//  void PostUserTrackingAction(const G4Track*) override;

private:
  RunAction* fRunAction = nullptr;
  EventAction* fEventAction = nullptr;
};

#endif
