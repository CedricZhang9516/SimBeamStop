//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// P00RunData.cc
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "P00RunData.hh"
#include "P00Analysis.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "Geometry.hh"

#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TouchableHandle.hh"
#include "G4VProcess.hh"
#include "G4VPhysicalVolume.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
P00RunData::P00RunData():G4Run(), fVolumeNames{{ "Target"}}
{
  for ( auto& edep : fEdep)            { edep = 0.; }
  for ( auto& xPosition : fXPosition ) { xPosition = 0.; }
  for ( auto& yPosition : fYPosition ) { yPosition = 0.; }
  for ( auto& zPosition : fZPosition ) { zPosition = 0.; }
  for ( auto& kinEnergy : fKinEnergy ) { kinEnergy = 0.; }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
P00RunData::~P00RunData()
{;}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

G4int P00RunData::GetProcID( const G4String& procName )
{
       if( procName == "Transportation" ) return 0;
  else if( procName == "muIoni"         ) return 1;
  else if( procName == "Decay"          ) return 2;
  else if( procName == "msc"            ) return 3;
       G4cout << "GetProcID: Unexpected process name: "
	      << procName << G4endl;
       return -1;
}

void P00RunData::StoreVars( const G4Step* aStep, TreeVars& tv )
{
  const G4Track*     aTrack = aStep->GetTrack();
  const G4StepPoint* prePnt = aStep->GetPreStepPoint();
  const G4StepPoint* pstPnt = aStep->GetPostStepPoint();
  const G4TouchableHandle& preTch = prePnt->GetTouchableHandle();
  const G4TouchableHandle& pstTch = prePnt->GetTouchableHandle();
  //  const G4Material*  preMat = prePnt->GetMaterial();
  //  const G4Material*  pstMat = pstPnt->GetMaterial();
  
  tv.trkId        = aTrack->GetTrackID();
  tv.trkIdMtr     = aTrack->GetParentID();
  tv.trkStatus    = (G4int)aTrack->GetTrackStatus();
  tv.trkPdgId     = aTrack->GetParticleDefinition()->GetPDGEncoding();
  tv.trkProperTime= aTrack->GetProperTime();
  tv.trkPolVct    = aTrack->GetPolarization();
  tv.trkGlobalTime= aTrack->GetGlobalTime();
  tv.trkLocalTime = aTrack->GetLocalTime();
  tv.trkLength    = aTrack->GetTrackLength();
  tv.trkVelocity  = aTrack->GetVelocity();
  tv.trkTotE      = aTrack->GetTotalEnergy();
  tv.trkKinE      = aTrack->GetKineticEnergy();
  tv.trkPos       = aTrack->GetPosition();
  tv.trkMomVct    = aTrack->GetMomentum();
  tv.trkMomDir    = aTrack->GetMomentumDirection();
  tv.trkProdVtx   = aTrack->GetVertexPosition();
  tv.trkMomDirPV  = aTrack->GetVertexMomentumDirection();
  tv.trkKinEPV    = aTrack->GetVertexKineticEnergy();
  tv.trkVolIdPV   = Geometry::volumeID[aTrack->GetLogicalVolumeAtVertex()->GetName()];
  tv.trkStepNum   = aTrack->GetCurrentStepNumber();
  tv.trkStepLen   = aTrack->GetStepLength();
  tv.trkPreVolId  = Geometry::volumeID[aTrack->GetVolume()->GetName()];
  tv.trkPostVolId = -2;
  G4VPhysicalVolume* nextVolume= aTrack->GetNextVolume();
  if( nextVolume ){
    tv.trkPostVolId = Geometry::volumeID[nextVolume->GetName()];
  }
  
  tv.stpTotE      = aStep->GetTotalEnergyDeposit();
  tv.stpNonIoniE  = aStep->GetNonIonizingEnergyDeposit();
  tv.stpLength    = aStep->GetStepLength();
  tv.stpDeltaT    = aStep->GetDeltaTime();
  tv.stpDeltaPos  = aStep->GetDeltaPosition();

  tv.preStatus     = (G4int)prePnt->GetStepStatus();
  tv.preMass       = prePnt->GetMass();
  tv.preCharge     = prePnt->GetCharge();
  tv.preProperTime = prePnt->GetProperTime();
  tv.preMagneticMom= prePnt->GetMagneticMoment();
  tv.prePolVct     = prePnt->GetPolarization();
  tv.preGlobalTime = prePnt->GetGlobalTime();
  tv.preLocalTime  = prePnt->GetLocalTime();
  tv.preVelocity   = prePnt->GetVelocity();
  tv.preBeta       = prePnt->GetBeta();
  tv.preGamma      = prePnt->GetGamma();
  tv.preTotE       = prePnt->GetTotalEnergy();
  tv.preKinE       = prePnt->GetKineticEnergy();
  tv.preWorldPos   = prePnt->GetPosition();
  tv.preLocalPos   = preTch->GetHistory()->GetTopTransform().TransformPoint(prePnt->GetPosition());
  tv.preMomVct     = prePnt->GetMomentum();
  tv.preMomDir     = prePnt->GetMomentumDirection();
  tv.preProcId     = -2;
  tv.preProcType   = -2;
  tv.preProcSubType= -2;
  const G4VProcess* preProcess = prePnt->GetProcessDefinedStep();
  if( preProcess ){
    tv.preProcId     = GetProcID(preProcess->GetProcessName());
    tv.preProcType   = (G4int)preProcess->GetProcessType();
    tv.preProcSubType= preProcess->GetProcessSubType();
  }
  tv.preVolId= -2;
  G4VPhysicalVolume* preVol= prePnt->GetPhysicalVolume();
  if( preVol ){
    tv.preVolId = Geometry::volumeID[preVol->GetName()];
  }
  tv.preVolDepth   = preTch->GetHistoryDepth();
  tv.preVolCopyNum = preTch->GetCopyNumber();

  tv.postStatus     = (G4int)pstPnt->GetStepStatus();
  tv.postMass       = pstPnt->GetMass();
  tv.postCharge     = pstPnt->GetCharge();
  tv.postProperTime = pstPnt->GetProperTime();
  tv.postMagneticMom= pstPnt->GetMagneticMoment();
  tv.postPolVct     = pstPnt->GetPolarization();
  tv.postGlobalTime = pstPnt->GetGlobalTime();
  tv.postLocalTime  = pstPnt->GetLocalTime();
  tv.postVelocity   = pstPnt->GetVelocity();
  tv.postBeta       = pstPnt->GetBeta();
  tv.postGamma      = pstPnt->GetGamma();
  tv.postTotE       = pstPnt->GetTotalEnergy();
  tv.postKinE       = pstPnt->GetKineticEnergy();
  tv.postWorldPos   = pstPnt->GetPosition();
  tv.postLocalPos   = pstTch->GetHistory()->GetTopTransform().TransformPoint(pstPnt->GetPosition());
  tv.postMomVct     = pstPnt->GetMomentum();
  tv.postMomDir     = pstPnt->GetMomentumDirection();
  tv.postProcId     = -2;
  tv.postProcType   = -2;
  tv.postProcSubType= -2;
  const G4VProcess* pstProcess = pstPnt->GetProcessDefinedStep();
  if( pstProcess ){
    tv.postProcId     = GetProcID(pstProcess->GetProcessName());
    tv.postProcType   = (G4int)pstProcess->GetProcessType();
    tv.postProcSubType= pstProcess->GetProcessSubType();
  }
  tv.postVolId= -2;
  G4VPhysicalVolume* pstVol= pstPnt->GetPhysicalVolume();
  if( pstVol ){
    tv.postVolId = Geometry::volumeID[pstVol->GetName()];
  }
  tv.postVolDepth   = pstTch->GetHistoryDepth();
  tv.postVolCopyNum = pstTch->GetCopyNumber();
}

void P00RunData::DummyVars( TreeVars& tv )
{
  G4ThreeVector zeroVct(0,0,0);
  
  tv.trkId        = -1;
  tv.trkIdMtr     = -1;
  tv.trkStatus    = -1;
  tv.trkPdgId     =  0;
  tv.trkProperTime=  0;
  tv.trkPolVct    = zeroVct;
  tv.trkGlobalTime=  0;
  tv.trkLocalTime =  0;
  tv.trkLength    =  0;
  tv.trkVelocity  =  0;
  tv.trkTotE      =  0;
  tv.trkKinE      =  0;
  tv.trkPos       = zeroVct;
  tv.trkMomVct    = zeroVct;
  tv.trkMomDir    = zeroVct;
  tv.trkProdVtx   = zeroVct;
  tv.trkMomDirPV  = zeroVct;
  tv.trkKinEPV    =  0;
  tv.trkVolIdPV   = -1;
  tv.trkStepNum   = -1;
  tv.trkStepLen   =  0;
  tv.trkPreVolId  = -1;
  tv.trkPostVolId = -2;
  
  tv.stpTotE      =  0;
  tv.stpNonIoniE  =  0;
  tv.stpLength    =  0;
  tv.stpDeltaT    =  0;
  tv.stpDeltaPos  =  zeroVct;

  tv.preStatus     = -1;
  tv.preMass       =  0;
  tv.preCharge     =  0;
  tv.preProperTime =  0;
  tv.preMagneticMom=  0;
  tv.prePolVct     = zeroVct;
  tv.preGlobalTime =  0;
  tv.preLocalTime  =  0;
  tv.preVelocity   =  0;
  tv.preBeta       =  0;
  tv.preGamma      =  0;
  tv.preTotE       =  0;
  tv.preKinE       =  0;
  tv.preWorldPos   = zeroVct;
  tv.preLocalPos   = zeroVct;
  tv.preMomVct     = zeroVct;
  tv.preMomDir     = zeroVct;
  tv.preProcId     = -2;
  tv.preProcType   = -2;
  tv.preProcSubType= -2;
  tv.preVolId      = -2;
  tv.preVolDepth   = -1;
  tv.preVolCopyNum = -1;     

  tv.postStatus     = -1;     
  tv.postMass       =  0;     
  tv.postCharge     =  0;     
  tv.postProperTime =  0;     
  tv.postMagneticMom=  0;     
  tv.postPolVct     = zeroVct;
  tv.postGlobalTime =  0;     
  tv.postLocalTime  =  0;     
  tv.postVelocity   =  0;     
  tv.postBeta       =  0;     
  tv.postGamma      =  0;     
  tv.postTotE       =  0;     
  tv.postKinE       =  0;     
  tv.postWorldPos   = zeroVct;
  tv.postLocalPos   = zeroVct;
  tv.postMomVct     = zeroVct;
  tv.postMomDir     = zeroVct;
  tv.postProcId     = -2;
  tv.postProcType   = -2;
  tv.postProcSubType= -2;
  tv.postVolId      = -2;
  tv.postVolDepth   = -1;     
  tv.postVolCopyNum = -1;     
}


void P00RunData::CreateTree( const G4String& name, const G4String& title )
{
  auto mgr = G4AnalysisManager::Instance();

  mgr->CreateNtuple(name,title);
  mgr->CreateNtupleIColumn("trkId"); // columnID = 0
  mgr->CreateNtupleIColumn("trkIdMtr");
  mgr->CreateNtupleIColumn("trkStatus");
  mgr->CreateNtupleIColumn("trkPdgId");
  mgr->CreateNtupleDColumn("trkProperTime");
  mgr->CreateNtupleDColumn("trkPolVctX");
  mgr->CreateNtupleDColumn("trkPolVctY");
  mgr->CreateNtupleDColumn("trkPolVctZ");
  mgr->CreateNtupleDColumn("trkPolMag");
  mgr->CreateNtupleDColumn("trkGlobalTime");
  mgr->CreateNtupleDColumn("trkLocalTime");
  mgr->CreateNtupleDColumn("trkLength");
  mgr->CreateNtupleDColumn("trkVelocity");
  mgr->CreateNtupleDColumn("trkTotE");
  mgr->CreateNtupleDColumn("trkKinE");
  mgr->CreateNtupleDColumn("trkPosX");
  mgr->CreateNtupleDColumn("trkPosY");
  mgr->CreateNtupleDColumn("trkPosZ");
  mgr->CreateNtupleDColumn("trkMomVctX");
  mgr->CreateNtupleDColumn("trkMomVctY");
  mgr->CreateNtupleDColumn("trkMomVctZ");
  mgr->CreateNtupleDColumn("trkMomMag");
  mgr->CreateNtupleDColumn("trkMomDirX");
  mgr->CreateNtupleDColumn("trkMomDirY");
  mgr->CreateNtupleDColumn("trkMomDirZ");
  mgr->CreateNtupleDColumn("trkProdVtxX");
  mgr->CreateNtupleDColumn("trkProdVtxY");
  mgr->CreateNtupleDColumn("trkProdVtxZ");
  mgr->CreateNtupleDColumn("trkMomDirPVX");
  mgr->CreateNtupleDColumn("trkMomDirPVY");
  mgr->CreateNtupleDColumn("trkMomDirPVZ");
  mgr->CreateNtupleDColumn("trkKinEPV");
  mgr->CreateNtupleIColumn("trkVolIdPV");
  mgr->CreateNtupleIColumn("trkStepNum");
  mgr->CreateNtupleDColumn("trkStepLen");
  mgr->CreateNtupleIColumn("trkPreVolId");
  mgr->CreateNtupleIColumn("trkPostVolId");

  mgr->CreateNtupleDColumn("stpTotE");
  mgr->CreateNtupleDColumn("stpNonIoniE");
  mgr->CreateNtupleDColumn("stpLength");
  mgr->CreateNtupleDColumn("stpDeltaT");
  mgr->CreateNtupleDColumn("stpDeltaPosX");
  mgr->CreateNtupleDColumn("stpDeltaPosY");
  mgr->CreateNtupleDColumn("stpDeltaPosZ");

  mgr->CreateNtupleIColumn("preStatus");
  mgr->CreateNtupleDColumn("preMass");
  mgr->CreateNtupleDColumn("preCharge");
  mgr->CreateNtupleDColumn("preProperTime");
  mgr->CreateNtupleDColumn("preMagneticMom");
  mgr->CreateNtupleDColumn("prePolVctX");
  mgr->CreateNtupleDColumn("prePolVctY");
  mgr->CreateNtupleDColumn("prePolVctZ");
  mgr->CreateNtupleDColumn("prePolMag");
  mgr->CreateNtupleDColumn("preGlobalTime");
  mgr->CreateNtupleDColumn("preLocalTime");
  mgr->CreateNtupleDColumn("preVelocity");
  mgr->CreateNtupleDColumn("preBeta");
  mgr->CreateNtupleDColumn("preGamma");
  mgr->CreateNtupleDColumn("preTotE");
  mgr->CreateNtupleDColumn("preKinE");
  mgr->CreateNtupleDColumn("preWorldPosX");
  mgr->CreateNtupleDColumn("preWorldPosY");
  mgr->CreateNtupleDColumn("preWorldPosZ");
  mgr->CreateNtupleDColumn("preLocalPosX");
  mgr->CreateNtupleDColumn("preLocalPosY");
  mgr->CreateNtupleDColumn("preLocalPosZ");
  mgr->CreateNtupleDColumn("preMomVctX");
  mgr->CreateNtupleDColumn("preMomVctY");
  mgr->CreateNtupleDColumn("preMomVctZ");
  mgr->CreateNtupleDColumn("preMomMag");
  mgr->CreateNtupleDColumn("preMomDirX");
  mgr->CreateNtupleDColumn("preMomDirY");
  mgr->CreateNtupleDColumn("preMomDirZ");
  mgr->CreateNtupleIColumn("preProcId");
  mgr->CreateNtupleIColumn("preProcType");
  mgr->CreateNtupleIColumn("preProcSubType");
  mgr->CreateNtupleIColumn("preVolId");
  mgr->CreateNtupleIColumn("preVolDepth");
  mgr->CreateNtupleIColumn("preVolCopyNum");

  mgr->CreateNtupleIColumn("postStatus");
  mgr->CreateNtupleDColumn("postMass");
  mgr->CreateNtupleDColumn("postCharge");
  mgr->CreateNtupleDColumn("postProperTime");
  mgr->CreateNtupleDColumn("postMagneticMom");
  mgr->CreateNtupleDColumn("postPolVctX");
  mgr->CreateNtupleDColumn("postPolVctY");
  mgr->CreateNtupleDColumn("postPolVctZ");
  mgr->CreateNtupleDColumn("postPolMag");
  mgr->CreateNtupleDColumn("postGlobalTime");
  mgr->CreateNtupleDColumn("postLocalTime");
  mgr->CreateNtupleDColumn("postVelocity");
  mgr->CreateNtupleDColumn("postBeta");
  mgr->CreateNtupleDColumn("postGamma");
  mgr->CreateNtupleDColumn("postTotE");
  mgr->CreateNtupleDColumn("postKinE");
  mgr->CreateNtupleDColumn("postWorldPosX");
  mgr->CreateNtupleDColumn("postWorldPosY");
  mgr->CreateNtupleDColumn("postWorldPosZ");
  mgr->CreateNtupleDColumn("postLocalPosX");
  mgr->CreateNtupleDColumn("postLocalPosY");
  mgr->CreateNtupleDColumn("postLocalPosZ");
  mgr->CreateNtupleDColumn("postMomVctX");
  mgr->CreateNtupleDColumn("postMomVctY");
  mgr->CreateNtupleDColumn("postMomVctZ");
  mgr->CreateNtupleDColumn("postMomMag");
  mgr->CreateNtupleDColumn("postMomDirX");
  mgr->CreateNtupleDColumn("postMomDirY");
  mgr->CreateNtupleDColumn("postMomDirZ");
  mgr->CreateNtupleIColumn("postProcId");
  mgr->CreateNtupleIColumn("postProcType");
  mgr->CreateNtupleIColumn("postProcSubType");
  mgr->CreateNtupleIColumn("postVolId");
  mgr->CreateNtupleIColumn("postVolDepth");
  mgr->CreateNtupleIColumn("postVolCopyNum");

  mgr->FinishNtuple();

}

void P00RunData::FillTree( const G4int& tid, const TreeVars& tv )
{
  auto mgr = G4AnalysisManager::Instance();

  G4int cid = 0;
  mgr->FillNtupleIColumn(tid,cid++,tv.trkId);
  mgr->FillNtupleIColumn(tid,cid++,tv.trkIdMtr);
  mgr->FillNtupleIColumn(tid,cid++,tv.trkStatus);
  mgr->FillNtupleIColumn(tid,cid++,tv.trkPdgId);
  mgr->FillNtupleDColumn(tid,cid++,tv.trkProperTime);
  mgr->FillNtupleDColumn(tid,cid++,tv.trkPolVct.x());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkPolVct.y());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkPolVct.z());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkPolVct.mag());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkGlobalTime);
  mgr->FillNtupleDColumn(tid,cid++,tv.trkLocalTime);
  mgr->FillNtupleDColumn(tid,cid++,tv.trkLength);
  mgr->FillNtupleDColumn(tid,cid++,tv.trkVelocity);
  mgr->FillNtupleDColumn(tid,cid++,tv.trkTotE);
  mgr->FillNtupleDColumn(tid,cid++,tv.trkKinE);
  mgr->FillNtupleDColumn(tid,cid++,tv.trkPos.x());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkPos.y());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkPos.z());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkMomVct.x());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkMomVct.y());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkMomVct.z());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkMomVct.mag());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkMomDir.x());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkMomDir.y());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkMomDir.z());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkProdVtx.x());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkProdVtx.y());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkProdVtx.z());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkMomDirPV.x());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkMomDirPV.y());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkMomDirPV.z());
  mgr->FillNtupleDColumn(tid,cid++,tv.trkKinEPV);
  mgr->FillNtupleIColumn(tid,cid++,tv.trkVolIdPV);
  mgr->FillNtupleIColumn(tid,cid++,tv.trkStepNum);
  mgr->FillNtupleDColumn(tid,cid++,tv.trkStepLen);
  mgr->FillNtupleIColumn(tid,cid++,tv.trkPreVolId);
  mgr->FillNtupleIColumn(tid,cid++,tv.trkPostVolId);

  mgr->FillNtupleDColumn(tid,cid++,tv.stpTotE);
  mgr->FillNtupleDColumn(tid,cid++,tv.stpNonIoniE);
  mgr->FillNtupleDColumn(tid,cid++,tv.stpLength);
  mgr->FillNtupleDColumn(tid,cid++,tv.stpDeltaT);
  mgr->FillNtupleDColumn(tid,cid++,tv.stpDeltaPos.x());
  mgr->FillNtupleDColumn(tid,cid++,tv.stpDeltaPos.y());
  mgr->FillNtupleDColumn(tid,cid++,tv.stpDeltaPos.z());

  mgr->FillNtupleIColumn(tid,cid++,tv.preStatus);
  mgr->FillNtupleDColumn(tid,cid++,tv.preMass);
  mgr->FillNtupleDColumn(tid,cid++,tv.preCharge);
  mgr->FillNtupleDColumn(tid,cid++,tv.preProperTime);
  mgr->FillNtupleDColumn(tid,cid++,tv.preMagneticMom);
  mgr->FillNtupleDColumn(tid,cid++,tv.prePolVct.x());
  mgr->FillNtupleDColumn(tid,cid++,tv.prePolVct.y());
  mgr->FillNtupleDColumn(tid,cid++,tv.prePolVct.z());
  mgr->FillNtupleDColumn(tid,cid++,tv.prePolVct.mag());
  mgr->FillNtupleDColumn(tid,cid++,tv.preGlobalTime);
  mgr->FillNtupleDColumn(tid,cid++,tv.preLocalTime);
  mgr->FillNtupleDColumn(tid,cid++,tv.preVelocity);
  mgr->FillNtupleDColumn(tid,cid++,tv.preBeta);
  mgr->FillNtupleDColumn(tid,cid++,tv.preGamma);
  mgr->FillNtupleDColumn(tid,cid++,tv.preTotE);
  mgr->FillNtupleDColumn(tid,cid++,tv.preKinE);
  mgr->FillNtupleDColumn(tid,cid++,tv.preWorldPos.x());
  mgr->FillNtupleDColumn(tid,cid++,tv.preWorldPos.y());
  mgr->FillNtupleDColumn(tid,cid++,tv.preWorldPos.z());
  mgr->FillNtupleDColumn(tid,cid++,tv.preLocalPos.x());
  mgr->FillNtupleDColumn(tid,cid++,tv.preLocalPos.y());
  mgr->FillNtupleDColumn(tid,cid++,tv.preLocalPos.z());
  mgr->FillNtupleDColumn(tid,cid++,tv.preMomVct.x());
  mgr->FillNtupleDColumn(tid,cid++,tv.preMomVct.y());
  mgr->FillNtupleDColumn(tid,cid++,tv.preMomVct.z());
  mgr->FillNtupleDColumn(tid,cid++,tv.preMomVct.mag());
  mgr->FillNtupleDColumn(tid,cid++,tv.preMomDir.x());
  mgr->FillNtupleDColumn(tid,cid++,tv.preMomDir.y());
  mgr->FillNtupleDColumn(tid,cid++,tv.preMomDir.z());
  mgr->FillNtupleIColumn(tid,cid++,tv.preProcId);
  mgr->FillNtupleIColumn(tid,cid++,tv.preProcType);
  mgr->FillNtupleIColumn(tid,cid++,tv.preProcSubType);
  mgr->FillNtupleIColumn(tid,cid++,tv.preVolId);
  mgr->FillNtupleIColumn(tid,cid++,tv.preVolDepth);
  mgr->FillNtupleIColumn(tid,cid++,tv.preVolCopyNum);

  mgr->FillNtupleIColumn(tid,cid++,tv.postStatus);
  mgr->FillNtupleDColumn(tid,cid++,tv.postMass);
  mgr->FillNtupleDColumn(tid,cid++,tv.postCharge);
  mgr->FillNtupleDColumn(tid,cid++,tv.postProperTime);
  mgr->FillNtupleDColumn(tid,cid++,tv.postMagneticMom);
  mgr->FillNtupleDColumn(tid,cid++,tv.postPolVct.x());
  mgr->FillNtupleDColumn(tid,cid++,tv.postPolVct.y());
  mgr->FillNtupleDColumn(tid,cid++,tv.postPolVct.z());
  mgr->FillNtupleDColumn(tid,cid++,tv.postPolVct.mag());
  mgr->FillNtupleDColumn(tid,cid++,tv.postGlobalTime);
  mgr->FillNtupleDColumn(tid,cid++,tv.postLocalTime);
  mgr->FillNtupleDColumn(tid,cid++,tv.postVelocity);
  mgr->FillNtupleDColumn(tid,cid++,tv.postBeta);
  mgr->FillNtupleDColumn(tid,cid++,tv.postGamma);
  mgr->FillNtupleDColumn(tid,cid++,tv.postTotE);
  mgr->FillNtupleDColumn(tid,cid++,tv.postKinE);
  mgr->FillNtupleDColumn(tid,cid++,tv.postWorldPos.x());
  mgr->FillNtupleDColumn(tid,cid++,tv.postWorldPos.y());
  mgr->FillNtupleDColumn(tid,cid++,tv.postWorldPos.z());
  mgr->FillNtupleDColumn(tid,cid++,tv.postLocalPos.x());
  mgr->FillNtupleDColumn(tid,cid++,tv.postLocalPos.y());
  mgr->FillNtupleDColumn(tid,cid++,tv.postLocalPos.z());
  mgr->FillNtupleDColumn(tid,cid++,tv.postMomVct.x());
  mgr->FillNtupleDColumn(tid,cid++,tv.postMomVct.y());
  mgr->FillNtupleDColumn(tid,cid++,tv.postMomVct.z());
  mgr->FillNtupleDColumn(tid,cid++,tv.postMomVct.mag());
  mgr->FillNtupleDColumn(tid,cid++,tv.postMomDir.x());
  mgr->FillNtupleDColumn(tid,cid++,tv.postMomDir.y());
  mgr->FillNtupleDColumn(tid,cid++,tv.postMomDir.z());
  mgr->FillNtupleIColumn(tid,cid++,tv.postProcId);
  mgr->FillNtupleIColumn(tid,cid++,tv.postProcType);
  mgr->FillNtupleIColumn(tid,cid++,tv.postProcSubType);
  mgr->FillNtupleIColumn(tid,cid++,tv.postVolId);
  mgr->FillNtupleIColumn(tid,cid++,tv.postVolDepth);
  mgr->FillNtupleIColumn(tid,cid++,tv.postVolCopyNum);

  mgr->AddNtupleRow(tid);

}

