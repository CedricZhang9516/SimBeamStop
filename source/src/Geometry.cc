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

#include <cfloat>
#include <cstdlib>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

std::map<G4String,G4int> Geometry::volumeID;

Geometry::Geometry( const G4int& targetNumber, const G4int& DGthickness )
  : G4VUserDetectorConstruction()
  , fTgtNum(targetNumber)
  //, fBCThick(0.33*mm), fMylThick(0.05*mm)
  , fBCThick(0.05*mm), fMylThick(0.05*mm)
  , fTgtThickUnscaled(0*mm), fTgtThickScl(1)
  , fMessenger(nullptr)
  , fDGthickness(DGthickness)
{
  fBCPosZ = -153*mm;// - fBCThick/2;
  //fDGPosZ = fBCPosZ + fBCThick/2 + 1*mm;
  fDGPosZ = -7*mm-0.15*mm/2;
  //above two are no longer used, Cedric 0227
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

  auto& cmdTgtThickScl
    = fMessenger->DeclareMethod("target_thick_scale",
				&Geometry::SetTargetThickScale,
				"Set target thickness scale");
  cmdTgtThickScl.SetParameterName("target_thick_scale",true);
  //  cmdTgtThickScl.SetRange("target_thick_scale >= 0. && target_thick_scale <= 10.");
  //-- Invoking SetRange() interrupts the program execution
  //   with the message "parameter out of range: ..."
  //   Problem of DeclareMethod()?
  cmdTgtThickScl.SetDefaultValue("1.");

  auto& cmdTgtThick
    = fMessenger->DeclareMethodWithUnit("target_thickness","mm",
					&Geometry::SetTargetThicknessIF,
					"Set target thickness");
  cmdTgtThick.SetParameterName("target_thickness",true);
  cmdTgtThick.SetRange("target_thickness >= 0. && target_thickness <= 11.");
  cmdTgtThick.SetDefaultValue("0.");

  auto& cmdMylThick
    = fMessenger->DeclareMethodWithUnit("mylar_thickness","mm",
					&Geometry::SetMylarThickness,
					"Set mylar thickness");
  cmdMylThick.SetParameterName("mylar_thickness",true);
  cmdMylThick.SetRange("mylar_thickness >= 0. && mylar_thickness <= 1.");
  cmdMylThick.SetDefaultValue("0.05");
}

void
Geometry::SetTargetNum( G4int val )
{
  G4cout << "SetTargetNum: /SimBeamStop/geometry/target_number"
	 << G4endl;

  if( val < 0 || val > 27 ){
    G4cout << "Parameter out of range: "
	   << "target_number >= 0 && target_number <= 27."
	   << G4endl;
    G4cout << "***** Illegal parameter (99) "
	   << "</SimBeamStop/geometry/target_number " << val << "> *****"
	   << G4endl;
    G4cout << "SetTargetNum: aborting ..."
	   << G4endl;
    std::abort();
    //    return;
  }

  if( val == fTgtNum ){
    G4cout << "SetTargetNum:"
	   << " No change because of the new value (" << val << ")"
	   << " == the current value (" << fTgtNum << ")."
	   << G4endl;
    return;
  }

  fTgtNum = val;
  G4cout << "SetTargetNum: Changed fTgtNum to be " << fTgtNum << G4endl;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  // PrintTargetParam();
}

void
Geometry::SetTargetThickScale( G4double val )
{
  G4cout << "SetTargetThickScale: /SimBeamStop/geometry/target_thick_scale"
	 << G4endl;

  if( val < 0. ){
    G4cout << "Parameter out of range: "
	   << "target_thick_scale >= 0."
	   << G4endl;
    G4cout << "***** Illegal parameter (99) "
	   << "</SimBeamStop/geometry/target_thick_scale " << val << "> *****"
	   << G4endl;
    G4cout << "SetTargetThickScale: aborting ..."
	   << G4endl;
    std::abort();
    //    return;
  }

  if( fabs(val - fTgtThickScl)< DBL_EPSILON ){
    G4cout << "SetTargetThickScale:"
	   << " No change because of the new value (" << val << ")"
	   << " == the current value (" << fTgtThickScl << ")."
	   << G4endl;
    return;
  }

  fTgtThickScl = val;
  G4cout << "SetTargetThickScale: Changed fTgtThickScl to be " << fTgtThickScl
	 << G4endl;
  SetTargetThickness();
}

