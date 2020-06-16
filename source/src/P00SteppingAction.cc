//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// P00SteppingAction.cc
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "P00EventAction.hh"
#include "P00SteppingAction.hh"
#include "P00RunData.hh"
#include "P00Analysis.hh"
#include "Geometry.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"

#include <float.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

P00SteppingAction::P00SteppingAction(G4UserEventAction* eventAction)
  : G4UserSteppingAction(), fEvtAct((P00EventAction*)eventAction)
{}

P00SteppingAction::~P00SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

void P00SteppingAction::PrintTrackInfo( const G4Track* track, const G4String& tag="Track:" )
{ // track/include/G4Track.hh

  G4int trkID    = track->GetTrackID(); // =1 for the primary particle.
  G4int trkID_ma = track->GetParentID();
  G4TrackStatus status= track->GetTrackStatus();
  //-- track/include/G4TrackStatus.hh
  //  enum G4TrackStatus
  //  {
  //     fAlive,                   // =0: Continue the tracking
  //     fStopButAlive,            // =1: Invoke active rest physics processes and kill the current track afterward
  //     fStopAndKill,             // =2: Kill the current track
  //     fKillTrackAndSecondaries, // =3: Kill the current track and associated secondaries.
  //     fSuspend,                 // =4: Suspend the current track
  //     fPostponeToNextEvent      // =5: Postpones the tracking of the current track to the next event.
  //   };

  const G4ParticleDefinition* ptclDef= track->GetParticleDefinition();
  G4String ptclName    = ptclDef->GetParticleName();
  G4double properTime  = track->GetProperTime(); // Proper time of the current track
  G4ThreeVector polVct = track->GetPolarization();

  G4double gblTime     = track->GetGlobalTime(); // Time since this event is created.
  G4double locTime     = track->GetLocalTime();  // Time since the current track is created.
  G4double length      = track->GetTrackLength();
  G4double velocity    = track->GetVelocity();
  G4double totE        = track->GetTotalEnergy();
  G4double kinE        = track->GetKineticEnergy();
  G4ThreeVector trkPos = track->GetPosition();
  G4ThreeVector momVct = track->GetMomentum();
  G4ThreeVector momDir = track->GetMomentumDirection();

  const G4VProcess* creProc = track->GetCreatorProcess();
  G4String creProcName("n/a");
  if( creProc ) creProcName = creProc->GetProcessName();
  G4String creModeName      = track->GetCreatorModelName();
  G4int creModeID           = track->GetCreatorModelID();

  G4cout << tag
	 << " trkID= "        << trkID
	 << ", trkID_ma= "    << trkID_ma
	 << ", status= "      << status
	 << ", ptclName= "    << ptclName
	 << ", polVct= "      << polVct
	 << G4endl;
  
  const G4Step* aStep= track->GetStep();
  G4int stepNum      = track->GetCurrentStepNumber();
  G4double stepLen   = track->GetStepLength();

  G4cout << tag
	 << " stepNum= "  << stepNum
	 << ", stepLen= " << G4BestUnit(stepLen,"Length");

  //-- Accessor for the gometry information
  G4VPhysicalVolume* pvol= track->GetVolume();
  G4cout << ", pvol= " << pvol->GetName();

  G4cout << "; next= ";
  G4VPhysicalVolume* pvol_next= 0;
  if( (pvol_next= track->GetNextVolume()) ){
    // Access the next volume
    G4cout << pvol_next->GetName();
  }else{
    G4cout << "n/a";
  }
 
  G4LogicalVolume* lvol= pvol->GetLogicalVolume();
  G4VPhysicalVolume* pvol_dtr= 0;
  for( G4int i=0; i<lvol->GetNoDaughters(); i++ ){
    // Access the daughter volumes
    if( !(pvol_dtr= lvol->GetDaughter(i)) ) continue;
    G4String space(", ");
    if( i == 0 ) space= "; ";
    G4cout << space << "dtr" << i << "= " << pvol_dtr->GetName();
  }
  G4cout << G4endl;

  //-- Accessor for the material information
  //  G4Material* mate= track->GetMaterial();    
  //  G4cout << tag << mate << G4endl;
  //  G4Material* mate_next= 0;
  //  if( (mate_next = track->GetNextMaterial()) ) G4cout << tag << mate << G4endl;

  //-- Track production vertex information available in G4Track
  G4ThreeVector prodVtx  = track->GetVertexPosition(); // Track production vertex.
  G4ThreeVector momDir_pv= track->GetVertexMomentumDirection();
  G4double kinE_pv       = track->GetVertexKineticEnergy();
  const G4LogicalVolume* lvol_pv= track->GetLogicalVolumeAtVertex();

  G4cout << tag
	 << " prodVtx= "       << G4BestUnit(prodVtx,"Length")
	 << ", momDir_pv= "    << momDir_pv
	 << ", kinE_pv= "      << G4BestUnit(kinE_pv,"Energy")
	 << ", lvol_pv_name= " << lvol_pv->GetName()
	 << G4endl;

  G4cout << tag
	 << " creProcName= "  << creProcName
	 << ", creModeName= " << creModeName
	 << ", creModeID= "   << creModeID
	 << G4endl;
  G4cout << tag
	 << " properTime= "   << G4BestUnit(properTime,"Time")
	 << ", gblTime= "     << G4BestUnit(gblTime,"Time")
	 << ", locTime= "     << G4BestUnit(locTime,"Time")
	 << ", length= "      << G4BestUnit(length,"Length")
	 << ", velocity= "    << velocity * ns/mm << " mm/ns"
	 << ", totE= "        << G4BestUnit(totE,"Energy")
	 << ", kinE= "        << G4BestUnit(kinE,"Energy")
	 << G4endl;
  G4cout << tag
	 << " trkPos= "       << G4BestUnit(trkPos,"Length")
	 << ", momVct= "      << momVct / MeV << " MeV/c"
	 << ", momDir= "      << momDir
	 << G4endl;

  //  const G4DynamicParticle* GetDynamicParticle() const;
  //  const G4MaterialCutsCouple* GetMaterialCutsCouple() const;
  //  const G4MaterialCutsCouple* GetNextMaterialCutsCouple() const;
  //  const G4VTouchable*      GetTouchable() const;
  //  const G4TouchableHandle& GetTouchableHandle() const;
  //  const G4VTouchable*      GetNextTouchable() const;
  //  const G4TouchableHandle& GetNextTouchableHandle() const;
  //  const G4VTouchable*      GetOriginTouchable() const;
  //  const G4TouchableHandle& GetOriginTouchableHandle() const;
  //  G4double GetWeight() const;
  //  G4VUserTrackInformation* GetUserInformation() const;
  //  void SetUserInformation(G4VUserTrackInformation* aValue) const;
  //  static G4double GetMaxTOfVelocityTable();
  //  static G4double GetMinTOfVelocityTable();
  //  static G4int    GetNbinOfVelocityTable();
  //  G4VAuxiliaryTrackInformation* GetAuxiliaryTrackInformation(G4int idx) const;
  //  std::map<G4int,G4VAuxiliaryTrackInformation*>* GetAuxiliaryTrackInformationMap() const
  //  { return fpAuxiliaryTrackInformationMap; }  
}

