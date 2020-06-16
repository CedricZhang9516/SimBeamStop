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

P00EventAction::P00EventAction()
  : G4UserEventAction(), eventID(0)
{}

P00EventAction::~P00EventAction()
{}

void P00EventAction::BeginOfEventAction(const G4Event* event)
{  
  eventID = event->GetEventID();
  auto runData = static_cast<P00RunData*>
    (G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  runData->Reset();
}
  
void P00EventAction::EndOfEventAction(const G4Event* event)
{
  auto mgr = G4AnalysisManager::Instance();
  auto runData = static_cast<P00RunData*>
    (G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  runData->PrintPerEvent(event);
  runData->FillPerEvent(mgr);
}
