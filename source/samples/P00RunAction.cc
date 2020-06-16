//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// P00RunAction.cc
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "P00RunAction.hh"
#include "P00RunData.hh"
#include "P00Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
P00RunAction::P00RunAction():G4UserRunAction()
{
  auto mgr = G4AnalysisManager::Instance();
  G4cout << "Using " << mgr->GetType() << G4endl;
  
  mgr->SetVerboseLevel(0);

  // Set this to start hid from "1" (will start from "0" otherwise).
  mgr->SetFirstHistoId(1);
  mgr->CreateH1("EvtStats","Event statistics",15,0,15);
  mgr->CreateH1("TrkStatF","Track status at the first step",6,0,6);
  mgr->CreateH1("TrkStatT","Track status at the target step",6,0,6);
  mgr->CreateH1("TrkStatL","Track status at the last step",6,0,6);
  mgr->CreateH1("ProcTypeL","Process type at the last step",15,0,15);

  mgr->SetNtupleMerging(true);

  //-- First Ntuple, ID = 0
  mgr->CreateNtuple("position","mu+ decay position in the target");
  // Column takes values of G4double type
  mgr->CreateNtupleDColumn("x"); // columnID = 0
  mgr->CreateNtupleDColumn("y"); // columnID = 1
  mgr->CreateNtupleDColumn("z"); // columnID = 2
  mgr->FinishNtuple();

  //-- Second Ntuple, ID = 1
  mgr->CreateNtuple("energy", "mu+ initial kinetic energy");
  mgr->CreateNtupleDColumn("e"); // columnID = 0
  mgr->FinishNtuple();

  auto runData = static_cast<P00RunData*>
    (G4RunManager::GetRunManager()->GetNonConstCurrentRun());

  //-- Third Ntuple, ID = 2
  //  runData->CreateTree("Init","mu+ at the initial point");
  //-- Fourth Ntuple, ID = 3
  //  runData->CreateTree("Entrance","mu+ at the entrance point of aerogel");
  //-- Fifth Ntuple, ID = 4
  //  runData->CreateTree("End","mu+ at the end point of tracking");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
P00RunAction::~P00RunAction()
{
  delete G4AnalysisManager::Instance();  
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
// Controlling of creating a run, beginning a run and end a run. 
G4Run* P00RunAction::GenerateRun() //Create a run
{
  return (new P00RunData);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
void P00RunAction::BeginOfRunAction(const G4Run* run)
{
  // G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  // G4RunManager::GetRunManager()->SetRandomNumberStoreDir("randomInfo/");
  // if (isMaster) G4Random::showEngineStatus();
  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;
  auto mgr = G4AnalysisManager::Instance();
  mgr->OpenFile("P00RawData");
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
void P00RunAction::EndOfRunAction(const G4Run*)   // print histogram statistics
{
  auto mgr = G4AnalysisManager::Instance();
  mgr->Write();
  mgr->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
