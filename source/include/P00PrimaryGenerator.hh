//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// P00PrimaryGenerator.hh
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef P00PrimaryGenerator_h
#define P00PrimaryGenerator_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class PrimaryGeneratorAction1;
class G4Event;
class G4ParticleGun;
class G4GeneralParticleSource;



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
class P00PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
  public:
    P00PrimaryGenerator(const G4String& InputName, const G4int& InputN);
   ~P00PrimaryGenerator();

  public:
    void GeneratePrimaries(G4Event*);

  private:
    G4GeneralParticleSource* fpParticleGun;

  public:
    G4ParticleGun* GetParticleGun() { return particleGun; };

    PrimaryGeneratorAction1*  GetAction1() { return action1; };

  private:
    G4ParticleGun*           particleGun;
    PrimaryGeneratorAction1* action1;

};
#endif


