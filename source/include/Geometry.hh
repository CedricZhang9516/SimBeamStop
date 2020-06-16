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
  Geometry( const G4int& targetNumber, const G4int& DGthickness);
  ~Geometry();

  static std::map<G4String,G4int> volumeID;

private:
  //-- UIcommand parameters and methods
  G4GenericMessenger* fMessenger;
  void DefineCommands();
  G4int fTgtNum;
  G4int fDGthickness;
  void SetTargetNum( G4int val );
  G4int GetTargetNum() const { return fTgtNum; }
  G4double fTgtThickScl;
  void SetTargetThickScale( G4double val );
  G4double GetTargetThickScale() const { return fTgtThickScl; }
  G4double fTgtThickUnscaled;
  G4double fTgtThickScaled;
  void SetTargetThickness();
  void SetTargetThicknessIF( G4double val );
  G4double GetTargetThickness() const { return fTgtThickScaled; }
  void PrintTargetParam();
  G4double fMylThick;
  void SetMylarThickness( G4double val );
  G4double GetMylarThickness() const { return fMylThick; }

  //-- For material definitions
  G4Material* fMateDefault;
  G4Material* fMatePb;
  G4Material* fMateAl;
  G4Material* fMateMylar;
  G4Material* fMateKapton;
  G4Material* fMateScint;
  G4Material* fMateSiO2;
  //  G4Material* fPerfLyrMaterial;

  G4Material* fMateDegrader;
  G4Material* fMateTgtSP;
  G4Material* fMateTgtAl;
  G4Material* fMateMSR12_2a;
  G4Material* fMateMSR12_2c;
  G4Material* fMateMSR12_2d;
  G4Material* fMateMSR12_3b;
  G4Material* fMateMSR12_3c;
  G4Material* fMateMSR12_3d;
  G4Material* fMatePMSQ_LD;
  G4Material* fMatePMSQ_HD;
  G4Material* fMateMSR08_2b3;

  G4Material* StainlessSteel;

  void DefineMaterials();

  //-- For geometry definitions
  const G4double fBCThick;
  G4double fBCPosZ;
  G4double fDGPosZ;
  G4VPhysicalVolume* fPVolWorld;
  G4VPhysicalVolume* fPVolCollim1;
  G4VPhysicalVolume* fPVolCollim2;
  G4VPhysicalVolume* fPVolMylWin;
  G4VPhysicalVolume* fPVolAlPipe;
  G4VPhysicalVolume* fPVolAlHead;
  G4VPhysicalVolume* fPVolVetoCnt;
  std::vector<G4VPhysicalVolume*>* fPVolMylWrps;
  G4VPhysicalVolume* fPVolBeamCnt;

  G4VPhysicalVolume* fPVolDegrader;
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
  G4VPhysicalVolume* GeomDG( const G4int& id );
  G4VPhysicalVolume* GeomDG2( const G4int& id );
  G4VPhysicalVolume* GeomTgt( const G4int& id );
  G4VPhysicalVolume* GeomSP( const G4int& id );
  G4VPhysicalVolume* GeomS08( const G4int& id );
  G4VPhysicalVolume*
  SetPhysVol( const G4String& name, G4VSolid* solid,
	      G4Material* material, G4VPhysicalVolume* mtrVol,
	      const G4ThreeVector& transVec );
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
#endif
