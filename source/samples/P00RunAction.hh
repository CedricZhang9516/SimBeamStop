//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// P00RunAction.hh
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// Run action class
///
/// It accumulates statistic and computes dispersion of the energy deposit 
/// and track lengths of charged particles with use of analysis tools:
/// Ntuple are created in BeginOfRunAction() for the following 
/// physics quantities:
/// - XPosition in aerogel
/// - YPosition in aerogel
/// - ZPosition in aerogel
/// The histograms are saved in the output file in a format
/// accoring to a selected technology in P00Analysis.hh. (default: root)
///
/// In EndOfRunAction(), the accumulated statistic and computed 
/// dispersion is printed.
///
#ifndef P00RunAction_h
#define P00RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh" // Global Constants and typedefs
#include "Randomize.hh"

class G4Run;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
class P00RunAction : public G4UserRunAction
{
public:
           P00RunAction();
  virtual ~P00RunAction();
  virtual G4Run* GenerateRun();
  virtual void BeginOfRunAction(const G4Run*);
  virtual void   EndOfRunAction(const G4Run*);
};
#endif