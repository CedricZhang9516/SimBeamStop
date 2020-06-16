//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// P00RunData.cc
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "P00RunData.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "Geometry.hh"

#include "G4Event.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TouchableHandle.hh"
#include "G4VProcess.hh"
#include "G4VPhysicalVolume.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

P00RunData::P00RunData()
  : G4Run()
    //  , fVolumeNames{{"Target"}}
  , nbit_evt(15), nbit_hit(15), nbit_end(15), nbit_yld(10)
  , evtStats(0), hitStats(0), endStats(0), yldStats(0)
  , evtLevel(0)
{}

P00RunData::~P00RunData()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

G4int
P00RunData::GetProcID( const G4String& procName )
{
       if( procName == "UserDefinedLimit" ) return 1;
  else if( procName == "Transportation"   ) return 2;
  else if( procName == "muIoni"           ) return 3;
  else if( procName == "Decay"            ) return 4;
  else if( procName == "msc"              ) return 5;
       G4cout << "GetProcID: Unexpected process name: "
	      << procName << G4endl;
       return 0;
}

void
P00RunData::SetBit( G4int& bitvar, const G4int& ith )
{
  if( ith < 0 || ith >= sizeof(bitvar)*8 ) return;
  G4int ithbit = 1 << ith;
  bitvar = bitvar | ithbit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

void
P00RunData::CreateAtRunAction( G4AnalysisManager* mgr )
{
  CreateH1s(mgr);

  //CreateTps(mgr);
    CreateTGen1(mgr); // Ntuple ID = 0
    CreateTEnd1(mgr); // Ntuple ID = 1
    CreateTEnd(mgr);  // Ntuple ID = 2
}

void
P00RunData::FillPerEvent( G4AnalysisManager* mgr )
{
  //  inline G4bool G4TNtupleManager<tools::wroot::ntuple>::FillNtupleTColumn
  //    (G4int ntupleId, G4int columnId, const std::string& value)
  //
  //  virtual G4bool AddNtupleRow(G4int ntupleId) final;

  FillH1s(mgr);
  //FillTps(mgr);
    FillTGen1(mgr,0);
    if( evtLevel == 3 ) FillTEnd1(mgr,1);
    FillTEnd(mgr,2);
}


void
P00RunData::CreateTps( G4AnalysisManager* mgr )
{
  
  mgr->CreateNtuple("Muon", "Muon");
  mgr->CreateNtupleDColumn("X");
  mgr->CreateNtupleDColumn("Y");
  mgr->FinishNtuple();

  //mgr->CreateH1("evtStats","Event statistics",15,0,15);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

void
P00RunData::CreateH1s( G4AnalysisManager* mgr )
{
  mgr->CreateH1("evtStats","Event statistics",15,0,15);
  mgr->CreateH1("hitStats","Hit geometry",15,0,15);
  mgr->CreateH1("endStats","End geometry",15,0,15);
  mgr->CreateH1("yldStats","Yield statistics",10,0,10);
  mgr->CreateH1("evtLevel","Event level",5,0,5);
  mgr->CreateH1("trkStatF","Track status at the first step",6,0,6);
  mgr->CreateH1("trkStatT","Track status at the target step",6,0,6);
  mgr->CreateH1("trkStatL","Track status at the last step",6,0,6);
  mgr->CreateH1("procTypeL","Process type at the last step",13,0,13);

  mgr->CreateH1("genTrkStpNum" ,"Gen::trk: step number",30,0,30);
  mgr->CreateH1("genTrkStat"   ,"Gen::trk: status",6,0,6);
  mgr->CreateH1("genPreStpStat","Gen::pre: status",8,0,8);
  mgr->CreateH1("genPrePrcId"  ,"Gen::pre: process ID",6,0,6);
  mgr->CreateH1("genPreVolId"  ,"Gen::pre: volume ID",15,0,15);
  mgr->CreateH1("genPreKinE"   ,"Gen::pre: kinetic energy",100,0,5);
  mgr->CreateH1("genPreMom"    ,"Gen::pre: momentum",150,0,30);
  mgr->CreateH1("genPreVtxX"   ,"Gen::pre: vertex X",150,-15,15);
  mgr->CreateH1("genPreVtxY"   ,"Gen::pre: vertex Y",150,-15,15);
  mgr->CreateH1("genPreVtxZ"   ,"Gen::pre: vertex Z",100,-210,-190);

  mgr->CreateH1("endTrkStpNum" ,"End::trk: step number",30,0,30);
  mgr->CreateH1("endTrkStat"   ,"End::trk: status",6,0,6);
  mgr->CreateH1("endPstStpStat","End::pst: status",8,0,8);
  mgr->CreateH1("endPstPrcId"  ,"End::pst: process ID",6,0,6);
  mgr->CreateH1("endPstVolId"  ,"End::pst: volume ID",15,0,15);
  mgr->CreateH1("endPstVtxZ"   ,"End::pst: vertex Z",110,-220,220);

  mgr->CreateH1("wldPstKinE"   ,"Wld::pst: kinetic energy",100,0,5);
  mgr->CreateH1("wldPstMom"    ,"Wld::pst: momentum",150,0,30);
  mgr->CreateH1("wldPstVtxX"   ,"Wld::pst: vertex X",100,-100,100);
  mgr->CreateH1("wldPstVtxY"   ,"Wld::pst: vertex Y",100,-100,100);
  mgr->CreateH1("wldPstVtxZ"   ,"Wld::pst: vertex Z",110,-220,220);
  
  mgr->CreateH1("tgtPstKinE"   ,"Tgt::pst: kinetic energy",50,0,5);
  mgr->CreateH1("tgtPstMom"    ,"Tgt::pst: momentum",60,0,30);
  mgr->CreateH1("tgtPstVtxX"   ,"Tgt::pst: vertex X",150,-15,15);
  mgr->CreateH1("tgtPstVtxY"   ,"Tgt::pst: vertex Y",150,-15,15);
  mgr->CreateH1("tgtPstVtxZa"  ,"Tgt::pst: vertex Z (range a)",130,-0.12,0.01);
  mgr->CreateH1("tgtPstVtxZb"  ,"Tgt::pst: vertex Z (range b)",130,-12,1);
}

void
P00RunData::FillH1s( G4AnalysisManager* mgr )
{
  int hid= 1;
  for( G4int ibit=0; ibit<nbit_evt; ibit++ ){
    int biq = 1 << ibit; // bit in question.
    if( evtStats & biq ) mgr->FillH1(hid,ibit);
  }
  hid++;
  for( G4int ibit=0; ibit<nbit_hit; ibit++ ){
    int biq = 1 << ibit; // bit in question.
    if( hitStats & biq ) mgr->FillH1(hid,ibit);
  }
  hid++;
  for( G4int ibit=0; ibit<nbit_end; ibit++ ){
    int biq = 1 << ibit; // bit in question.
    if( endStats & biq ) mgr->FillH1(hid,ibit);
  }
  hid++;
  for( G4int ibit=0; ibit<nbit_yld; ibit++ ){
    int biq = 1 << ibit; // bit in question.
    if( yldStats & biq ) mgr->FillH1(hid,ibit);
  }
  hid++; mgr->FillH1(hid,evtLevel);
  hid++; mgr->FillH1(hid,muInfo.first.trkstatus);
  hid++; mgr->FillH1(hid,muInfo.target.trkstatus);
  hid++; mgr->FillH1(hid,muInfo.last.trkstatus);
  hid++; mgr->FillH1(hid,muInfo.last.proctype);

  hid++; mgr->FillH1(hid,hldGen.trkStpNum);
  hid++; mgr->FillH1(hid,hldGen.trkStat);
  hid++; mgr->FillH1(hid,hldGen.pntStpStat[0]);
  hid++; mgr->FillH1(hid,hldGen.pntPrcId[0]);
  hid++; mgr->FillH1(hid,hldGen.pntVolId[0]);
  hid++; mgr->FillH1(hid,hldGen.pntKinE[0]);
  hid++; mgr->FillH1(hid,hldGen.pntMom[0].mag());
  hid++; mgr->FillH1(hid,hldGen.pntVtx[0].x());
  hid++; mgr->FillH1(hid,hldGen.pntVtx[0].y());
  hid++; mgr->FillH1(hid,hldGen.pntVtx[0].z());

  hid++; mgr->FillH1(hid,hldEnd.trkStpNum);
  hid++; mgr->FillH1(hid,hldEnd.trkStat);
  hid++; mgr->FillH1(hid,hldEnd.pntStpStat[1]);
  hid++; mgr->FillH1(hid,hldEnd.pntPrcId[1]);
  hid++; mgr->FillH1(hid,hldEnd.pntVolId[1]);
  hid++; mgr->FillH1(hid,hldEnd.pntVtx[1].z());

  if( yldStats & 0b11110000 ){
  // if( yldStats & 0b10000000 ){
    hid++; mgr->FillH1(hid,hldEnd.pntKinE[1]);
    hid++; mgr->FillH1(hid,hldEnd.pntMom[1].mag());
    hid++; mgr->FillH1(hid,hldEnd.pntVtx[1].x());
    hid++; mgr->FillH1(hid,hldEnd.pntVtx[1].y());
    hid++; mgr->FillH1(hid,hldEnd.pntVtx[1].z());
  }else{
    hid += 5;
  }

  if( evtLevel == 3 ){
    hid++; mgr->FillH1(hid,hldEnd.pntKinE[1]);
    hid++; mgr->FillH1(hid,hldEnd.pntMom[1].mag());
    hid++; mgr->FillH1(hid,hldEnd.pntVtx[1].x());
    hid++; mgr->FillH1(hid,hldEnd.pntVtx[1].y());
    hid++; mgr->FillH1(hid,hldEnd.pntVtx[1].z());
    hid++; mgr->FillH1(hid,hldEnd.pntVtx[1].z());
  }else{
    hid += 6;
  }
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

void
P00RunData::CreateTGen1( G4AnalysisManager* mgr )
{
  mgr->CreateNtuple("energy", "mu+ initial kinetic energy");
  mgr->CreateNtupleDColumn("e"); // columnID = 0
  mgr->FinishNtuple();
}

void P00RunData::FillTGen1( G4AnalysisManager* mgr, const G4int& tid )
{
  G4int cid = 0;
  mgr->FillNtupleDColumn(tid,cid++,hldGen.pntKinE[2]);
  mgr->AddNtupleRow(tid);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

void
P00RunData::CreateTEnd1( G4AnalysisManager* mgr )
{
  mgr->CreateNtuple("position","mu+ decay position in the target");
  mgr->CreateNtupleDColumn("endKinE");
  mgr->CreateNtupleDColumn("endMom" );
  mgr->CreateNtupleDColumn("glbt_end"); // columnID = 0...wrong these comments, changed already
  mgr->CreateNtupleDColumn("glbt_gen"); // columnID = 0...wrong these comments, changed already
  mgr->CreateNtupleDColumn("prpt"); // columnID = 0...wrong these comments, changed already
  mgr->CreateNtupleDColumn("loct"); // columnID = 0...wrong these comments, changed already
  mgr->CreateNtupleDColumn("x"); // columnID = 0
  mgr->CreateNtupleDColumn("y"); // columnID = 1
  mgr->CreateNtupleDColumn("z"); // columnID = 2
  mgr->FinishNtuple();
}

void P00RunData::FillTEnd1( G4AnalysisManager* mgr, const G4int& tid )
{
  G4int cid = 0;
  mgr->FillNtupleDColumn(tid,cid++,hldEnd.pntKinE[1]);
  mgr->FillNtupleDColumn(tid,cid++,hldEnd.pntMom[1].mag());
  mgr->FillNtupleDColumn(tid,cid++,hldEnd.glbtime);
  mgr->FillNtupleDColumn(tid,cid++,hldGen.glbtime);
  mgr->FillNtupleDColumn(tid,cid++,hldEnd.pntPrpT[1]);
  mgr->FillNtupleDColumn(tid,cid++,hldEnd.pntLocT[1]);
  mgr->FillNtupleDColumn(tid,cid++,hldEnd.pntVtx[1].x());
  mgr->FillNtupleDColumn(tid,cid++,hldEnd.pntVtx[1].y());
  mgr->FillNtupleDColumn(tid,cid++,hldEnd.pntVtx[1].z());
        //= aPoint->GetProperTime();
      //= aPoint->GetLocalTime();
  mgr->AddNtupleRow(tid);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

void
P00RunData::CreateTGen2( G4AnalysisManager* mgr )
{
  mgr->CreateNtuple("gen","mu+ generated and ended");
  //  CreateColumns(mgr,"prd");
  //  CreateColumns(mgr,"end");

  mgr->CreateNtupleDColumn("prdKinE");
  mgr->CreateNtupleDColumn("prdMom" );
  mgr->CreateNtupleDColumn("prdVtxX");
  mgr->CreateNtupleDColumn("prdVtxY");
  mgr->CreateNtupleDColumn("prdVtxZ");

  mgr->CreateNtupleIColumn("endEvtLevel");
  mgr->CreateNtupleIColumn("endTrkStat");
  mgr->CreateNtupleIColumn("endStpStat");
  mgr->CreateNtupleIColumn("endPrcId"  );
  mgr->CreateNtupleIColumn("endVolId"  );

  mgr->CreateNtupleDColumn("endKinE");
  mgr->CreateNtupleDColumn("endMom" );
  mgr->CreateNtupleDColumn("endVtxX");
  mgr->CreateNtupleDColumn("endVtxY");
  mgr->CreateNtupleDColumn("endVtxZ");

  mgr->FinishNtuple();
}

void P00RunData::FillTGen2( G4AnalysisManager* mgr, const G4int& tid )
{
  G4int cid = 0;

  //  FillColumns(mgr,tid,cid,hldGen,0);
  //  FillColumns(mgr,tid,cid,hldEnd,1);
  /*
  mgr->FillNtupleDColumn(tid,cid++,hldGen.pntKinE[0]);
  mgr->FillNtupleDColumn(tid,cid++,hldGen.pntMom[0].mag());
  mgr->FillNtupleDColumn(tid,cid++,hldGen.pntVtx[0].x());
  mgr->FillNtupleDColumn(tid,cid++,hldGen.pntVtx[0].y());
  mgr->FillNtupleDColumn(tid,cid++,hldGen.pntVtx[0].z());
  */
  
  mgr->FillNtupleIColumn(tid,cid++,evtLevel);
  mgr->FillNtupleIColumn(tid,cid++,hldEnd.trkStat);
  mgr->FillNtupleIColumn(tid,cid++,hldEnd.pntStpStat[1]);
  mgr->FillNtupleIColumn(tid,cid++,hldEnd.pntPrcId[1]  );
  mgr->FillNtupleIColumn(tid,cid++,hldEnd.pntVolId[1]  );

  mgr->FillNtupleDColumn(tid,cid++,hldEnd.pntKinE[1]);
  mgr->FillNtupleDColumn(tid,cid++,hldEnd.pntMom[1].mag());
  mgr->FillNtupleDColumn(tid,cid++,hldEnd.pntVtx[1].x());
  mgr->FillNtupleDColumn(tid,cid++,hldEnd.pntVtx[1].y());
  mgr->FillNtupleDColumn(tid,cid++,hldEnd.pntVtx[1].z());
  
  mgr->AddNtupleRow(tid);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

void
P00RunData::CreateTTgt2( G4AnalysisManager* mgr )
{
  mgr->CreateNtuple("tgt","mu+ hit target and ended");
  //  CreateColumns(mgr,"hit0");
  //  CreateColumns(mgr,"hit1");
  //  CreateColumns(mgr,"ext0");
  //  CreateColumns(mgr,"ext1");
  mgr->FinishNtuple();
}

void P00RunData::FillTTgt2( G4AnalysisManager* mgr, const G4int& tid )
{
  G4int cid = 0;
  //  FillColumns(mgr,tid,cid,hldTgtHit0,1);
  //  FillColumns(mgr,tid,cid,hldTgtHit1,1);
  //  FillColumns(mgr,tid,cid,hldTgtExt0,1);
  //  FillColumns(mgr,tid,cid,hldTgtExt1,1);
  mgr->AddNtupleRow(tid);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

void
P00RunData::CreateTEnd( G4AnalysisManager* mgr )
{
  mgr->CreateNtuple("end","mu+ ended");
  //  CreateColumns(mgr,"end");

  mgr->CreateNtupleIColumn("endTrkStpNum");
  mgr->CreateNtupleIColumn("endTrkStat");
  mgr->CreateNtupleIColumn("endEvtLevel");

  mgr->CreateNtupleIColumn("endPreStpStat");
  mgr->CreateNtupleIColumn("endPrePrcId"  );
  mgr->CreateNtupleIColumn("endPreVolId"  );
  mgr->CreateNtupleDColumn("endPreKinE");
  mgr->CreateNtupleDColumn("endPreMom" );
  mgr->CreateNtupleDColumn("endPreVtxX");
  mgr->CreateNtupleDColumn("endPreVtxY");
  mgr->CreateNtupleDColumn("endPreVtxZ");

  mgr->CreateNtupleIColumn("endPstStpStat");
  mgr->CreateNtupleIColumn("endPstPrcId"  );
  mgr->CreateNtupleIColumn("endPstVolId"  );
  mgr->CreateNtupleDColumn("endPstKinE");
  mgr->CreateNtupleDColumn("endPstMom" );
  mgr->CreateNtupleDColumn("endPstVtxX");
  mgr->CreateNtupleDColumn("endPstVtxY");
  mgr->CreateNtupleDColumn("endPstVtxZ");

  mgr->FinishNtuple();
}

void
P00RunData::FillTEnd( G4AnalysisManager* mgr, const G4int& tid )
{
  G4int cid = 0;

  //  FillColumns(mgr,tid,cid,hldEnd,1);

  mgr->FillNtupleIColumn(tid,cid++,hldEnd.trkStpNum);
  mgr->FillNtupleIColumn(tid,cid++,hldEnd.trkStat);
  mgr->FillNtupleIColumn(tid,cid++,evtLevel);

  for( int index=0; index<2; index++ ){
    mgr->FillNtupleIColumn(tid,cid++,hldEnd.pntStpStat[index]);
    mgr->FillNtupleIColumn(tid,cid++,hldEnd.pntPrcId[index]  );
    mgr->FillNtupleIColumn(tid,cid++,hldEnd.pntVolId[index]  );
    mgr->FillNtupleDColumn(tid,cid++,hldEnd.pntKinE[index]);
    mgr->FillNtupleDColumn(tid,cid++,hldEnd.pntMom[index].mag());
    mgr->FillNtupleDColumn(tid,cid++,hldEnd.pntVtx[index].x());
    mgr->FillNtupleDColumn(tid,cid++,hldEnd.pntVtx[index].y());
    mgr->FillNtupleDColumn(tid,cid++,hldEnd.pntVtx[index].z());
  }
  
  mgr->AddNtupleRow(tid);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

void
P00RunData::CreateColumns( G4AnalysisManager* mgr, const G4String& prefix )
{
  mgr->CreateNtupleIColumn(prefix+"EvtLevel");

  mgr->CreateNtupleIColumn(prefix+"TrkStat");
  mgr->CreateNtupleIColumn(prefix+"StpNum");
  mgr->CreateNtupleDColumn(prefix+"TrkLen");

  /*
  mgr->CreateNtupleDColumn(prefix+"StpLen");
  mgr->CreateNtupleDColumn(prefix+"StpDelT");
  mgr->CreateNtupleDColumn(prefix+"StpDelTotE");
  mgr->CreateNtupleDColumn(prefix+"StpDelNiE");
  mgr->CreateNtupleDColumn(prefix+"StpDelVtxX");
  mgr->CreateNtupleDColumn(prefix+"StpDelVtxY");
  mgr->CreateNtupleDColumn(prefix+"StpDelVtxZ");
  */
  
  mgr->CreateNtupleIColumn(prefix+"StpStat");
  mgr->CreateNtupleIColumn(prefix+"PrcType");
  mgr->CreateNtupleIColumn(prefix+"PrcId"  );
  mgr->CreateNtupleIColumn(prefix+"VolId"  );
  mgr->CreateNtupleDColumn(prefix+"Beta"   );
  mgr->CreateNtupleDColumn(prefix+"Gamma"  );

  mgr->CreateNtupleDColumn(prefix+"PrpT");
  mgr->CreateNtupleDColumn(prefix+"LocT");
  mgr->CreateNtupleDColumn(prefix+"Velo");
  mgr->CreateNtupleDColumn(prefix+"KinE");
  mgr->CreateNtupleDColumn(prefix+"VtxX");
  mgr->CreateNtupleDColumn(prefix+"VtxY");
  mgr->CreateNtupleDColumn(prefix+"VtxZ");
  mgr->CreateNtupleDColumn(prefix+"Mom" );
  mgr->CreateNtupleDColumn(prefix+"MomX");
  mgr->CreateNtupleDColumn(prefix+"MomY");
  mgr->CreateNtupleDColumn(prefix+"MomZ");

  /*
  for( G4int index= 0; index<2; index++ ){
    G4String snum = std::to_string(index);
    mgr->CreateNtupleIColumn(prefix+"StpStat"+snum);
    mgr->CreateNtupleIColumn(prefix+"PrcType"+snum);
    mgr->CreateNtupleIColumn(prefix+"PrcId"  +snum);
    mgr->CreateNtupleIColumn(prefix+"VolId"  +snum);
    mgr->CreateNtupleDColumn(prefix+"Beta"   +snum);
    mgr->CreateNtupleDColumn(prefix+"Gamma"  +snum);
  }

  for( G4int index= 0; index<3; index++ ){
    G4String snum = std::to_string(index);
    mgr->CreateNtupleDColumn(prefix+"PrpT"+snum);
    mgr->CreateNtupleDColumn(prefix+"LocT"+snum);
    mgr->CreateNtupleDColumn(prefix+"Velo"+snum);
    mgr->CreateNtupleDColumn(prefix+"KinE"+snum);
    mgr->CreateNtupleDColumn(prefix+"VtxX"+snum);
    mgr->CreateNtupleDColumn(prefix+"VtxY"+snum);
    mgr->CreateNtupleDColumn(prefix+"VtxZ"+snum);
    mgr->CreateNtupleDColumn(prefix+"Mom" +snum);
    mgr->CreateNtupleDColumn(prefix+"MomX"+snum);
    mgr->CreateNtupleDColumn(prefix+"MomY"+snum);
    mgr->CreateNtupleDColumn(prefix+"MomZ"+snum);
  }
  */
}

void
P00RunData::FillColumns( G4AnalysisManager* mgr, const G4int& tid,
			 G4int& cid, const DataHolder& hld, const G4int& index )
{
  mgr->FillNtupleIColumn(tid,cid++,evtLevel);

  mgr->FillNtupleIColumn(tid,cid++,hld.trkStat);
  mgr->FillNtupleIColumn(tid,cid++,hld.trkStpNum);
  mgr->FillNtupleDColumn(tid,cid++,hld.trkLen);

  /*
  mgr->FillNtupleDColumn(tid,cid++,hld.stpLen);
  mgr->FillNtupleDColumn(tid,cid++,hld.stpDelT);
  mgr->FillNtupleDColumn(tid,cid++,hld.stpDelTotE);
  mgr->FillNtupleDColumn(tid,cid++,hld.stpDelNiE);
  mgr->FillNtupleDColumn(tid,cid++,hld.stpDelVtx.x());
  mgr->FillNtupleDColumn(tid,cid++,hld.stpDelVtx.y());
  mgr->FillNtupleDColumn(tid,cid++,hld.stpDelVtx.z());
  */
  
  //  for( G4int index= 0; index<2; index++ ){
    mgr->FillNtupleIColumn(tid,cid++,hld.pntStpStat[index]);
    mgr->FillNtupleIColumn(tid,cid++,hld.pntPrcType[index]);
    mgr->FillNtupleIColumn(tid,cid++,hld.pntPrcId[index]  );
    mgr->FillNtupleIColumn(tid,cid++,hld.pntVolId[index]  );
    mgr->FillNtupleDColumn(tid,cid++,hld.pntBeta[index]   );
    mgr->FillNtupleDColumn(tid,cid++,hld.pntGamma[index]  );
    //  }

    //  for( G4int index= 0; index<3; index++ ){
    mgr->FillNtupleDColumn(tid,cid++,hld.pntPrpT[index]);
    mgr->FillNtupleDColumn(tid,cid++,hld.pntLocT[index]);
    mgr->FillNtupleDColumn(tid,cid++,hld.pntVelo[index]);
    mgr->FillNtupleDColumn(tid,cid++,hld.pntKinE[index]);
    mgr->FillNtupleDColumn(tid,cid++,hld.pntVtx[index].x());
    mgr->FillNtupleDColumn(tid,cid++,hld.pntVtx[index].y());
    mgr->FillNtupleDColumn(tid,cid++,hld.pntVtx[index].z());
    mgr->FillNtupleDColumn(tid,cid++,hld.pntMom[index].mag());
    mgr->FillNtupleDColumn(tid,cid++,hld.pntMom[index].x());
    mgr->FillNtupleDColumn(tid,cid++,hld.pntMom[index].y());
    mgr->FillNtupleDColumn(tid,cid++,hld.pntMom[index].z());
    //  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

void
P00RunData::SetAllVars( const G4Step* aStep, DataHolder& hld )
{
  SetTrkVars(aStep,hld);
  SetStpVars(aStep,hld);
  SetPntVars(aStep,hld,0);
  SetPntVars(aStep,hld,1);
}

void
P00RunData::SetTrkVars( const G4Step* aStep, DataHolder& hld )
{
  G4Track* aTrack = aStep->GetTrack();

  hld.trkStat   = (G4int)aTrack->GetTrackStatus();
  hld.trkStpNum = aTrack->GetCurrentStepNumber();
  hld.trkLen    = aTrack->GetTrackLength();
  //  hld.stpLen    = aTrack->GetStepLength(); // Also available from step info.

  //--> Also available from step-point info.
  const G4int index= 2; // index= 0, 1 are filled in SetPntVars().
  hld.pntPrpT[index] = aTrack->GetProperTime();
  hld.pntLocT[index] = aTrack->GetLocalTime();
  hld.pntVelo[index] = aTrack->GetVelocity();
  hld.pntKinE[index] = aTrack->GetKineticEnergy();
  hld.pntVtx[index]  = aTrack->GetPosition();
  hld.pntMom[index]  = aTrack->GetMomentum();
  //  hld.pol        = aTrack->GetPolarization();
    //hld.glbtime    = aTrack->GetGlobalTime();
  //  hld.totE       = aTrack->GetTotalEnergy();
}

void
P00RunData::SetStpVars( const G4Step* aStep, DataHolder& hld )
{
  hld.stpLen     = aStep->GetStepLength();
  hld.stpDelT    = aStep->GetDeltaTime();
  hld.stpDelTotE = aStep->GetTotalEnergyDeposit();
  hld.stpDelNiE  = aStep->GetNonIonizingEnergyDeposit();
  hld.stpDelVtx  = aStep->GetDeltaPosition();
}

void
P00RunData::SetPntVars( const G4Step* aStep, DataHolder& hld,
			const G4int& index )
{
  G4StepPoint* aPoint = nullptr;
  switch( index ){
  case 0: aPoint = aStep->GetPreStepPoint();  break;
  case 1: aPoint = aStep->GetPostStepPoint(); break;
  default:
    std::cout << "SetPntVars::unexpected index: " << index << std::endl;
    return;
  }

  const G4VProcess* aProcess = aPoint->GetProcessDefinedStep();
  G4String procName("UserDefinedLimit");
  G4int procType= -1;
  if( aProcess ){
    procName= aProcess->GetProcessName();
    procType= (G4int)aProcess->GetProcessType();
  }

  G4VPhysicalVolume* vol= aPoint->GetPhysicalVolume();
  G4int volId= -1;
  if( vol ) volId = Geometry::volumeID[vol->GetName()];

  hld.pntStpStat[index] = (G4int)aPoint->GetStepStatus();
  hld.pntPrcType[index] = procType;
  hld.pntPrcId[index]   = GetProcID(procName);
  hld.pntVolId[index]   = volId;
  hld.pntBeta[index]    = aPoint->GetBeta();  // Velocity in unit of c.
  hld.pntGamma[index]   = aPoint->GetGamma(); // = 1/sqrt[1-beta*beta].

  //-- Also available from track info.
  hld.pntPrpT[index]    = aPoint->GetProperTime();
  hld.pntLocT[index]    = aPoint->GetLocalTime();
  hld.pntVelo[index]    = aPoint->GetVelocity();
  hld.pntKinE[index]    = aPoint->GetKineticEnergy();
  hld.pntVtx[index]     = aPoint->GetPosition();
  hld.pntMom[index]     = aPoint->GetMomentum();
  //    hld.pol         = aPoint->GetPolarization();
      hld.glbtime     = aPoint->GetGlobalTime();
  //    hld.totE        = aPoint->GetTotalEnergy();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

void
P00RunData::Reset()
{ 
  evtStats = 0;
  hitStats = 0;
  endStats = 0;
  yldStats = 0;
  evtLevel = 0;
  muInfo.reset();
  ClearVars(hldGen);
  ClearVars(hldEnd);
}

void
P00RunData::ClearVars( DataHolder& hld )
{
  G4ThreeVector zeroVct(0,0,0);
  
  hld.trkStat    = -1;
  hld.trkStpNum  = -1;
  hld.trkLen     = -1;

  hld.stpLen     = -1;
  hld.stpDelT    = -1;
  hld.stpDelTotE = -1;
  hld.stpDelNiE  = -1;
  hld.stpDelVtx  = zeroVct;

  hld.glbtime = -1;

  for( G4int index= 0; index<2; index++ ){
    hld.pntStpStat[index] = -1;
    hld.pntPrcType[index] = -1;
    hld.pntPrcId[index]   = -1;
    hld.pntVolId[index]   = -1;
    hld.pntBeta[index]    = -1;
    hld.pntGamma[index]   = -1;
  }
  
  for( G4int index= 0; index<3; index++ ){
    hld.pntPrpT[index] = -1;
    hld.pntLocT[index] = -1;
    hld.pntVelo[index] =  0;
    hld.pntKinE[index] = -1;
    hld.pntVtx[index]  = zeroVct;
    hld.pntMom[index]  = zeroVct;
    //  hld.pol        = zeroVct;
    //  hld.glbtime    = -1;
    //  hld.totE       = -1;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........

void
P00RunData::PrintPerEvent( const G4Event* event )
{
  G4bool printF= false;
  G4bool printT= false;
  G4bool printL= false;
  if( muInfo.first.trkstatus  !=  0 &&
      muInfo.first.trkstatus  !=  2 ) printF= true;
  if( muInfo.target.stpnum    != -1 &&
      muInfo.target.trkstatus !=  0 &&
      muInfo.target.trkstatus !=  1 ) printT= true;
  if( muInfo.last.trkstatus   !=  2 ) printL= true;

  auto eventID = event->GetEventID();
  if( printF || printT || printL ){
    G4cout << "EndOfEvent: " << eventID << G4endl;
    if( printF ){ G4cout << "unexpected muInfo.first: ";  muInfo.first.print(); }
    if( printT ){ G4cout << "unexpected muInfo.target: "; muInfo.target.print(); }
    if( printL ){ G4cout << "unexpected muInfo.last: ";   muInfo.last.print(); }
  }
}
