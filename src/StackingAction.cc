#include "StackingAction.hh"
#include "G4Track.hh"
#include "DetectorConstruction.hh"

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* track)
{
  // kill secondary neutrino
  const DetectorConstruction* detConstruction = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  if (detConstruction->GetLithiumGammaFlag()) return fUrgent;
  if (!track) return fUrgent;
  if (!track->GetVolume()) return fUrgent;
  if (track->GetVolume()->GetLogicalVolume()->GetName()!="LLitiumdisc") return fUrgent;
  if (track->GetDefinition()->GetParticleName() == "gamme"){
    return fKill;
  }
  else
    return fUrgent;
}
