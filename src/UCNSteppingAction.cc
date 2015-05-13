#include "G4SystemOfUnits.hh"

#include "UCNSteppingAction.hh"
#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

UCNSteppingAction::UCNSteppingAction(int JOBNUM, std::string OUTPATH, int SECON, std::string NAME)
{
  secondaries=SECON;

  jobnumber=JOBNUM;
  std::string filesuffix = "track.out";
  std::string outpath = OUTPATH;
  std::string name = NAME;

  if (!trackfile.is_open()){
    std::ostringstream filename;
    filename << outpath << '/' << std::setw(12) << std::setfill('0') << jobnumber << name << filesuffix;
    std::cout << "Creating " << filename.str() << '\n';
    trackfile.open(filename.str().c_str());
    if (!trackfile.is_open()){
      std::cout << "Could not create" << filename.str() << '\n';
      exit(-1);
    }
    trackfile << "jobnumber particle polarisation "
      "t x y z vx vy vz "
      "H E Bx dBxdx dBxdy dBxdz By dBydx "
      "dBydy dBydz Bz dBzdx dBzdy dBzdz Babs dBdx dBdy dBdz Ex Ey Ez V\n";
    trackfile.precision(10);
  }


}

UCNSteppingAction::~UCNSteppingAction()
{;}

void UCNSteppingAction::UserSteppingAction(const G4Step * theStep)
{

  G4Track * theTrack = theStep->GetTrack();

  // check if it is alive
  if(theTrack->GetTrackStatus()!=fAlive) { return; }

  // check if it is primary
  if(theTrack->GetParentID()!=0&&!secondaries) { return; }

  particle=0;
  t = theTrack->GetGlobalTime()/s;
  x = (theTrack->GetPosition()/m)[0];
  y = (theTrack->GetPosition()/m)[1];
  z = (theTrack->GetPosition()/m)[2];
  vx = (theTrack->GetVelocity()/(m/s))*(theTrack->GetMomentumDirection())[0];
  vy = (theTrack->GetVelocity()/(m/s))*(theTrack->GetMomentumDirection())[1];
  vz = (theTrack->GetVelocity()/(m/s))*(theTrack->GetMomentumDirection())[2];
  if((theTrack->GetPolarization())[1]>0) polarisation=1;
  else                                   polarisation=-1;
  H = theTrack->GetTotalEnergy()/eV;
  E = theTrack->GetKineticEnergy()/eV;

  trackfile << jobnumber << " " << particle << " " << polarisation << " "
	    << t << " " << x << " " << y << " " << z << " " << vx << " "
	    << vy << " " << vz << " " << H << " " << E << " ";
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      trackfile << B[i][j] << " ";
  trackfile << Ei[0] << " " << Ei[1] << " " << Ei[2] << " " << V << '\n';


  /*
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
  */


}