void
Geometry::SetTargetThicknessIF( G4double val )
{
  G4cout << "SetTargetThicknessIF: /SimBeamStop/geometry/target_thickness"
	 << G4endl;

  // Invalidade the scale factor when a specific thickness is directly given.
  fTgtThickScl      = 1.;
  fTgtThickUnscaled = val;
  G4cout << "SetTargetThicknessIF: Changed fTgtThickScl to be "
	 << fTgtThickScl
	 << G4endl;
  G4cout << "SetTargetThicknessIF: Changed fTgtThickUnscaled to be "
	 << G4BestUnit(fTgtThickUnscaled,"Length")
	 << G4endl;
  SetTargetThickness();
}

void
Geometry::SetTargetThickness()
{
  fTgtThickScaled = fTgtThickUnscaled * fTgtThickScl;
  const G4double& zlen = fTgtThickScaled;
  const G4double& zpos = -zlen/2;

  G4Box* solid = (G4Box*)fPVolTarget->GetLogicalVolume()->GetSolid();
  solid->SetZHalfLength(zlen/2);

  // Old, no distinction between frame/object.
  G4ThreeVector transVect   = fPVolTarget->GetTranslation();
  G4ThreeVector transVectObj= fPVolTarget->GetObjectTranslation();
  G4ThreeVector transVectFrm= fPVolTarget->GetFrameTranslation();
  //  G4cout << "transVectOld= " << transVect    << G4endl;
  //  G4cout << "transVectObj= " << transVectObj << G4endl;
  //  G4cout << "transVectFrm= " << transVectFrm << G4endl;
  transVectObj.setZ(zpos);
  fPVolTarget->SetTranslation(transVectObj);

  // Tell G4RunManager that we change the geometry
  G4RunManager::GetRunManager()->GeometryHasBeenModified();

  G4cout << "SetTargetThickness: Changed fTgtThickScaled to be "
	 << G4BestUnit(fTgtThickScaled,"Length")
	 << G4endl;
  PrintTargetParam();
}

void
Geometry::PrintTargetParam()
{
  G4cout << "PrintTargetParam"
	 << ": fTgtNum= "           << fTgtNum
	 << ", fTgtThickScl= "      << fTgtThickScl
	 << ", fTgtThickUnscaled= " << G4BestUnit(fTgtThickUnscaled,"Length")
	 << ", fTgtThickScaled= "   << G4BestUnit(fTgtThickScaled  ,"Length")
	 << G4endl;
  G4Box* solid = (G4Box*)fPVolTarget->GetLogicalVolume()->GetSolid();
  G4Material* material = fPVolTarget->GetLogicalVolume()->GetMaterial();
  G4cout << "PrintTargetParam"
	 << ": material= "  << material->GetName()
	 << ", density= "   << material->GetDensity()
	 << ", xlen/2= "    << G4BestUnit(solid->GetXHalfLength(),"Length")
	 << ", ylen/2= "    << G4BestUnit(solid->GetYHalfLength(),"Length")
	 << ", zlen/2= "    << G4BestUnit(solid->GetZHalfLength(),"Length")
	 << ", transVect= " << G4BestUnit(fPVolTarget->GetTranslation(),"Length")
	 << G4endl;
}

