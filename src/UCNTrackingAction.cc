#include "UCNTrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"

void UCNTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  // Create trajectory only for primaries
  if(aTrack->GetParentID()==0)
  { fpTrackingManager->SetStoreTrajectory(true); }
  else
  { fpTrackingManager->SetStoreTrajectory(false); }
}


