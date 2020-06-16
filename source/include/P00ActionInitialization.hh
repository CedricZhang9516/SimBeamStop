//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// P00ActionInitialization.hh
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef P00ActionInitialization_h
#define P00ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "globals.hh" // Global Constants and typedefs

class Geometry;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
class P00ActionInitialization : public G4VUserActionInitialization
{
  public:
    P00ActionInitialization(const char* OutputName, const char* InputName, const G4int& InputN);
    virtual ~P00ActionInitialization();
    virtual void Build() const;
  private:
  	//char fOutputName2[];
  	G4String fOutputName2;
  	G4String fInputName2;
  	G4int fInputN2;
};
#endif



