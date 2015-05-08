#include "UCNSteppingAction.hh"
#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

UCNSteppingAction::UCNSteppingAction()
{;}

UCNSteppingAction::~UCNSteppingAction()
{;}

void UCNSteppingAction::UserSteppingAction(const G4Step * theStep)
{
  G4Track * theTrack = theStep->GetTrack();

  // check if it is alive
  if(theTrack->GetTrackStatus()!=fAlive) { return; }

  // check if it is primary
  if(theTrack->GetParentID()!=0) { return; }

  // check if it is NOT muon
  G4ParticleDefinition * particleType = theTrack->GetDefinition();
  if((particleType==G4MuonPlus::MuonPlusDefinition())
   ||(particleType==G4MuonMinus::MuonMinusDefinition()))
  { return; }

  // check if it is entering to the calorimeter volume
  G4StepPoint * thePrePoint = theStep->GetPreStepPoint();
  G4VPhysicalVolume * thePrePV = thePrePoint->GetPhysicalVolume();
  G4String thePrePVname = thePrePV->GetName();
  if(thePrePVname(0,4)=="calo") { return; }
  G4StepPoint * thePostPoint = theStep->GetPostStepPoint();
  G4VPhysicalVolume * thePostPV = thePostPoint->GetPhysicalVolume();
  G4String thePostPVname = thePostPV->GetName();
  if(thePostPVname(0,4)!="calo") { return; }

  // then suspend the track
  theTrack->SetTrackStatus(fSuspend);
}


