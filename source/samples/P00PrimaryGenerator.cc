//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// P00PrimaryGenerator.cc
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "P00PrimaryGenerator.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
P00PrimaryGenerator::P00PrimaryGenerator()
: fpParticleGun(0) 
{
  fpParticleGun = new G4GeneralParticleSource();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
P00PrimaryGenerator::~P00PrimaryGenerator()
{
  delete fpParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
void P00PrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
  fpParticleGun->GeneratePrimaryVertex(anEvent);
}

