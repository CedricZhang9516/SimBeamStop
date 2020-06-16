//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// P00EventAction.cc
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "P00EventAction.hh"
#include "P00RunData.hh"
#include "P00Analysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip> // for std::setw (Set field width), function like Tab key.

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
P00EventAction::P00EventAction()
  : G4UserEventAction(), nbin(15)
{
  hEvtStat.resize(nbin);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
P00EventAction::~P00EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
void P00EventAction::BeginOfEventAction(const G4Event* event)
{  
  //  auto eventID = event->GetEventID();
  //  G4cout << "Begin of event: " << eventID << G4endl;
  
  hitTarget= false;
  muInfo.reset();
  for( G4int ibin=0; ibin<nbin; ibin++ ) hEvtStat[ibin]= false;
  
  auto runData = static_cast<P00RunData*>
    (G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  runData->Reset();  
  runData->hitTarget= false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
  
void P00EventAction::EndOfEventAction(const G4Event* event)
{
  G4bool printF= false;
  G4bool printT= false;
  G4bool printL= false;
  if( muInfo.first.trkstatus  !=  0 &&
      muInfo.first.trkstatus  !=  2 ) printF= true;
  if( muInfo.target.stpnum    != -1 &&
      muInfo.target.trkstatus !=  0 &&
      muInfo.target.trkstatus !=  1 ) printT= true;
  if( muInfo.last.trkstatus   !=  2 ) printL= true;

  auto eventID = event->GetEventID();
  if( printF || printT || printL ){
    G4cout << "EndOfEvent: " << eventID << G4endl;
    if( printF ) G4cout << "unexpected muInfo.first: ";  muInfo.first.print();
    if( printT ) G4cout << "unexpected muInfo.target: "; muInfo.target.print();
    if( printL ) G4cout << "unexpected muInfo.last: ";   muInfo.last.print();
  }

  auto anaMgr = G4AnalysisManager::Instance();
  for( G4int ibin=0; ibin<nbin; ibin++ ){
    if( hEvtStat[ibin] ) anaMgr->FillH1(1,ibin);
  }
  anaMgr->FillH1(2,muInfo.first.trkstatus);
  anaMgr->FillH1(3,muInfo.target.trkstatus);
  anaMgr->FillH1(4,muInfo.last.trkstatus);
  anaMgr->FillH1(5,muInfo.last.proctype);
  
  auto runData = static_cast<P00RunData*>
    (G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  runData->FillPerEvent();
//  runData->FillTree(2,runData->IniTV);
//  runData->FillTree(3,runData->EntTV);
//  runData->FillTree(4,runData->EndTV);


  //  PrintEventStatistics(runData->GetXPosition(dTargetID),
  //		       runData->GetYPosition(dTargetID),
  //		       runData->GetZPosition(dTargetID));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

// print event statistics, will be used at EndOfEventAction()
void P00EventAction::PrintEventStatistics
( const G4double& x, const G4double& y, const G4double& z ) const
{
  G4cout
    << "   Target: stopped mu+ x position: "
    << std::setw(7) << G4BestUnit(x, "Length") << G4endl
    << "   Target: stopped mu+ y position: "
    << std::setw(7) << G4BestUnit(y, "Length") << G4endl
    << "   Target: stopped mu+ z position: "
    << std::setw(7) << G4BestUnit(z, "Length") << G4endl;
}  