void P00RunData::FillPerEvent()
{
  //  inline G4bool G4TNtupleManager<tools::wroot::ntuple>::FillNtupleTColumn
  //    (G4int ntupleId, G4int columnId, const std::string& value)
  //
  //  virtual G4bool AddNtupleRow(G4int ntupleId) final;

  auto mgr = G4AnalysisManager::Instance();
  for ( auto kinEnergy : fKinEnergy) {
    mgr->FillNtupleDColumn(1, 0, kinEnergy);
  }
  mgr->AddNtupleRow(1);
  
  for ( auto edep : fEdep ) {
    if( edep==0 ) return;
  }

  //-- fXPosition is a 1-dimension array that contain xPosition numbers
  //   for each volumn specified in P00RunData.hh
  for ( auto xPosition : fXPosition ) {
    mgr->FillNtupleDColumn(0, 0, xPosition);
  }
  for ( auto yPosition : fYPosition ) {
    mgr->FillNtupleDColumn(0, 1, yPosition);
  }
  for ( auto zPosition : fZPosition ) {
    mgr->FillNtupleDColumn(0, 2, zPosition);
  }
  mgr->AddNtupleRow(0);

  //  FillTree(mgr,2,IniTV);
  //  FillTree(mgr,3,EntTV);
  //  FillTree(mgr,4,EndTV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
void P00RunData::Reset()
{ 
  for ( auto& edep : fEdep )           { edep = 0.; }
  for ( auto& xPosition : fXPosition ) { xPosition = 0.; }
  for ( auto& yPosition : fYPosition ) { yPosition = 0.; }
  for ( auto& zPosition : fZPosition ) { zPosition = 0.; }
  for ( auto& kinEnergy : fKinEnergy ) { kinEnergy =0.; }
  DummyVars(IniTV);
  DummyVars(EntTV);
  DummyVars(EndTV);
}
