//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// P00EventAction.hh
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// Event action class
///

#ifndef P00EventAction_h
#define P00EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include <vector>

class P00EventAction : public G4UserEventAction
{
public:
           P00EventAction();
  virtual ~P00EventAction();

  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void    EndOfEventAction(const G4Event* event);

  G4int eventID;

private:
};
#endif