void P00SteppingAction::PrintStepInfo( const G4Step* step, const G4String& tag="Step:" )
{ // track/include/G4Step.hh

  G4double depoE_tot = step->GetTotalEnergyDeposit();
  G4double depoE_ni  = step->GetNonIonizingEnergyDeposit();
  G4double length    = step->GetStepLength();
  G4double deltaT    = step->GetDeltaTime();
  G4ThreeVector deltaPos = step->GetDeltaPosition();

  G4cout << tag
	 << " deltaT= "     << G4BestUnit(deltaT,"Time")
	 << ", deltaL= "    << G4BestUnit(length,"Length")
	 << ", depoE_tot= " << G4BestUnit(depoE_tot,"Energy")
	 << ", depoE_ni= "  << G4BestUnit(depoE_ni,"Energy")
	 << ", deltaPos= "  << G4BestUnit(deltaPos,"Length")
	 << G4endl;

  //  G4StepPoint* pnt1 = step->GetPreStepPoint();
  //  G4StepPoint* pnt2 = step->GetPostStepPoint();
  //  if( pnt1 ) PrintStepPointInfo(pnt1,"    prePoint:");
  //  if( pnt2 ) PrintStepPointInfo(pnt2,"    pstPoint:");
  
  // cotrole flag for stepping
  //  G4SteppingControl GetControlFlag() const;
  //-- track/include/G4SteppingControl.h
  //   enum G4SteppingControl
  //   {
  //     NormalCondition,
  //     AvoidHitInvocation, // Hit will NOT be called
  //     Debug
  //   };
  //
  // NOTE: following flags are not used. Will be ready in later release
  //   G4bool IsFirstStepInVolume() const;
  //   G4bool IsLastStepInVolume() const;
  //
  // secodaries in the current step
  //   G4int GetNumberOfSecondariesInCurrentStep() const;
  //   const std::vector<const G4Track*>* GetSecondaryInCurrentStep() const;
  //
  // NOTE: Secondary bucket of the Step contains
  //       all secondaries during tracking the current track
  //       (i.e. NOT secondaries produced in the current step)
  // all following methods give same object (i.e. G4TrackVector  )
  // but 2nd one will create bucket in addition
  //  const G4TrackVector* GetSecondary() const ;
  //  G4TrackVector* GetfSecondary();
  //  G4TrackVector* NewSecondaryVector();
  //  std::vector<G4ThreeVector>* GetPointerToVectorOfAuxiliaryPoints() const {
  //    return fpVectorOfAuxiliaryPointsPointer;
  //  }
}

