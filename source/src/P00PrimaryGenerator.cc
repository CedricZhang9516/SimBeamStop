//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// P00PrimaryGenerator.cc
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "P00PrimaryGenerator.hh"
#include "PrimaryGeneratorAction1.hh"

#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
P00PrimaryGenerator::P00PrimaryGenerator(const G4String& InputName, const G4int& InputN)
: fpParticleGun(0)
{

	G4int n_particle = 1;

	particleGun  = new G4ParticleGun(n_particle);

	fpParticleGun = new G4GeneralParticleSource();

	G4ParticleDefinition* particle
	       = G4ParticleTable::GetParticleTable()->FindParticle("mu+");
	particleGun->SetParticleDefinition(particle);
	//particleGun->SetParticleDefinition(particle);
	/*
	particleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
	particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,1.,0.));
	particleGun->SetParticleEnergy(100.*GeV);
	particleGun->SetParticlePosition(G4ThreeVector(0.*cm,0.*cm,0.*cm));
	*/
	action1 = new PrimaryGeneratorAction1(particleGun, InputName, InputN);
	// not to be used, will be replaced by the particleGun

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
P00PrimaryGenerator::~P00PrimaryGenerator()
{
  delete fpParticleGun;
  delete action1;
  delete particleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
void P00PrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
	action1->GeneratePrimaries(anEvent);
  // only here need to be changed into MC mode
  //fpParticleGun->GeneratePrimaryVertex(anEvent);
}

