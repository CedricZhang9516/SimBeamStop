//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// P00RunData.hh
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// Run data class
///
/// It defines data members to hold the mu+ stopped position (x, y, z) and stopped time (t).
///
/// The data are collected step by step in P00SteppingAction, and
/// the accumulated values are filled in histograms and ntuple
/// event by event in P00EventAction.

#ifndef P00RunData_h
#define P00RunData_h 1

#include "G4Run.hh"
#include "P00Analysis.hh"
#include "globals.hh"
#include <array>
#include <map>
#include <vector>

#include "G4ThreeVector.hh"
class G4Event;
class G4Step;
//class G4Track;

//-- track/include/G4TrackStatus.hh
//  enum G4TrackStatus
//  {
//     fAlive,                   // =0: Continue the tracking
//     fStopButAlive,            // =1: Invoke active rest physics processes and kill the current track afterward
//     fStopAndKill,             // =2: Kill the current track
//     fKillTrackAndSecondaries, // =3: Kill the current track and associated secondaries.
//     fSuspend,                 // =4: Suspend the current track
//     fPostponeToNextEvent      // =5: Postpones the tracking of the current track to the next event.
//   };

//-- track/include/G4StepStatus.hh
//   enum G4StepStatus
//   {
//     fWorldBoundary,     // =0: Step reached the world boundary
//     fGeomBoundary,      // =1: Step defined by a geometry boundary
//     fAtRestDoItProc,    // =2: Step defined by a PreStepDoItVector
//     fAlongStepDoItProc, // =3: Step defined by a AlongStepDoItVector
//     fPostStepDoItProc,  // =4: Step defined by a PostStepDoItVector
//     fUserDefinedLimit,  // =5: Step defined by the user Step limit in the logical volume
//     fExclusivelyForcedProc, //=6: Step defined by an exclusively forced PostStepDoIt process
//     fUndefined          //=7: Step not defined yet
//   };

//-- processes/management/include/G4ProcessType.hh
//  enum G4ProcessType
//  { 
//    fNotDefined,
//    fTransportation,
//    fElectromagnetic,
//    fOptical, 
//    fHadronic,
//    fPhotolepton_hadron,
//    fDecay,
//    fGeneral,
//    fParameterisation,
//    fUserDefined,
//    fParallel,
//    fPhonon,
//    fUCN
//  };

struct StepInfo {
  G4int stpnum;
  G4int trkstatus;
  G4int proctype;
  G4String procname;
  void set( const G4int& sn, const G4int& ts, const G4int& pt,
	    const G4String& pn ){
    stpnum   = sn;
    trkstatus= ts;
    proctype = pt;
    procname = pn;
  }
  void reset(){
    stpnum   = -1;
    trkstatus= -1;
    proctype = -1;
    procname = "n/a";
  }
  void print(){
    G4cout << "stpnum= " << stpnum
	   << ", trkstatus= " << trkstatus
	   << ", proctype= " << proctype
	   << ", procname= " << procname
	   << G4endl;
  }
};

struct ParticleInfo {
  StepInfo first;
  StepInfo target;
  StepInfo last;
  void reset(){
    first.reset();
    target.reset();
    last.reset();
  }
};

struct DataHolder {
  G4int trkStat;
  G4int trkStpNum;
  G4double trkLen;

  G4double stpLen;
  G4double stpDelT;
  G4double stpDelTotE;
  G4double stpDelNiE;
  G4ThreeVector stpDelVtx;

  G4double glbtime;

  //-- Variables available from the pre/post-step-point.
  //   index= 0:pre, 1:post
  G4int pntStpStat[2];
  G4int pntPrcType[2];
  G4int pntPrcId[2];
  G4int pntVolId[2];
  G4double pntBeta[2];
  G4double pntGamma[2];

  //-- Variables available from the pre/post-step-point or track.
  //   index= 0:pre, 1:post, 2:track
  G4double pntPrpT[3];
  G4double pntLocT[3];
  G4double pntVelo[3];
  G4double pntKinE[3];
  G4ThreeVector pntVtx[3];
  G4ThreeVector pntMom[3];
};

class P00RunData : public G4Run
{
public:
           P00RunData();
  virtual ~P00RunData();
  
  G4int GetProcID( const G4String& procName );
  void SetBit( G4int& bitvar, const G4int& ith );
  
  void CreateAtRunAction( G4AnalysisManager* mgr );
  void CreateH1s( G4AnalysisManager* mgr );
  void CreateTps( G4AnalysisManager* mgr );

  void CreateTGen1( G4AnalysisManager* mgr );
  void CreateTEnd1( G4AnalysisManager* mgr );
  void CreateTGen2( G4AnalysisManager* mgr );
  void CreateTTgt2( G4AnalysisManager* mgr );
  void CreateTEnd( G4AnalysisManager* mgr );
  void CreateColumns( G4AnalysisManager* mgr, const G4String& prefix );

  void FillPerEvent( G4AnalysisManager* mgr );
  void FillH1s( G4AnalysisManager* mgr );
  void FillTps( G4AnalysisManager* mgr );

  void FillTGen1( G4AnalysisManager* mgr, const G4int& tid );
  void FillTEnd1( G4AnalysisManager* mgr, const G4int& tid );
  void FillTGen2( G4AnalysisManager* mgr, const G4int& tid );
  void FillTTgt2( G4AnalysisManager* mgr, const G4int& tid );
  void FillTEnd( G4AnalysisManager* mgr, const G4int& tid );
  void FillColumns( G4AnalysisManager* mgr, const G4int& tid,
		    G4int& cid, const DataHolder& hld, const G4int& index=0 );
  
  void SetAllVars( const G4Step* aStep, DataHolder& hld );
  void SetTrkVars( const G4Step* aStep, DataHolder& hld );
  void SetStpVars( const G4Step* aStep, DataHolder& hld );
  void SetPntVars( const G4Step* aStep, DataHolder& hld, const G4int& index=0 );

  void Reset();
  void ClearVars( DataHolder& hld );
  void PrintPerEvent( const G4Event* event );

  const G4int nbit_evt;
  const G4int nbit_hit;
  const G4int nbit_end;
  const G4int nbit_yld;
  G4int evtStats;
  G4int hitStats;
  G4int endStats;
  G4int yldStats;
  G4int evtLevel;
  
  ParticleInfo muInfo;
  DataHolder hldGen;
  DataHolder hldEnd;


private:

  /*
  // Define a 1-dimension "G4String"-type array named "fVolumeName",
  // ARRAY store Name for different volumn.
  // eg. fVolumeNames[volID =dTargetID =0] ="anyNameUlike"
  std::array<G4String,1> fVolumeNames;
  */
  
};

#endif


