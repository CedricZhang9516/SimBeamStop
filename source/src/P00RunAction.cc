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
P00RunAction::P00RunAction(const G4String& OutputName):G4UserRunAction()
{
  auto mgr = G4AnalysisManager::Instance();
  G4cout << "Using " << mgr->GetType() << G4endl;
  mgr->SetVerboseLevel(0);
  mgr->SetFileName(OutputName);
  mgr->SetFirstHistoId(1); // Set the first hid to be "1". (Default is "0".)
  mgr->SetNtupleMerging(true); // Valid for threaded processes?

  auto runData = static_cast<P00RunData*>
    (G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  runData->CreateAtRunAction(mgr);
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
  mgr->OpenFile(); // The default file name is given by the 'SetFileName()'
                   // method in the constructor.
  // mgr->OpenFile("test"); // If the file name is given directly this way,
                            // the '/analysis/setFileName' command in G4 session
                            // won't work.

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
void P00RunAction::EndOfRunAction(const G4Run*)   // print histogram statistics
{
  auto mgr = G4AnalysisManager::Instance();
  mgr->Write();
  mgr->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
