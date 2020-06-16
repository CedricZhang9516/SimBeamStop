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
#include "globals.hh"
#include <array>
#include <map>
#include <vector>

#include "G4ThreeVector.hh"
class G4Step;

struct TreeVars {
  G4int trkId;
  G4int trkIdMtr;
  G4int trkStatus;
  G4int trkPdgId;
  G4double trkProperTime;
  G4ThreeVector trkPolVct;
  G4double trkGlobalTime;
  G4double trkLocalTime;
  G4double trkLength;
  G4double trkVelocity;
  G4double trkTotE;
  G4double trkKinE;
  G4ThreeVector trkPos;
  G4ThreeVector trkMomVct;
  G4ThreeVector trkMomDir;
  G4ThreeVector trkProdVtx;
  G4ThreeVector trkMomDirPV;
  G4double trkKinEPV;
  G4int trkVolIdPV;
  G4int trkStepNum;
  G4double trkStepLen;
  G4int trkPreVolId;
  G4int trkPostVolId;

  G4double stpTotE;
  G4double stpNonIoniE;
  G4double stpLength;
  G4double stpDeltaT;
  G4ThreeVector stpDeltaPos;

  G4int preStatus;
  G4double preMass;
  G4double preCharge;
  G4double preProperTime;
  G4double preMagneticMom;
  G4ThreeVector prePolVct;
  G4double preGlobalTime;
  G4double preLocalTime;
  G4double preVelocity;
  G4double preBeta;
  G4double preGamma;
  G4double preTotE;
  G4double preKinE;
  G4ThreeVector preWorldPos;
  G4ThreeVector preLocalPos;
  G4ThreeVector preMomVct;
  G4ThreeVector preMomDir;
  G4int preProcId;
  G4int preProcType;
  G4int preProcSubType;
  G4int preVolId;
  G4int preVolDepth;
  G4int preVolCopyNum;

  G4int postStatus;
  G4double postMass;
  G4double postCharge;
  G4double postProperTime;
  G4double postMagneticMom;
  G4ThreeVector postPolVct;
  G4double postGlobalTime;
  G4double postLocalTime;
  G4double postVelocity;
  G4double postBeta;
  G4double postGamma;
  G4double postTotE;
  G4double postKinE;
  G4ThreeVector postWorldPos;
  G4ThreeVector postLocalPos;
  G4ThreeVector postMomVct;
  G4ThreeVector postMomDir;
  G4int postProcId;
  G4int postProcType;
  G4int postProcSubType;
  G4int postVolId;
  G4int postVolDepth;
  G4int postVolCopyNum;
};

class P00RunData : public G4Run
{
public:
           P00RunData();
  virtual ~P00RunData();
  void Add(G4int volID, G4double de,
	   G4double px, G4double py, G4double pz, G4double kinEnergy);
  void FillPerEvent();
  void Reset();


  G4int GetProcID( const G4String& procName );
  void StoreVars( const G4Step* aStep, TreeVars& tv );
  void DummyVars( TreeVars& tv );
  void FillTree( const G4int& tid, const TreeVars& tv );
  
  // Volume that Steppingaction should look into
  G4String  GetVolumeName(G4int volID) const;
  // Energy deposite inside the volumn (aerogel sample)
  G4double  GetEdep(G4int volID) const;        
  // x coordinate of mu+ stopped point inside aerogel
  G4double  GetXPosition(G4int volID) const;   
  // y coordinate of mu+ stopped point inside aerogel
  G4double  GetYPosition(G4int volID) const;   
  // z coordinate of mu+ stopped point inside aerogel
  G4double  GetZPosition(G4int volID) const;   
  G4double  GetKinEnergy(G4int volID) const;

  G4bool hitTarget;
  TreeVars IniTV;
  TreeVars EntTV;
  TreeVars EndTV;

private:

  // Define a 1-dimension "G4String"-type array named "fVolumeName",
  // ARRAY store Name for different volumn.
  // eg. fVolumeNames[volID =dTargetID =0] ="anyNameUlike"
  std::array<G4String,1> fVolumeNames;
  std::array<G4double,1> fEdep; 
  std::array<G4double,1> fXPosition;
  std::array<G4double,1> fYPosition; 
  std::array<G4double,1> fZPosition;
  std::array<G4double,1> fKinEnergy;

};

const G4int dTargetID = 0; // id of Target volumn.
// In case there is other detector volumn in the future,
// they will be assign id=1, 2, etc.

// inline functions that will be used in SteppingAction.cc
// REALLY NEEDED INLINE??????
inline void
P00RunData::Add (G4int volID, G4double de,
		 G4double px, G4double py, G4double pz, G4double kinEnergy)
{  
  fEdep[volID] = de;
  fXPosition[volID] = px;
  fYPosition[volID] = py;
  fZPosition[volID] = pz;
  fKinEnergy[volID] = kinEnergy;
}

inline G4String  P00RunData::GetVolumeName(G4int volID) const {
  return fVolumeNames[volID];
}
inline G4double  P00RunData::GetEdep(G4int volID) const {
  return fEdep[volID];
}
inline G4double  P00RunData::GetXPosition(G4int volID) const {
  return fXPosition[volID];
}   
inline G4double  P00RunData::GetYPosition(G4int volID) const {
  return fYPosition[volID];
}
inline G4double  P00RunData::GetZPosition(G4int volID) const{
  return fZPosition[volID];
}
inline G4double  P00RunData::GetKinEnergy(G4int volID) const{
  return fKinEnergy[volID];
}

#endif