void P00SteppingAction::PrintStepPointInfo( const G4StepPoint* point,
					    const G4String& tag="Point:" )
{ // track/include/G4StepPoint.hh

  G4TouchableHandle touch = point->GetTouchableHandle();
  if( !touch ){
    G4cout << "Null pointer for touch." << G4endl;
  }
  
  G4StepStatus status = point->GetStepStatus();
  //-- track/include/G4StepStatus.hh
  //   enum G4StepStatus
  //   {
  //     fWorldBoundary,     // =0: Step reached the world boundary
  //     fGeomBoundary,      // =1: Step defined by a geometry boundary
  //     fAtRestDoItProc,    // =2: Step defined by a PreStepDoItVector
  //     fAlongStepDoItProc, // =3: Step defined by a AlongStepDoItVector
  //     fPostStepDoItProc,  // =4: Step defined by a PostStepDoItVector
  //     fUserDefinedLimit,  // =5: Step defined by the user Step limit in the logical volume
  //     fExclusivelyForcedProc, //=6: Step defined by an exclusively forced PostStepDoIt process
  //     fUndefined          //=7: Step not defined yet
  //   };

  G4double mass       = point->GetMass();
  G4double charge     = point->GetCharge();
  G4double properTime = point->GetProperTime();    // Proper time of the particle.
  G4double magmom     = point->GetMagneticMoment();
  G4ThreeVector polVct= point->GetPolarization();

  G4double gblTime    = point->GetGlobalTime();    // Time since this event is created.
  G4double locTime    = point->GetLocalTime();     // Time since the track is created.
  G4double velocity   = point->GetVelocity();
  G4double beta       = point->GetBeta();          // Velocity of the track in unit of c.
  G4double gamma      = point->GetGamma();         // Gamma factor (1/sqrt[1-beta*beta]).
  G4double totE       = point->GetTotalEnergy();   // Total energy of the track.
  G4double kinE       = point->GetKineticEnergy(); // Kinetic energy of the track.
  G4ThreeVector wldPos= point->GetPosition();
  G4ThreeVector locPos= touch->GetHistory()->GetTopTransform().TransformPoint(wldPos);
  G4ThreeVector momVct= point->GetMomentum();      // Total momentum of the track.
  G4ThreeVector momDir= point->GetMomentumDirection(); // Momentum direction (unit vector)

  const G4VProcess* aProcess = point->GetProcessDefinedStep();
  G4String procName("UserDefinedLimit");
  G4int procType= -1;
  G4int procSubType= -1;
  if( aProcess ){
    procName= aProcess->GetProcessName();
    procType= (G4int)aProcess->GetProcessType();
    procSubType= aProcess->GetProcessSubType();
  }
  // If the pointer is 0, this means the Step is defined
  // by the user defined limit in the current volume.

  //-- processes/management/include/G4ProcessType.hh
  //  enum G4ProcessType
  //  { 
  //    fNotDefined,
  //    fTransportation,
  //    fElectromagnetic,
  //    fOptical, 
  //    fHadronic,
  //    fPhotolepton_hadron,
  //    fDecay,
  //    fGeneral,
  //    fParameterisation,
  //    fUserDefined,
  //    fParallel,
  //    fPhonon,
  //    fUCN
  //  };

  G4cout << tag
	 << " status= "      << status
	 << ", procName= "   << procName
	 << ", procType= "   << procType
	 << ", procSubType= "<< procSubType
    //	 << ", mass= "       << G4BestUnit(mass,"Mass")
    	 << ", mass= "       << mass / MeV << " MeV/c"
	 << ", charge= "     << charge
	 << ", properTime= " << G4BestUnit(properTime,"Time")
	 << ", magmom= "     << magmom
	 << ", polVct= "     << polVct
	 << G4endl;

  //-- Accessor for the gometry information
  G4VPhysicalVolume* pvol= point->GetPhysicalVolume();
  //  G4VPhysicalVolume* pvol = touch->GetVolume();
  G4LogicalVolume* lvol = 0;
  G4String volname("n/a");
  G4Region* region = 0;
  if( pvol ){
    volname = pvol->GetName();
    lvol    = pvol->GetLogicalVolume();
    region  = lvol->GetRegion();
  }
  G4int depth   = touch->GetHistoryDepth();
  G4int copynum = touch->GetCopyNumber();

  G4cout << tag
	 << " pvol= "     << volname
    	 << ", depth= "   << depth
    	 << ", copynum= " << copynum;

  // The following usage results in a segmentation violation
  // at the first line and a null pointer check does not work.
  //  G4VPhysicalVolume* pvol_ma = touch->GetVolume(1);
  //  if( pvol_ma ) copynum_ma= touch->GetCopyNumber(1);

  G4String maTag("ma");
  for( G4int idepth= 1, igrand= 0; idepth<=depth; idepth++, igrand++ ){
    if( igrand > 0 ) maTag = "g" + maTag;
    G4cout << "; " << maTag << "= " << touch->GetVolume(idepth)->GetName()
	   << ", depth= " << depth - idepth
	   << ", copynum= " << touch->GetCopyNumber(idepth);
  }
  G4cout << G4endl;
  
  //-- Accessor for the material information
  //  G4Material* mate = point->GetMaterial();
  //    G4Material* mate = lvol->GetMaterial();
  //  if( mate ) PrintMaterialInfo(mate,"      Mate:");

  G4cout << tag
	 << " gblTime= "     << G4BestUnit(gblTime,"Time")
	 << ", locTime= "    << G4BestUnit(locTime,"Time")
	 << ", velocity= "   << velocity * ns/mm << " mm/ns"
	 << ", beta= "       << beta
	 << ", gamma= "      << gamma
	 << ", totE= "       << G4BestUnit(totE,"Energy")
	 << ", kinE= "       << G4BestUnit(kinE,"Energy")
	 << G4endl;
  G4cout << tag
	 << " wldPos= "      << G4BestUnit(wldPos,"Length")
	 << ", locPos= "     << G4BestUnit(locPos,"Length")
	 << ", momVct= "     << momVct / MeV << " MeV/c"
	 << ", momDir="      << momDir
	 << G4endl;

  //  const G4MaterialCutsCouple* GetMaterialCutsCouple() const;
  //  G4VSensitiveDetector* GetSensitiveDetector() const;
  //  G4double GetSafety() const;
  //  G4double GetWeight() const;
}