void
Geometry::SetMylarThickness( G4double val )
{
  fMylThick = val;

  G4Tubs* tube = (G4Tubs*)fPVolMylWin->GetLogicalVolume()->GetSolid();
  tube->SetZHalfLength(fMylThick/2);

  // Tell G4RunManager that we change the geometry
  G4RunManager::GetRunManager()->GeometryHasBeenModified();

  G4cout << "Parameter changed: mylar_thickness= "
	 << G4BestUnit(GetMylarThickness(),"Length")
	 << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

void
Geometry::DefineMaterials()
{
  // Use G4-NIST materials data base
  G4NistManager* mateMgr = G4NistManager::Instance();
  fMateDefault = mateMgr->FindOrBuildMaterial("G4_Galactic");
  fMatePb      = mateMgr->FindOrBuildMaterial("G4_Pb");
  fMateAl      = mateMgr->FindOrBuildMaterial("G4_Al");
  fMateMylar   = mateMgr->FindOrBuildMaterial("G4_MYLAR");
  fMateKapton  = mateMgr->FindOrBuildMaterial("G4_KAPTON");
  fMateScint   = mateMgr->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  fMateSiO2    = mateMgr->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

  G4Element* Si  = mateMgr->FindOrBuildElement("Si");
  G4Element* O   = mateMgr->FindOrBuildElement("O");
  G4int    nel;    // nel=number of elements;
  G4int    natoms; // natoms=number of atoms

  fMateTgtSP = fMateSiO2; // From NIST data base.
  fMateTgtAl = fMateAl;   // From NIST data base.
  //fMateDegrader = fMateKapton;
  fMateDegrader = fMateAl;
  StainlessSteel =  mateMgr->FindOrBuildMaterial("G4_STAINLESS-STEEL");


  //SUS
  // Use NIST database for elements and materials whereever possible.
  //G4NistManager* man = G4NistManager::Instance();
  //mateMgr->SetVerbose(1);
  // Define elements from NIST
  mateMgr->FindOrBuildElement("H");
  mateMgr->FindOrBuildElement("Be");
  G4Element* C  = mateMgr->FindOrBuildElement("C");
  mateMgr->FindOrBuildElement("N");
  //mateMgr->FindOrBuildElement("O");
  mateMgr->FindOrBuildElement("Al");
  //G4Element* Si = mateMgr->FindOrBuildElement("Si");
  mateMgr->FindOrBuildElement("Ti");
  G4Element* Cr = mateMgr->FindOrBuildElement("Cr");
  G4Element* Mn = mateMgr->FindOrBuildElement("Mn");
  G4Element* Fe = mateMgr->FindOrBuildElement("Fe");
  G4Element* Ni = mateMgr->FindOrBuildElement("Ni");
  mateMgr->FindOrBuildElement("W");
  mateMgr->FindOrBuildElement("Au");
  mateMgr->FindOrBuildElement("Pb");
  // Define pure NIST materials
  //mateMgr->FindOrBuildMaterial("G4_Al");
  mateMgr->FindOrBuildMaterial("G4_Ti");
  mateMgr->FindOrBuildMaterial("G4_W");
  mateMgr->FindOrBuildMaterial("G4_Au");
  // Define other NIST materials
  mateMgr->FindOrBuildMaterial("G4_WATER");
  //mateMgr->FindOrBuildMaterial("G4_KAPTON");
  G4Material* Air = mateMgr->FindOrBuildMaterial("G4_AIR");
  // Define materials not in NIST
  G4double density;
  G4int ncomponents;
  G4double fractionmass;
  /*
  StainlessSteel = new G4Material("StainlessSteel", density= 8.06*g/cm3, ncomponents=6);
  StainlessSteel->AddElement(C, fractionmass=0.001);
  StainlessSteel->AddElement(Si, fractionmass=0.007);
  StainlessSteel->AddElement(Cr, fractionmass=0.18);
  StainlessSteel->AddElement(Mn, fractionmass=0.01);
  StainlessSteel->AddElement(Fe, fractionmass=0.712);
  StainlessSteel->AddElement(Ni, fractionmass=0.09);
*/

  //  fMateTgtSP = new G4Material("TgtSP", 2.32*g/cm3, nel=2);
  //  fMateTgtSP->AddElement(Si, natoms=1);
  //  fMateTgtSP->AddElement( O, natoms=2);
  //  fMateTgtAl = new G4Material("TgtAl", 2.70*g/cm3, nel=2); // Arroy?
  //  fMateTgtAl->AddElement(Al, natoms=1);
  //  fMateTgtAl->AddElement(??, natoms=?);

  //  G4double density = 0.023500*CLHEP::g/CLHEP::cm3; // Official record value.
  //  fPerfLyrMaterial = new G4Material("Aerogel1", density, nel=2);
  //  fPerfLyrMaterial->AddElement(Si, natoms=1);
  //  fPerfLyrMaterial->AddElement( O, natoms=2);

  std::vector<G4Material*> msrs;
  nel= 2;
  fMateMSR12_2a  = new G4Material("MSR12_2a" , 0.02350*g/cm3, nel);
  msrs.push_back(fMateMSR12_2a);
  fMateMSR12_2c  = new G4Material("MSR12_2c" , 0.02330*g/cm3, nel);
  msrs.push_back(fMateMSR12_2c);
  fMateMSR12_2d  = new G4Material("MSR12_2d" , 0.02330*g/cm3, nel);
  msrs.push_back(fMateMSR12_2d);
  fMateMSR12_3b  = new G4Material("MSR12_3b" , 0.02360*g/cm3, nel);
  msrs.push_back(fMateMSR12_3b);
  //fMateMSR12_3c  = new G4Material("MSR12_3c" , 0.02320*g/cm3, nel);
  fMateMSR12_3c  = new G4Material("MSR12_3c" , 0.03220*g/cm3, nel);
  msrs.push_back(fMateMSR12_3c);
  fMateMSR12_3d  = new G4Material("MSR12_3d" , 0.02360*g/cm3, nel);
  msrs.push_back(fMateMSR12_3d);
  fMateMSR08_2b3 = new G4Material("MSR08_2b3", 0.02600*g/cm3, nel);
  msrs.push_back(fMateMSR08_2b3);
  for( auto&& mate : msrs ){
    mate->AddElement(Si,natoms=1);
    mate->AddElement( O,natoms=2);
  }

  std::vector<G4Material*> pmsqs;
  nel= 2; // should be 3 for C.
  fMatePMSQ_LD   = new G4Material("PMSQ_LD"  , 0.05250*g/cm3, nel);
  pmsqs.push_back(fMatePMSQ_LD);
  fMatePMSQ_HD   = new G4Material("PMSQ_HD"  , 0.09964*g/cm3, nel);
  pmsqs.push_back(fMatePMSQ_HD);
  for( auto&& mate : pmsqs ){
    mate->AddElement(Si,natoms=1);
    mate->AddElement( O,natoms=2);
  }
}

//*** Geant4 Material Database ***
//-- http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html#g4matrdb
//  NIST compounds
//  =============================================================
//  Ncomp             Name      density(g/cm^3)  I(eV) ChFormula
//  =============================================================
//    2         G4_SILICON_DIOXIDE       2.32     139.2   SiO_2
//        14             1
//         8             2
//
//-- http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/ForApplicationDeveloper/html/Detector/material.html#recipes-for-building-elements-and-materials
//  // define a material from elements and/or others materials (mixture of mixtures)
//  density = 0.200*g/cm3;
//  G4Material* Aerog = new G4Material(name="Aerogel", density, ncomponents=3);
//  Aerog->AddMaterial(SiO2, fractionmass=62.5*perCent);
//  Aerog->AddMaterial(H2O , fractionmass=37.4*perCent);
//  Aerog->AddElement (elC , fractionmass= 0.1*perCent);
//
//  // define Elements
//  a = 1.01*g/mole;
//  G4Element* elH  = new G4Element(name="Hydrogen",symbol="H" , z= 1., a);
//
//  a = 12.01*g/mole;
//  G4Element* elC  = new G4Element(name="Carbon"  ,symbol="C" , z= 6., a);
//
//  a = 14.01*g/mole;
//  G4Element* elN  = new G4Element(name="Nitrogen",symbol="N" , z= 7., a);
//
//  a = 16.00*g/mole;
//  G4Element* elO  = new G4Element(name="Oxygen"  ,symbol="O" , z= 8., a);
//
//  a = 28.09*g/mole;
//  G4Element* elSi = new G4Element(name="Silicon", symbol="Si", z=14., a);
//
//  density = 2.200*g/cm3;
//  G4Material* SiO2 = new G4Material(name="quartz", density, ncomponents=2);
//  SiO2->AddElement(elSi, natoms=1);
//  SiO2->AddElement(elO , natoms=2);
//
//  // define a material from elements.   case 1: chemical molecule
//  density = 1.000*g/cm3;
//  G4Material* H2O = new G4Material(name="Water", density, ncomponents=2);
//  H2O->AddElement(elH, natoms=2);
//  H2O->AddElement(elO, natoms=1);
//
//  // define a material from elements.   case 2: mixture by fractional mass
//  density = 1.290*mg/cm3;
//  G4Material* Air = new G4Material(name="Air  "  , density, ncomponents=2);
//  Air->AddElement(elN, fractionmass=0.7);
//  Air->AddElement(elO, fractionmass=0.3);
//
//=============== G4_vacuum =================
//-- From PhysicalConstants.h
// G4double density, massfraction, nComponents, temperature, pressure;
//  density     = 2.376e-15*g/cm3;
//  pressure    = 1.e-19*pascal;
//  temperature = 300*kelvin;
//  G4Material* vacuum = new G4Material( "Galactic", density, nComponents=1,
//				       kStateGas, temperature, pressure );
//  vacuum-> AddMaterial( mateMgr->FindOrBuildMaterial("G4_AIR"),
//		        massfraction=1.0 );
//  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

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
  //fPVolAlPipe  = GeomAlPipe(1);//done
  //fPVolCollim1 = GeomCollimator1(2);//done
  //fPVolCollim2 = GeomCollimator2(3);//done
  //fPVolAlHead  = GeomAlHead(4);//done
  //fPVolMylWin  = GeomMylarWindow(5);//tubs//done
  //fPVolVetoCnt = GeomVC(6);//done
  //fPVolMylWrps = GeomMylarWraps(7,2);//box//done
  //fPVolBeamCnt = GeomBC(7);//change to SUS

  fPVolDegrader  = GeomDG2 (8);
  fPVolDegrader  = GeomDG (9);
  fPVolTarget  = GeomTgt(10);
  PrintTargetParam();

  /*
  switch( fTgtNum ){
  case 1:  fPVolTarget = GeomS08(10); break;
  default: fPVolTarget = GeomSP(10);
  }
  */
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
  G4Material* material = fMateDefault;

  volumeID[name]= id;
  G4Box* solid = new G4Box(name,xlen/2,ylen/2,zlen/2);
  return SetPhysVol(name,solid,material,nullptr,G4ThreeVector(0,0,zpos));
  //  G4Box( const G4String& pName, G4double pX, G4double pY, G4double pZ );
}

