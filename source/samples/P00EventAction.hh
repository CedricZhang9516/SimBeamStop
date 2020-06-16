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

struct StepInfo {
  G4int stpnum;
  G4int trkstatus;
  G4int proctype;
  G4String procname;
  void set( const G4int& sn, const G4int& ts, const G4int& pt,
	    const G4String& pn ){
    stpnum   = sn;
    trkstatus= ts;
    proctype = pt;
    procname = pn;
  }
  void reset(){
    stpnum   = -1;
    trkstatus= -1;
    proctype = -1;
    procname = "n/a";
  }
  void print(){
    G4cout << "stpnum= " << stpnum
	   << ", trkstatus= " << trkstatus
	   << ", proctype= " << proctype
	   << ", procname= " << procname
	   << G4endl;
  }
};

struct ParticleInfo {
  StepInfo first;
  StepInfo target;
  StepInfo last;
  void reset(){
    first.reset();
    target.reset();
    last.reset();
  }
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
class P00EventAction : public G4UserEventAction
{
public:
           P00EventAction();
  virtual ~P00EventAction();

  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void    EndOfEventAction(const G4Event* event); // In EndOfEventAction(), it prints the data stored in P00RunData object.

  G4bool hitTarget;
  ParticleInfo muInfo;
  const G4int nbin;
  std::vector<G4bool> hEvtStat;

private:
  // methods
  void PrintEventStatistics(const G4double& x,
			    const G4double& y, const G4double& z) const;
};
#endif
