//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// P00ActionInitialization.hh
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef P00ActionInitialization_h
#define P00ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class Geometry;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
class P00ActionInitialization : public G4VUserActionInitialization
{
  public:
    P00ActionInitialization();
    virtual ~P00ActionInitialization();
    virtual void Build() const;
};
#endif