G4VPhysicalVolume*
Geometry::GeomCollimator1( const G4int& id )
{
  const G4String& name = "PbCollim1";
  const G4double& din  =  16*mm;
  const G4double& dout =  60*mm;
  const G4double& zlen =  50*mm;
  const G4double& zpos = -130*mm - zlen/2;
  //G4Material* material = fMatePb;
  G4Material* material = fMateDefault;

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
  const G4String& name = "PbCollim2";
  const G4double& din  =  12*mm;
  const G4double& dout =  60*mm;
  const G4double& zlen =  50*mm;
  const G4double& zpos = -80*mm - zlen/2;
  //G4Material* material = fMatePb;
  G4Material* material = fMateDefault;

  volumeID[name]= id;
  G4Tubs* solid= new G4Tubs(name,din/2,dout/2,zlen/2,0*deg,360*deg);
  return SetPhysVol(name,solid,material,fPVolWorld,G4ThreeVector(0,0,zpos));
}

G4VPhysicalVolume*
Geometry::GeomMylarWindow( const G4int& id )
{
  const G4String& name = "MylarWin";
  const G4double& din  =   0*mm;
  const G4double& dout =  32*mm;
  const G4double& zlen = fMylThick;
  const G4double& zpos = -65*mm - zlen/2;
  //G4Material* material = fMateMylar;
  G4Material* material = fMateDefault;

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
  //G4Material* material = fMateAl;
  G4Material* material = fMateDefault;

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
  //G4Material* material = fMateAl;
  G4Material* material = fMateDefault;

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
  //G4Material* material  = fMateScint;
  G4Material* material  = fMateDefault;

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
  //const G4String& name = "SUS";
  //const G4double& xlen = 18*mm;
  const G4double& xlen = 80*mm;
  //const G4double& ylen = 18*mm;
  const G4double& ylen = 80*mm;
  const G4double& zlen = fBCThick;
  const G4double& zpos = -50*mm;
  //G4Material* material = fMateScint;
  G4Material* material = fMateDefault;

  volumeID[name]= id;
  G4Box* solid = new G4Box(name,xlen/2,ylen/2,zlen/2);
  return SetPhysVol(name,solid,material,fPVolWorld,G4ThreeVector(0,0,zpos));
}

