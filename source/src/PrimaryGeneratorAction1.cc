//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file eventgenerator/particleGun/src/PrimaryGeneratorAction1.cc
/// \brief Implementation of the PrimaryGeneratorAction1 class
//
//
// $Id$
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction1.hh"
#include "P00PrimaryGenerator.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using namespace std;

PrimaryGeneratorAction1::PrimaryGeneratorAction1(G4ParticleGun* gun, const char* InputName, G4int InputN)
: fParticleGun(gun)
{
  //TH1F * hx = new TH1F("x","x",100,-50,50);
  //outFile.open("FromDline.txt");
  //outFile.open("Sline_0904_ns_1.txt");
  //outFile.open("S2area_191118_1.txt");
  //outFile.open("S2area_191202_tot.txt");
  //outFile.open("S2area_191215_tot2.txt");
  //outFile.open("150419_surfmu_at_focus_aftertune_p36init_sepON_maxNevt.dat.txt.Stopping.txt");
  //outFile.open("150201_surfmu_at_focus.dat.txt.Stopping.txt");
  //outFile.open("G-2EDM_output_1e6_gendat_afterfit_SEPON_all_dump-sum.dat.Stopping.txt");
  //outFile.open("/home/had/zhangce/Hline/data/g-2_output/g4beamline_TDR_Fig.3.14.BLTrackFile-focus5.txt");
  //outFile.open("/home/had/zhangce/Hline/data/g-2_output/Cedric_200526_tot.txt");
  G4String fInputName = InputName;
  //outFile.open("/home/had/zhangce/Hline/data/g-2_output/Cedric_200526_focus04_tot.txt");
  outFile.open(fInputName);

  N = InputN;

  G4cout << "N: " << N << G4endl;

  int i = 0;
  while( i < N){

      outFile.getline(buffer,256);
      sscanf(buffer,"%lf %lf %lf %lf %lf %lf %lf %lf",&rX,&rY,&rZ,&rPx,&rPy,&rPz,&rEk,&rT);
      X.push_back(rX);
      //rZ = -22;
      //hx->fill(rX);
      Y.push_back(rY);
      Z.push_back(rZ);
      Px.push_back(rPx);
      Py.push_back(rPy);
      Pz.push_back(rPz);
      Ek.push_back(rEk);
      T.push_back(rT);
      i++;
      /*
      G4cout<<"EventPrint "<<i<<" "
      <<rX<<" "
      <<rY<<" "
      <<rZ<<" "
      <<rX<<" "
      <<rY<<" "
      <<rZ<<" "
      <<rEk<<" "
      <<rT<<" "
      <<G4endl;
      */
  }

  outFile.close();
  //hx->SaveAS("hx.pdf");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction1::~PrimaryGeneratorAction1()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction1::GeneratePrimaries(G4Event* anEvent)
{
  //const G4double r = 2*mm;
  //const G4double zmax = -198*mm;
  const G4double zmax = -198*mm;
  /*
  double P = sqrt(Px.at(n)*Px[n] + Py[n]*Py[n] + Pz[n]*Pz[n]);
  Px[n] /= P;
  Py[n] /= P;
  Pz[n] /= P;
  */
  //n = a*a+b*b+c*c;
  //a /= n;
  //b /= n;
  //c /= n;

  //vertex 1 uniform on cylinder
  //
  //G4double alpha = twopi*G4UniformRand();  //alpha uniform in (0, 2*pi)
  //G4double ux = std::cos(alpha);
  //G4double uy = std::sin(alpha);
  //G4double z = zmax*(2*G4UniformRand() - 1);  //z uniform in (-zmax, +zmax)
  G4double G4X = X.at(n);
  G4double G4Y = Y.at(n);
  G4double G4Z = Z.at(n);

  fParticleGun->SetParticlePosition(G4ThreeVector(G4X*mm,G4Y*mm,G4Z*mm));
  //fParticleGun->SetParticlePosition(G4ThreeVector((G4X-7)*mm,0,G4Z*mm));
  //fParticleGun->SetParticlePosition(G4ThreeVector(0,0,G4Z*mm));
  //fParticleGun->SetParticlePosition(G4ThreeVector(0,0,zmax));
  //fParticleGun->SetParticlePosition(G4ThreeVector((G4X-7)*mm,0,zmax));

  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(Px.at(n),Py.at(n),Pz.at(n)));
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));

  fParticleGun->SetParticleEnergy(Ek.at(n)*MeV);
  //fParticleGun->SetParticleEnergy(2.7955*MeV);
  //fParticleGun->SetParticleEnergy(3.7955*MeV);

  //fParticleGun->SetParticleTime(2*ns);
  fParticleGun->SetParticleTime(T.at(n)*ns);

  //fParticleGun->SetParticleMomentum(G4double aMomentum);

  fParticleGun->GeneratePrimaryVertex(anEvent);
  //G4cout<<"EndOfReadingEvent "<<n
  //           <<G4endl;
  n++;


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
