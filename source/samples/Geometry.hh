//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Geometry.hh
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef Geometry_h
#define Geometry_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

#include <map>
#include <vector>

class G4Material;
class G4Box;
class G4Tubs;
class G4VSolid;
class G4SubtractionSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4GenericMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
class Geometry : public G4VUserDetectorConstruction
{
public:
  Geometry( const G4int& targetNumber );
  ~Geometry();
  
  static std::map<G4String,G4int> volumeID;
  
private:
  //-- UIcommand parameters and methods
  G4int    fTargetNum;
  G4double fMylarThickness;
  G4GenericMessenger* fMessenger;
  void DefineCommands();
  void SetTargetNum( G4int val );
  G4int GetTargetNum() const { return fTargetNum; }
  void SetMylarThickness( G4double val );
  G4double GetMylarThickness() const { return fMylarThickness; }

  //-- For material definitions
  G4Material* fDefaultMaterial;
  G4Material* fPbMaterial;
  G4Material* fPipeMaterial;
  G4Material* fMylarMaterial;
  G4Material* fKaptonMaterial;
  G4Material* fScintillatorMaterial;
  G4Material* fSiO2;
  G4Material* fPerfLyrMaterial;
  void DefineMaterials();
  
  //-- For geometry definitions
  const G4double fthickBC;
  G4double fposZBC;
  G4VPhysicalVolume* fPVolWorld;
  G4VPhysicalVolume* fPVolCollim1;
  G4VPhysicalVolume* fPVolCollim2;
  G4VPhysicalVolume* fPVolMylWin;
  G4VPhysicalVolume* fPVolAlPipe;
  G4VPhysicalVolume* fPVolAlHead;
  G4VPhysicalVolume* fPVolVetoCnt;
  std::vector<G4VPhysicalVolume*>* fPVolMylWrps;
  G4VPhysicalVolume* fPVolBeamCnt;
  G4VPhysicalVolume* fPVolTarget;
  G4VPhysicalVolume* Construct();
  G4VPhysicalVolume* GeomWorld( const G4int& id );
  G4VPhysicalVolume* GeomCollimator1( const G4int& id );
  G4VPhysicalVolume* GeomCollimator2( const G4int& id );
  G4VPhysicalVolume* GeomMylarWindow( const G4int& id );
  G4VPhysicalVolume* GeomAlPipe( const G4int& id );
  G4VPhysicalVolume* GeomAlHead( const G4int& id );
  G4VPhysicalVolume* GeomVC( const G4int& id );
  std::vector<G4VPhysicalVolume*>*
  GeomMylarWraps( const G4int& id, const G4int& ncopy );
  G4VPhysicalVolume* GeomBC( const G4int& id );
  G4VPhysicalVolume* GeomSP( const G4int& id );
  G4VPhysicalVolume* GeomS08( const G4int& id );
  G4VPhysicalVolume*
  SetPhysVol( const G4String& name, G4VSolid* solid,
	      G4Material* material, G4VPhysicalVolume* mtrVol,
	      const G4ThreeVector& transVec );
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
#endif
