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
P00ActionInitialization::P00ActionInitialization(const char* OutputName, const char* InputName, const G4int& InputN )
: G4VUserActionInitialization()
,fOutputName2(OutputName), fInputName2(InputName), fInputN2(InputN)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
P00ActionInitialization::~P00ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
void P00ActionInitialization::Build() const
{
  SetUserAction(new P00PrimaryGenerator(fInputName2, fInputN2) );
  //G4String fOutputName2("SimBeamTest");
  SetUserAction(new P00RunAction(fOutputName2));
  G4UserEventAction* eventAction = new P00EventAction;
  SetUserAction(eventAction);
  SetUserAction(new P00SteppingAction(eventAction));
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........