G4VPhysicalVolume*
Geometry::GeomDG2( const G4int& id )
{
  const G4String& name = "SUS";
  //const G4String& name = "SUS";
  //const G4double& xlen = 18*mm;
  const G4double& xlen = 80*mm;
  //const G4double& xlen = 300*mm;
  //const G4double& ylen = 18*mm;
  const G4double& ylen = 80*mm;
  //const G4double& ylen = 300*mm;

  //const G4double& zlen = 0.05*mm;
  const G4double& zlen = 0.1*mm;
  const G4double& zpos = -153*mm;
  //const G4double& zpos = -150*mm;
  //G4Material* material = fMateScint;

  //G4Material* material = StainlessSteel;
  G4Material* material = fMateKapton;
  //G4Material* material = fMateAl;

  //G4Material* material = fMateDefault;

  //fMateDegrader = fMateKapton;
  //fMateDegrader = fMateAl;

  volumeID[name]= id;
  G4Box* solid = new G4Box(name,xlen/2,ylen/2,zlen/2);
  return SetPhysVol(name,solid,material,fPVolWorld,G4ThreeVector(0,0,zpos));
}

G4VPhysicalVolume*
Geometry::GeomDG( const G4int& id )
{
  const G4String& name = "DG";
  //const G4double xlen= 50.0*mm;

  //const G4double xlen= 43.0*mm;
  const G4double xlen= 300.0*mm;

  //const G4double xlen= 78.0*mm;
  //const G4double xlen= 200.0*mm;

  //const G4double ylen= 50.0*mm;

  //const G4double ylen= 35.0*mm;
  const G4double ylen= 300.0*mm;

  //const G4double ylen= 78.0*mm;
  //const G4double ylen= 200.0*mm;

  //const G4double& xlen = 18*mm;
  //const G4double& ylen = 18*mm;
  //const G4double& zlen = 150e-3*mm;
  //const G4double& zlen = 500e-3*mm;
  //
  //const G4double& zlen = 475e-3*mm;
  //const G4double& zlen = 450e-3*mm;
  //const G4double& zlen = 275e-3*mm;

  // Previously for the 20 mm and 23 mm
  //const G4double& zlen = 390e-3*mm;

  //const G4double& zlen =370e-3*mm;
  const G4double& zlen =fDGthickness*1e-3*mm;


  //const G4double& zlen = 550e-3*mm;
  //const G4double& zlen = 525e-3*mm;
  //const G4double& zlen = 600e-3*mm;
  //const G4double& zlen = 575e-3*mm;

  //const G4double& zpos = fDGPosZ;//-zlen/2 - 0.098*mm/2;
  //const G4double& zpos = -zlen/2 - 10.0*mm;
  //const G4double& zpos = -21.0*mm;
  const G4double& zpos = -10.0*mm;

  //const G4double& zpos = fBCPosZ;
  G4Material* material = fMateDegrader;
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
  //G4Material* material = fMateMylar;
  G4Material* material = fMateDefault;

  volumeID[name] = id;
  G4Box* solid = new G4Box(name,xlen/2,ylen/2,zlen/2);
  G4LogicalVolume* logical= new G4LogicalVolume(solid,material,name);

  G4String names[ncopy];
  for( G4int icopy=0; icopy<ncopy; icopy++ ){
    names[icopy] = name + std::to_string(icopy);
    G4cout<<names<<G4endl;
    volumeID[names[icopy]]= id + icopy;
  }

  G4double zposs[ncopy];
  zposs[0] = 5 - fBCThick/2 - zlen/2;
  zposs[1] = 5 + fBCThick/2 + zlen/2;

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
Geometry::GeomTgt( const G4int& id )
{
  const G4String& name = "Target";

  G4Material* material = nullptr;
  G4double zlen= 0;
  switch( fTgtNum ){
  //case  0: material= fMateTgtSP; zlen= 0.096*mm; break; // zlen: +-0.01 mm tol.
  case  0: material= fMateTgtSP; zlen= 7*mm; break; // zlen: +-0.01 mm tol.
  case 27: material= fMateTgtAl; zlen= 0.075*mm; break; // zlen: +-10% toler.
  case  1: case  2: case  4:
  case  5: material= fMateMSR12_3b;  zlen= 8.8*mm; break;
  case  3: case  6: case  7:
  //case  8: material= fMateMSR12_2a;  zlen= 8.8*mm; break;
  case  8: material= fMateMSR12_2a;  zlen= 7.12*mm; break;
  case  9:
  case 10: material= fMateMSR12_2d;  zlen= 8.9*mm; break;
  case 11: case 12: case 13:
  case 14: material= fMateMSR12_3d;  zlen= 8.9*mm; break;
  case 15:
  case 16: material= fMatePMSQ_LD;   zlen= 4.0*mm; break;
  case 17:
  case 25: material= fMatePMSQ_HD;   zlen= 2.0*mm; break;
  case 18: case 19: case 20:
  //case 21: material= fMateMSR12_3c;  zlen= 20.0*mm; break;
  case 21: material= fMateMSR12_3c;  zlen= 7.12*mm; break;
  //case 21: material= fMateMSR12_3c;  zlen= 10*mm; break;
  //case 21: material= fMateMSR12_3c;  zlen= 12*mm; break;
  //case 21: material= fMateMSR12_3c;  zlen= 14*mm; break;
  //case 21: material= fMateMSR12_3c;  zlen= 20*mm; break;
  case 22: case 23:
  case 24: material= fMateMSR12_2c;  zlen= 8.7*mm; break;
  case 26: material= fMateMSR08_2b3; zlen= 8.0*mm; break; // S2013-1
  default:
    G4cout << "Unexpected TgtNum: " << fTgtNum << G4endl;
  }
  fTgtThickUnscaled = zlen;
  fTgtThickScaled   = fTgtThickUnscaled * fTgtThickScl;
  zlen = fTgtThickScaled;


  //case 21: material= fMateMSR12_3c;  zlen= 8.8*mm; break;
  //case 21: material= fMateMSR12_3c;  zlen= 7.12*mm; break;
  //case 21: material= fMateMSR12_3c;  zlen= 10*mm; break;
  //case 21: material= fMateMSR12_3c;  zlen= 12*mm; break;
  //case 21: material= fMateMSR12_3c;  zlen= 14*mm; break;
  //case 21: material= fMateMSR12_3c;  zlen= 20*mm; break;

  //G4double xlen= 50.0*mm;

  //G4double xlen= 43.0*mm;
  G4double xlen= 300.0*mm;

  //G4double xlen= 78.0*mm;
  //G4double xlen= 200.0*mm;

  //G4double ylen= 50.0*mm;

  //G4double ylen= 35.0*mm;
  G4double ylen= 300.0*mm;

  //G4double ylen= 78.0*mm;
  //G4double ylen= 200.0*mm;

  if( fTgtNum == 0 ){ // Silica plate
    xlen = 30.0*mm; // xlen: +-0.1 mm tolerance.
    ylen = 42.0*mm; // ylen: +-0.1 mm tolerance.
  }

  //const G4double& ypos = -(35-ylen)/2;
  const G4double& zpos = -zlen/2;

  volumeID[name]= id;
  G4Box* solid = new G4Box(name,xlen/2,ylen/2,zlen/2);
  return SetPhysVol(name,solid,material,fPVolWorld,G4ThreeVector(0,0,zpos));
}

//*** Target property (from msr12 13_aerogel_data_20161003.pdf)
//-- For each aerogel
//AerogelID Tthickness Density Samples
//             [mm]  [g/cm^3]
//Silica Plate 0.096 2.32    SP1, SP2
//MSR12-3b     8.8   0.0236  S01, S02, S04, S05
//MSR12-2a     8.8   0.0235  S03, S06, S07, S08
//MSR12-2d     8.9   0.0233  S09, S10
//MSR12-3d     8.9   0.0236  S11, S12, S13, S14
//PMSQ-green   4     0.0525  S15, S16
//PMSQ-red     2     0.09964 S17, S25
//MSR12-3c     8.8   0.0232  S18, S19, S20, S21
//MSR12-2c     8.7   0.0233  S22, S23, S24
//MSR8-2b-3    8     0.026   S2013-1, a.k.a PTEP2014-S3
//Aluminium    0.075 2.7     Al
//MSR8-1c-2    7.6   0.029   PTEP2014-S1
//MSR2-1b-1    6.5   0.037   PTEP2014-S2
//MSR8-2c-3    ?     0.026   PTEP2014-S4
//Unknown      6.9   0.029   PTEP2013-S1
//Unknown      4.8   0.047   PTEP2013-S2
//Unknown      2.2   0.097   PTEP2013-S3
//Unknown      2     0.178   PTEP2013-S4
//
//-- For each target
//Sample Tthickness Density AerogelID Ablation
//            [mm] [g/cm^3]
//From S1249-2017 runs
//SP           0.096 2.32
//S01	       8.8   0.0236  MSR12-3b   hole
//S02	       8.8   0.0236  MSR12-3b   hole
//S03	       8.8   0.0235  MSR12-2a   hole
//S04	       8.8   0.0236  MSR12-3b   hole
//S05	       8.8   0.0236  MSR12-3b   hole
//S06	       8.8   0.0235  MSR12-2a   hole
//S07	       8.8   0.0235  MSR12-2a   hole
//S08	       8.8   0.0235  MSR12-2a   flat
//S09	       8.9   0.0233  MSR12-2d   hole
//S10	       8.9   0.0233  MSR12-2d   hole
//S11	       8.9   0.0236  MSR12-3d   hole
//S12	       8.9   0.0236  MSR12-3d   hole
//S13	       8.9   0.0236  MSR12-3d   hole
//S14	       8.9   0.0236  MSR12-3d   continuous
//S15	       4     0.0525  PMSQ-green flat
//S16	       4     0.0525  PMSQ-green hole
//S17	       2     0.09964 PMSQ-red   flat
//S18	       8.8   0.0232  MSR12-3c   hole
//S19	       8.8   0.0232  MSR12-3c   hole
//S20	       8.8   0.0232  MSR12-3c   hole
//S21	       8.8   0.0232  MSR12-3c   channel
//S22	       8.7   0.0233  MSR12-2c   channel
//S23	       8.7   0.0233  MSR12-2c   hole_enhaneced-cone
//S24	       8.7   0.0233  MSR12-2c   channel_tapered
//S25	       2     0.09964 PMSQ-red   hole
//S2013-1      8     0.026   MSR8-2b-3  hole (PTEP2014-S3)
//Al           0.075 2.7
//
//From PTEP 091C01 (2014)
//PTEP2014-S1  7.6   0.029   MSR8-1c-2  hole (density = 0.028?)
//PTEP2014-S2  6.5   0.037   MSR2-1b-1  hole
//PTEP2014-S3  8     0.026   MSR8-2b-3  hole
//PTEP2014-S4  ?     0.026   MSR8-2c-3  hole
//
//From PTEP 103C01 (2013)
//PTEP2013-S1  6.9   0.029   unknown    flat
//PTEP2013-S2  4.8   0.047   unknown    flat
//PTEP2013-S3  2.2   0.097   unknown    flat
//PTEP2013-S4  2     0.178   unknown    flat

G4VPhysicalVolume*
Geometry::GeomSP( const G4int& id )
{
  //  const G4String& name = "SP";
  const G4String& name = "Target";
  //  const G4double& xlen = 30.0*mm;  // Vendor specification with the tolerance of +-0.1 mm.
  //  const G4double& ylen = 42.0*mm;  // Vendor specification with the tolerance of +-0.1 mm.
  //  const G4double& zlen = 0.096*mm; // Vendor specification with the tolerance of +-0.01 mm.
  const G4double& xlen = 30*mm;  // Default assumption.
  const G4double& ylen = 40*mm;  // Default assumption.
  const G4double& zlen = 0.1*mm; // Default assumption.
  const G4double& zpos = -zlen/2;
  const G4double& ypos = -5*mm; // = (50-ylen)/2
  G4Material* material = fMateTgtSP;

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
  const G4double& zlen = 8.8*mm; // Official record value.
  const G4double& zpos = -zlen/2;
  //  G4Material* material = fPerfLyrMaterial;
  G4Material* material = fMateMSR12_2a; // MSR12-2a

  volumeID[name]= id;
  G4Box* solid = new G4Box(name,xlen/2,ylen/2,zlen/2);
  return SetPhysVol(name,solid,material,fPVolWorld,G4ThreeVector(0,0,zpos));
}
