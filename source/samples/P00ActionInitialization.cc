//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// P00ActionInitialization.cc
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "P00ActionInitialization.hh"
#include "P00PrimaryGenerator.hh"

#include "P00RunAction.hh"
#include "P00EventAction.hh"
#include "P00SteppingAction.hh"
#include "Geometry.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
P00ActionInitialization::P00ActionInitialization()
: G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
P00ActionInitialization::~P00ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
void P00ActionInitialization::Build() const
{
  SetUserAction(new P00PrimaryGenerator);
  SetUserAction(new P00RunAction);
  G4UserEventAction* eventAction = new P00EventAction;
  SetUserAction(eventAction);
  SetUserAction(new P00SteppingAction(eventAction));
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........



