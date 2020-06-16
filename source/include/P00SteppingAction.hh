//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// P00SteppingAction.hh
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// Stepping action class.
/// 
/// In this class: x, y, z position and energy deposite inside aerogel are collected and
/// updated in P00RunData object.
#ifndef P00SteppingAction_h
#define P00SteppingAction_h 1

#include "G4UserSteppingAction.hh"

class Geometry;
class G4Track;
class G4StepPoint;
class G4Material;
class G4String;
class P00EventAction;

class P00SteppingAction : public G4UserSteppingAction
{
public:
  P00SteppingAction(G4UserEventAction* eventAction);
  virtual ~P00SteppingAction();
  virtual void UserSteppingAction(const G4Step* step);

private:
  void PrintTrackInfo(const G4Track* track, const G4String& tag);
  void PrintStepInfo(const G4Step* step, const G4String& tag);
  void PrintStepPointInfo(const G4StepPoint* point, const G4String& tag);
  void PrintMaterialInfo(const G4Material* mate, const G4String& tag);

  P00EventAction* fEvtAct;
};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
#endif
