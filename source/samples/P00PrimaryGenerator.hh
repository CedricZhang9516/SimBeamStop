//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// P00PrimaryGenerator.hh
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef P00PrimaryGenerator_h
#define P00PrimaryGenerator_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
class G4Event;
class G4ParticleGun;
class G4GeneralParticleSource;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
class P00PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
  public:
    P00PrimaryGenerator();    
   ~P00PrimaryGenerator();

  public:
    void GeneratePrimaries(G4Event*);

  private:
    G4GeneralParticleSource* fpParticleGun;
};
#endif


