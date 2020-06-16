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
/// \file eventgenerator/particleGun/include/PrimaryGeneratorAction1.hh
/// \brief Definition of the PrimaryGeneratorAction1 class
//
//
// $Id$
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PrimaryGeneratorAction1_h
#define PrimaryGeneratorAction1_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

#include <iostream>
#include <fstream>

class G4ParticleGun;
class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
using namespace std;

class PrimaryGeneratorAction1
{
  public:
    PrimaryGeneratorAction1(G4ParticleGun*, const char* InputName, G4int InputN);
   ~PrimaryGeneratorAction1();

  public:
    void GeneratePrimaries(G4Event*);

  private:
    G4ParticleGun*  fParticleGun;
    fstream outFile;
    double n = 0;

    int N = 3326844;//3321746;//21385;//3231566;//21386;//3231566;//40524;//4287470;//7169969;//4287470;//7169969;//717135;//1085249;//71901;//1084426;// 108911;//10949;//1063;//10023120;//100000;//10023120;//10000;//10023120;//7063080;//9000;//7063080;//9000;//99174;//7063080; //1094702;//99224;//9856;//19675;//lines in the file

    double rX,rY,rZ,rPx,rPy,rPz,rEk,rT;
      std::vector<G4double>  X;
      std::vector<G4double>  Y;
      std::vector<G4double>  Z;
      std::vector<G4double>  Px;
      std::vector<G4double>  Py;
      std::vector<G4double>  Pz;
      std::vector<G4double>  Ek;
      std::vector<G4double>  T;

    char buffer[256];
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