void P00SteppingAction::PrintMaterialInfo( const G4Material* mate, const G4String& tag="Mate:" )
{ // material/include/G4Material.hh
  
  G4double Z= -1;
  G4double A= -1;
  G4String chemForm("Undefined");
  size_t nElem = mate->GetNumberOfElements(); // # of elements of this material:
  if( nElem == 1 ){
    // These function calls are meaningful only for single material.
    Z = mate->GetZ();
    A = mate->GetA();
  }else if( mate->GetChemicalFormula() != "" ){
    chemForm = mate->GetChemicalFormula();
  }
  G4String mateName   = mate->GetName();
  G4double molecMass  = mate->GetMassOfMolecule(); // for chemical compound
  G4State  state      = mate->GetState();
  //  enum G4State { kStateUndefined = 0, kStateSolid, kStateLiquid, kStateGas };
  G4double density    = mate->GetDensity();
  G4double temperature= mate->GetTemperature();
  G4double pressure   = mate->GetPressure();
  size_t   nMate      = mate->GetNumberOfMaterials();     // static function
  G4double nAtomVol   = mate->GetTotNbOfAtomsPerVolume(); // total # of atoms per volume:
  G4double nElecVol   = mate->GetTotNbOfElectPerVolume(); // total # of electrons per volume:
  G4double radLen     = mate->GetRadlen();                // radiation length:
  G4double nucIntLen  = mate->GetNuclearInterLength();    // nuclear interaction length
  G4IonisParamMat* ionisMat= mate->GetIonisation();       // ionisation parameters:

  G4cout << tag
	 << " mateName= "     << mateName
	 << ", nMate= "       << nMate
	 << ", nElem= "       << nElem
	 << ", chemForm= "    << chemForm
	 << ", Z= "           << Z
	 << ", A= "           << A
	 << ", molecMass= "   << molecMass * CLHEP::Avogadro / g  << " g/mole"
	 << G4endl;
  G4cout << tag
	 << " state= "        << state
    //	 << ", density= "     << G4BestUnit(density,"Volumic Mass")
    	 << ", density= "     << density * cm3/g << " g/cm3"
	 << ", temperature= " << G4BestUnit(temperature,"Temperature")
	 << ", pressure= "    << G4BestUnit(pressure,"Pressure")
	 << G4endl;
  G4cout << tag
	 << " nAtomVol= "     << nAtomVol * cm3 << " /cm3"
	 << ", nElecVol= "    << nElecVol * cm3 << " /cm3"
	 << ", radLen= "      << radLen / cm << " cm"
	 << ", nucIntLen= "   << nucIntLen / cm << " cm"
	 << G4endl;
  
  //  static const G4double NTP_Temperature = 293.15;
  //  G4SandiaTable* GetSandiaTable() const; // Sandia table:
  //  const G4Material* GetBaseMaterial() const; // Base material:
  //  const std::map<G4Material*,G4double>& GetMatComponents() const; // material components:
  //vector of pointers to elements constituing this material:
  //  const G4ElementVector* GetElementVector()  const;
  //vector of fractional mass of each element:
  //  const G4double* GetFractionVector() const;
  //vector of atom count of each element:
  //  const G4int* GetAtomsVector() const;
  //return a pointer to an element, given its index in the material:
  //  const G4Element* GetElement(G4int iel) const;
  //vector of nb of atoms per volume of each element in this material:
  //  const G4double* GetVecNbOfAtomsPerVolume() const;
  //  G4MaterialPropertiesTable* GetMaterialPropertiesTable() const;
  //  static size_t GetNumberOfMaterials();
  //  static G4MaterialTable* GetMaterialTable();
  //the index of this material in the Table:
  //  inline size_t GetIndex() const {return fIndexInTable;}
  //return  pointer to a material, given its name:
  //  static G4Material* GetMaterial(const G4String& name, G4bool warning=true);
  //  friend std::ostream& operator<<(std::ostream&, const G4Material*);
  //  friend std::ostream& operator<<(std::ostream&, const G4Material&);
  //  friend std::ostream& operator<<(std::ostream&, G4MaterialTable);
  //Fake default constructor for usage restricted to direct object
  //persistency for clients requiring preallocation of memory for
  //persistifiable objects.
  //  G4Material(__void__&);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

// Collect energy and track length step by step
void P00SteppingAction::UserSteppingAction(const G4Step* step)
{
  //  G4int debugLevel= 0;
  G4Track*     aTrack = step->GetTrack();
  G4StepPoint* prePnt = step->GetPreStepPoint();
  G4StepPoint* pstPnt = step->GetPostStepPoint();
  G4Material*  preMat = prePnt->GetMaterial();
  G4Material*  pstMat = pstPnt->GetMaterial();

  G4TrackStatus trkStatus = aTrack->GetTrackStatus();
  G4StepStatus  preStatus = prePnt->GetStepStatus();
  G4StepStatus  pstStatus = pstPnt->GetStepStatus();
  
  G4String particleName = aTrack->GetParticleDefinition()->GetParticleName();
  G4VPhysicalVolume* preVol = aTrack->GetVolume();
  G4VPhysicalVolume* pstVol = aTrack->GetNextVolume();

  const G4VProcess* preProc = prePnt->GetProcessDefinedStep();
  const G4VProcess* pstProc = pstPnt->GetProcessDefinedStep();
  
  G4String pstProcName("n/a");
  G4int pstProcType= -2;
  
  if( pstProc ){
    pstProcName= pstProc->GetProcessName();
    pstProcType= (G4int)pstProc->GetProcessType();
  }

  G4int stepNum= aTrack->GetCurrentStepNumber();

  auto runData = static_cast<P00RunData*>
    (G4RunManager::GetRunManager()->GetNonConstCurrentRun());

  if( particleName == "mu+" && aTrack->GetTrackID() == 1 ){

    //-- Generated.
    if( stepNum == 1 ){
      runData->hitStats = 1;
      runData->endStats = 1;
      runData->yldStats = 1;
      runData->SetAllVars(step,runData->hldGen);
    }

    //-- Check the hit geometries.
    if( pstStatus == fGeomBoundary ){
      //-- Note that the post-step point does not have the vaolume
      //   when its step status is "fWorldBoundary".
      if( pstVol ){
        const G4String& pstVolName = pstVol->GetName();
        G4int pstVolId = Geometry::volumeID[pstVolName];
        runData->SetBit(runData->hitStats,pstVolId);

        if( pstVolId ==  9 ){
      	  runData->evtLevel = 1;
          runData->SetBit(runData->yldStats,1);
        }
        else if( pstVolId == 10 ){
          if( runData->evtLevel == 1 ){
            runData->evtLevel = 2;
            runData->SetBit(runData->yldStats,2);
          }
        }
      }
      else{
        G4cout << "Undefined pstVol"
        << ": eventID= "   << fEvtAct->eventID
        << ", stepNum= "   << stepNum
        << ", trkStatus= " << trkStatus
        << ", preStatus= " << preStatus
        << ", pstStatus= " << pstStatus
        << G4endl;
      }
    }

    //-- The last step.
    if( trkStatus == fStopAndKill ){
      runData->SetAllVars(step,runData->hldEnd);

      //-- Check the end geometries.
      if( pstStatus == fWorldBoundary ){ // No post-step volume.
        runData->SetBit(runData->endStats,11);
      }
      else if( pstVol ){
        const G4String& pstVolName = pstVol->GetName();
        G4int pstVolId = Geometry::volumeID[pstVolName];
        if( pstVolId == 0 ){ // Ended in the world volume between two gometries.
          runData->SetBit(runData->endStats,12);
        }
        else{
          runData->SetBit(runData->endStats,pstVolId);
        }
      }
      else{

        G4cout << "Undefined pstVol"
        << ": eventID= "   << fEvtAct->eventID
        << ", stepNum= "   << stepNum
        << ", trkStatus= " << trkStatus
        << ", preStatus= " << preStatus
        << ", pstStatus= " << pstStatus
        << G4endl;
      }

      //-- Check the statistics for the evtLevel == 2.
      if( runData->evtLevel == 2 ){
        if( pstStatus == fWorldBoundary ){ // Reached at the WorldBoundary.
          runData->SetBit(runData->yldStats,7);
        }
        else if( pstVol ){
          const G4String& pstVolName = pstVol->GetName();
          G4int pstVolId = Geometry::volumeID[pstVolName];
          if( pstVolId == 10 ){      // Inside the target.
            if( pstProc && pstProc->GetProcessName() == "Decay" ){
              runData->evtLevel = 3;
              runData->SetBit(runData->yldStats,3);
      	    }
            else{
      	      runData->SetBit(runData->yldStats,4);
              G4cout << "yldStats= 4, pstProcName: "
              << pstProc->GetProcessName() << G4endl;
            }
          }
          else if( pstVolId == 0 ){ // Between the target and WorldBoundary.
      	    if( pstProc && pstProc->GetProcessName() == "Decay" ){
      	      runData->SetBit(runData->yldStats,5);
      	    }
            else{
      	      runData->SetBit(runData->yldStats,6);
      	      G4cout << "yldStats= 6, pstProcName: "<< pstProc->GetProcessName() << G4endl;
            }
      	  }
          else{
            G4cout << "Unexpected pstVol= " << pstVol
            << ": eventID= "   << fEvtAct->eventID
            << ", stepNum= "   << stepNum
            << ", trkStatus= " << trkStatus
            << ", preStatus= " << preStatus
            << ", pstStatus= " << pstStatus
            << G4endl;
          }
      	}
        else{
          G4cout << "Undefined pstVol"
          << ": eventID= "   << fEvtAct->eventID
          << ", stepNum= "   << stepNum
          << ", trkStatus= " << trkStatus
          << ", preStatus= " << preStatus
          << ", pstStatus= " << pstStatus
          << G4endl;
      	}
      }//if( runData->evtLevel == 2 )
    }//the last step, if( trkStatus == fStopAndKill )
  }//  if( particleName == "mu+" && aTrack->GetTrackID() == 1 )

  if( particleName == "mu+" && aTrack->GetTrackID() == 1 ){

    //-- mu+ generated.
    if( stepNum == 1 ){
      
      runData->evtStats = 1; // mu+ generated.
      runData->muInfo.first.set(stepNum,trkStatus,pstProcType,pstProcName);
      
      if( trkStatus == fStopAndKill ){
        runData->SetBit(runData->evtStats,1); // Killed at the first step!
      }
      else if( trkStatus != fAlive ){
        runData->SetBit(runData->evtStats,2); // Anything unexpected?
      }
    }

    //-- mu+ hit the target.
    if( pstStatus == fGeomBoundary && pstVol && pstVol->GetName() == "Target" ){
      runData->SetBit(runData->evtStats,3);   // Just entered into the target.
      runData->muInfo.target.set(stepNum,trkStatus,pstProcType,pstProcName);
      if( trkStatus == fStopButAlive ){
        runData->SetBit(runData->evtStats,4); // Stopped when entered into the target.
      }
      else if( trkStatus != fAlive ){
        runData->SetBit(runData->evtStats,5); // Anything unexpected?
      }
    }
    
    //-- Check if the last trkStatus must be 'fStopAndKill'.
    //   Keep updated, eventually the last step.
    runData->muInfo.last.set(stepNum,trkStatus,pstProcType,pstProcName);

    //-- mu+ at the last step,
    if( trkStatus == fStopAndKill && ( runData->evtLevel >= 2 ) ){
      
      if( pstStatus == fWorldBoundary ){
        runData->SetBit(runData->evtStats,6);     // Reached at the world boundary.
      }
      
      else{
        
        if( pstVol && pstVol->GetName() == "Target" ){
          runData->SetBit(runData->evtStats,7);   // Ended insde the target.
          if( pstProc && pstProc->GetProcessName() == "Decay" ){
    	      runData->SetBit(runData->evtStats,8); // By decay.
          }
          else{
            runData->SetBit(runData->evtStats,9); // By anything else?
            G4cout << "EndedInsideTarget: unexpected procName: "
            << pstProc->GetProcessName() << G4endl;
          }
        }

        else{
          runData->SetBit(runData->evtStats,10);   // Ended outside the target.
          if( pstProc && pstProc->GetProcessName() == "Decay" ){
            runData->SetBit(runData->evtStats,11); // By decay.
          }
          else{
            runData->SetBit(runData->evtStats,12); // By anything else?
            G4cout << "EndedOutsideTarget: unexpected procName: "
            << pstProc->GetProcessName() << G4endl;
          }
        }  
      }// if( pstStatus != fWorldBoundary )
    }//mu_+ at the last step, if( trkStatus == fStopAndKill )
  }//if( particleName == "mu+" && aTrack->GetTrackID() == 1 )
  
  /*
  //-- e+ decayed from the mu+.
  const G4VProcess* creProc = track->GetCreatorProcess();
  if( particleName == "e+" && aTrack->GetParentID() == 1
      && creProc && creProc->GetProcessName() == "Decay" ){
    //-- e+ which produced in the aerogel
    //-- e+ which produced outside the aerogel
  }
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........
