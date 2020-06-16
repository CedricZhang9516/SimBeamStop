//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Geometry.cc
/*
BC scintillator replacement for the 2017 runs of S1249
 The replacement BC is being constructed by Steve Chan in our scintillator
 shop now. They have requested a small change in dimension and will perhaps
 add a step in the "fishtail" rectangular-to-round conversion piece in order
 to make the rectangular face glue joint stronger and easier to align. Steve
 has the drawings of the holder from Sirui's talk at our last meeting and is
 aware of the constraints. He has selected a sample of BC408 scintillator
 that is "measured" to be 0.013" (0.33 mm) with variation in thickness of
 <0.0002" (<0.005 mm). I have asked him to weigh and measure the scintillator
 carefully before gluing it in so that we have the best possible estimate of
 thickness in g/cm^2.
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "Geometry.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4VPhysicalVolume.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

std::map<G4String,G4int> Geometry::volumeID;

Geometry::Geometry( const G4int& targetNumber )
  : G4VUserDetectorConstruction()
  , fthickBC(0.33*mm)
  , fMessenger(nullptr)
{
  //fTargetNum = 0;
  fTargetNum = targetNumber;
  fMylarThickness = 0.05*mm;
  fposZBC = -12*mm - fthickBC/2;
  
  DefineCommands();
  DefineMaterials();
}

Geometry::~Geometry()
{
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

void
Geometry::DefineCommands()
{
  fMessenger = new G4GenericMessenger(this,"/SimBeamStop/geometry/",
				      "Geometry control");

  auto& cmdMylThick
    = fMessenger->DeclareMethodWithUnit("mylar_thickness","mm",
					&Geometry::SetMylarThickness,
					"Set mylar thickness");
  cmdMylThick.SetParameterName("mylar_thickness",true);
  cmdMylThick.SetRange("mylar_thickness >= 0. && mylar_thickness <= 1.");
  cmdMylThick.SetDefaultValue("0.05");

  auto& cmdTgtNum
    = fMessenger->DeclareMethod("target_number",
				&Geometry::SetTargetNum,
				"Set the target number");
  cmdTgtNum.SetParameterName("target_number",true);
  //  cmdTgtNum.SetRange("target_number == 0 || target_number == 1");
  //-- Invoking SetRange() interrupts the program execution
  //   with the message "parameter out of range: ..."
  //   Problem of DeclareMethod()?
  cmdTgtNum.SetDefaultValue("0");
}

void
Geometry::SetTargetNum( G4int val )
{
  if( val != 0 && val != 1 ){
    G4cout << "***** Illegal parameter: "
	   << "</SimBeamStop/geometry/target_number "
	   << val << "> *****"
	   << G4endl;
    G4cout << "The parameter range allowed is: "
	   << "target_number == 0 || target_number ==1"
	   << G4endl;
    G4cout << "Parameter change refused."
	   << G4endl;
    return;
  }
  fTargetNum = val;
  G4RunManager::GetRunManager()->ReinitializeGeometry();  

  G4cout << "Parameter changed: target_number= "
	 << fTargetNum
	 << G4endl;
}

void
Geometry::SetMylarThickness( G4double val )
{
  fMylarThickness = val;

  G4Tubs* tube = (G4Tubs*)fPVolMylWin->GetLogicalVolume()->GetSolid();
  tube->SetZHalfLength(fMylarThickness/2);
  
  // tell G4RunManager that we change the geometry
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  
  G4cout << "Parameter changed: mylar_thickness= "
	 << G4BestUnit(GetMylarThickness(),"Length")
	 << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

void
Geometry::DefineMaterials()
{ 
  // use G4-NIST materials data base
  G4NistManager* materialMan = G4NistManager::Instance();
  fDefaultMaterial = materialMan->FindOrBuildMaterial("G4_Galactic");
  fPbMaterial      = materialMan->FindOrBuildMaterial("G4_Pb");
  fPipeMaterial    = materialMan->FindOrBuildMaterial("G4_Al");
  fMylarMaterial   = materialMan->FindOrBuildMaterial("G4_MYLAR");
  fKaptonMaterial  = materialMan->FindOrBuildMaterial("G4_KAPTON");
  fScintillatorMaterial
    = materialMan->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");;
  fSiO2            = materialMan->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  G4Element* Si    = materialMan->FindOrBuildElement("Si");
  G4Element* O     = materialMan->FindOrBuildElement("O");
  G4int    nel;    // nel=number of elements;
  G4int    natoms; // natoms=number of atoms
  G4double density = 0.0235*CLHEP::g/CLHEP::cm3;
  fPerfLyrMaterial = new G4Material("Aerogel1", density, nel=2);
  fPerfLyrMaterial->AddElement(Si, natoms=1);
  fPerfLyrMaterial->AddElement(O, natoms=2);

  //=============== G4_vacuum =================
  // From PhysicalConstants.h
  // G4double density, massfraction, nComponents, temperature, pressure;  
  //  density     = 2.376e-15*g/cm3;
  //  pressure    = 1.e-19*pascal;
  //  temperature = 300*kelvin;
  //  G4Material* vacuum = new G4Material( "Galactic", density, nComponents=1,
  //				       kStateGas, temperature, pressure );
  //  vacuum-> AddMaterial( materialMan->FindOrBuildMaterial("G4_AIR"),
  //		        massfraction=1.0 );
  //  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

G4VPhysicalVolume*
Geometry::Construct()
{
  // Clean old geometry, if any
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  fPVolWorld   = GeomWorld(0);
  fPVolAlPipe  = GeomAlPipe(1);
  fPVolCollim1 = GeomCollimator1(2);
  fPVolCollim2 = GeomCollimator2(3);
  fPVolAlHead  = GeomAlHead(4);
  fPVolMylWin  = GeomMylarWindow(5);
  fPVolVetoCnt = GeomVC(6);
  fPVolMylWrps = GeomMylarWraps(7,2);
  fPVolBeamCnt = GeomBC(9);
  
  switch( fTargetNum ){
  case 1:  fPVolTarget = GeomS08(10); break;
  default: fPVolTarget = GeomSP(10);
  }
  
  return fPVolWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

G4VPhysicalVolume*
Geometry::SetPhysVol( const G4String& name, G4VSolid* solid,
		      G4Material* material, G4VPhysicalVolume* mtrVol,
		      const G4ThreeVector& transVec )
{ 
  G4LogicalVolume* logical= new G4LogicalVolume(solid,material,name);
  //  G4LogicalVolume( G4VSolid*             pSolid,
  //		   G4Material*           pMaterial,
  //		   const G4String&       Name,
  //		   G4FieldManager*       pFieldMgr=0,
  //		   G4VSensitiveDetector* pSDetector=0,
  //		   G4UserLimits*         pULimits=0,
  //		   G4bool                Optimise=true );

  G4RotationMatrix* pRot= nullptr;
  G4int  pMany    = 0;
  G4int  pCopyNo  = 0;
  G4bool pSurfChk = false;

  G4VPhysicalVolume* physical
    = new G4PVPlacement(pRot,transVec,name,logical,mtrVol,
			pMany,pCopyNo,pSurfChk);
  //-- Default constructor:
  //
  //  G4PVPlacement( G4RotationMatrix*    pRot,
  //		 const G4ThreeVector& tlate,
  //		 G4LogicalVolume*     pCurrentLogical,
  //		 const G4String&      pName,
  //		 G4LogicalVolume*     pMotherLogical,
  //		 G4bool               pMany,
  //		 G4int                pCopyNo,
  //		 G4bool               pSurfChk=false );
  //
  //-- Variation A: The 1st and 2nd arguments of the default constructor
  //                can be merged in to a signel argument.
  //  
  //    G4PVPlacement(const G4Transform3D &Transform3D,
  //                        G4LogicalVolume *pCurrentLogical,
  //                        ...
  //-- Variation B: The 5th argument of the default constructor can be
  //                "G4VPhysicalVolume*" instead of "G4LogicalVolume*"
  //                if the order of the 3rd and 4th arguments are swapped.
  //
  //    G4PVPlacement(G4RotationMatrix *pRot,
  //                  const G4ThreeVector &tlate,
  //		      const G4String &pName,
  //		      G4LogicalVolume *pLogical,
  //		      G4VPhysicalVolume *pMother,
  //                  ...
  //-- Variation C: Variation A and B can be combined.

  return physical;
}

 //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

G4VPhysicalVolume*
Geometry::GeomWorld( const G4int& id )
{
  const G4String& name = "World";
  const G4double& xlen = 200*mm;
  const G4double& ylen = 200*mm;
  const G4double& zlen = 400*mm;
  const G4double& zpos =   0*mm;
  G4Material* material = fDefaultMaterial;
  
  volumeID[name]= id;
  G4Box* solid = new G4Box(name,xlen/2,ylen/2,zlen/2);
  return SetPhysVol(name,solid,material,nullptr,G4ThreeVector(0,0,zpos));
  //  G4Box( const G4String& pName, G4double pX, G4double pY, G4double pZ );
}

G4VPhysicalVolume*
Geometry::GeomCollimator1( const G4int& id )
{
  const G4String& name = "PbCollimator1";
  const G4double& din  =  16*mm;
  const G4double& dout =  60*mm;
  const G4double& zlen =  50*mm;
  const G4double& zpos = -130*mm - zlen/2;
  G4Material* material = fPbMaterial;

  volumeID[name]= id;
  G4Tubs* solid= new G4Tubs(name,din/2,dout/2,zlen/2,0*deg,360*deg);
  return SetPhysVol(name,solid,material,fPVolWorld,G4ThreeVector(0,0,zpos));
  //  G4Tubs(const G4String& pName,
  //	 G4double  pRMin, G4double  pRMax, G4double  pDz,
  //	 G4double  pSPhi, G4double  pDPhi)
}

G4VPhysicalVolume*
Geometry::GeomCollimator2( const G4int& id )
{
  const G4String& name = "PbCollimator2";
  const G4double& din  =  12*mm;
  const G4double& dout =  60*mm;
  const G4double& zlen =  50*mm;
  const G4double& zpos = -80*mm - zlen/2;
  G4Material* material = fPbMaterial;

  volumeID[name]= id;
  G4Tubs* solid= new G4Tubs(name,din/2,dout/2,zlen/2,0*deg,360*deg);
  return SetPhysVol(name,solid,material,fPVolWorld,G4ThreeVector(0,0,zpos));
}

G4VPhysicalVolume*
Geometry::GeomMylarWindow( const G4int& id )
{
  const G4String& name = "MylarWindow";
  const G4double& din  =   0*mm;
  const G4double& dout =  32*mm;
  const G4double& zlen = fMylarThickness;
  const G4double& zpos = -65*mm - zlen/2;
  G4Material* material = fMylarMaterial;

  volumeID[name]= id;
  G4Tubs* solid= new G4Tubs(name,din/2,dout/2,zlen/2,0*deg,360*deg);
  return SetPhysVol(name,solid,material,fPVolWorld,G4ThreeVector(0,0,zpos));
}

G4VPhysicalVolume*
Geometry::GeomAlPipe( const G4int& id )
{
  const G4String& name = "AlPipe";
  const G4double& din  =  60*mm;
  const G4double& dout =  70*mm;
  const G4double& zlen = 120*mm;
  const G4double& zpos = -80*mm - zlen/2;
  G4Material* material = fPipeMaterial;

  volumeID[name]= id;
  G4Tubs* solid= new G4Tubs(name,din/2,dout/2,zlen/2,0*deg,360*deg);
  return SetPhysVol(name,solid,material,fPVolWorld,G4ThreeVector(0,0,zpos));
}

G4VPhysicalVolume*
Geometry::GeomAlHead( const G4int& id )
{
  const G4String& name = "AlHead";
  const G4double& din  =  32*mm;
  const G4double& dout =  70*mm;
  const G4double& zlen =  25*mm;
  const G4double& zpos = -55*mm - zlen/2;
  G4Material* material = fPipeMaterial;

  volumeID[name]= id;
  G4Tubs* solid= new G4Tubs(name,din/2,dout/2,zlen/2,0*deg,360*deg);
  return SetPhysVol(name,solid,material,fPVolWorld,G4ThreeVector(0,0,zpos));
}

G4VPhysicalVolume*
Geometry::GeomVC( const G4int& id )
{
  const G4String& name  = "VC";
  const G4double& xlen  =  50*mm;
  const G4double& ylen  =  50*mm;
  const G4double& zlen  =   3*mm;
  const G4double& dhole =  10*mm;
  //  const G4double& zpos  = -53*mm;
  const G4double& zpos  = -50*mm - zlen/2;
  G4Material* material  = fScintillatorMaterial;

  volumeID[name]= id;
  G4Box*  box  = new G4Box (name+"Box",xlen/2,ylen/2,zlen/2);
  G4Tubs* hole = new G4Tubs(name+"Hole",0,dhole/2,zlen/2,0*deg,360*deg);
  G4VSolid* solid = new G4SubtractionSolid(name,box,hole);
  return SetPhysVol(name,solid,material,fPVolWorld,G4ThreeVector(0,0,zpos));
}

G4VPhysicalVolume*
Geometry::GeomBC( const G4int& id )
{
  const G4String& name = "BC";
  const G4double& xlen = 18*mm;
  const G4double& ylen = 18*mm;
  const G4double& zlen = fthickBC;
  const G4double& zpos = fposZBC;
  G4Material* material = fScintillatorMaterial;

  volumeID[name]= id;
  G4Box* solid = new G4Box(name,xlen/2,ylen/2,zlen/2);
  return SetPhysVol(name,solid,material,fPVolWorld,G4ThreeVector(0,0,zpos));
}

std::vector<G4VPhysicalVolume*>*
Geometry::GeomMylarWraps( const G4int& id, const G4int& ncopy )
{
  // Define 2 piece of 'mylars wrapped around BC' (thickness = 0.006 um)
  const G4String& name = "MylarWrap";
  const G4double& xlen = 18*mm;
  const G4double& ylen = 18*mm;
  const G4double& zlen = 0.006*mm;
  G4Material* material = fMylarMaterial;

  volumeID[name] = id;
  G4Box* solid = new G4Box(name,xlen/2,ylen/2,zlen/2);
  G4LogicalVolume* logical= new G4LogicalVolume(solid,material,name);

  G4String names[ncopy];
  for( G4int icopy=0; icopy<ncopy; icopy++ ){
    names[icopy] = name + std::to_string(icopy);
    volumeID[names[icopy]]= id + icopy;
  }

  G4double zposs[ncopy];
  zposs[0] = fposZBC - fthickBC/2 - zlen/2;
  zposs[1] = fposZBC + fthickBC/2 + zlen/2;
  
  G4RotationMatrix* pRot= nullptr;
  G4int  pMany    = 0;
  G4bool pSurfChk = false;

  std::vector<G4VPhysicalVolume*>* physicals
    = new std::vector<G4VPhysicalVolume*>();
  for( G4int icopy=0; icopy<ncopy; icopy++ ){
    G4VPhysicalVolume* physical
      = new G4PVPlacement(pRot,G4ThreeVector(0,0,zposs[icopy]),
			  names[icopy],logical,fPVolWorld,
			  pMany,icopy,pSurfChk);
    physicals->push_back(physical);
  }

  return physicals;
}

G4VPhysicalVolume*
Geometry::GeomSP( const G4int& id )
{
  //  const G4String& name = "SP";
  const G4String& name = "Target";
  const G4double& xlen = 30*mm;
  const G4double& ylen = 40*mm;
  const G4double& zlen = 0.1*mm;
  const G4double& zpos = -zlen/2;
  const G4double& ypos = -5*mm; // = (50-ylen)/2
  G4Material* material = fSiO2;

  volumeID[name]= id;
  G4Box* solid = new G4Box(name,xlen/2,ylen/2,zlen/2);
  return SetPhysVol(name,solid,material,fPVolWorld,G4ThreeVector(0,ypos,zpos));
}

G4VPhysicalVolume*
Geometry::GeomS08( const G4int& id )
{
  //  const G4String& name = "Aerogel";
  const G4String& name = "Target";
  const G4double& xlen = 50*mm;
  const G4double& ylen = 50*mm;
  //  const G4double& zlen = 9*mm;
  const G4double& zlen = 8.8*mm;
  const G4double& zpos = -zlen/2;
  G4Material* material = fPerfLyrMaterial;

  volumeID[name]= id;
  G4Box* solid = new G4Box(name,xlen/2,ylen/2,zlen/2);
  return SetPhysVol(name,solid,material,fPVolWorld,G4ThreeVector(0,0,zpos));
}
