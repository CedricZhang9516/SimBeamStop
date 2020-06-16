//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Geant4 Application
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "Geometry.hh"
#include "P00ActionInitialization.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "FTFP_BERT.hh"
#include "Randomize.hh"

#include "G4UIcommand.hh"
#include <time.h>


int main( int argc, char** argv )
{

  G4int DGthickness = 400;
  G4String macrofile("");
  G4int targetnum = 0;
  G4String OutputName("SimBeamStop");
  G4int InputN = 0;
  G4String InputName("SimBeamStop.txt");

  switch( argc ){
  case 2:
    targetnum= atoi(argv[1]);
    break;
  case 3:
    targetnum= atoi(argv[1]);
    macrofile= argv[2];
    break;
  case 5:
    targetnum= atoi(argv[1]);
    macrofile= argv[2];
    DGthickness= atoi(argv[3]);
    OutputName= argv[4];
    break;

  case 7:
    targetnum = atoi(argv[1]);
    macrofile = argv[2];
    DGthickness = atoi(argv[3]);
    OutputName = argv[4];
    InputName = argv[5];
    InputN = atoi(argv[6]);
    break;

  default:
    G4cout << "Usage: " << argv[0] << " targetNumber [macroFile]"
	   << G4endl;
    return -1;
  }

  // Seed random number according to time
  G4Random::setTheEngine(new CLHEP::RanluxEngine); // Choose the Random engine
  std::srand ( time(NULL) );
  // std::srand(0);
  double seed=std::rand();
  G4Random::setTheSeed(seed,4);
  G4cout << "Random seed = " << seed << G4endl;

  G4RunManager* runManager = new G4RunManager; // Construct default run manager

  //Set up mandatory user initialization
  auto geometry = new Geometry(targetnum, DGthickness);
  auto physicsList = new FTFP_BERT;
  auto actionInitialization = new P00ActionInitialization(OutputName, InputName, InputN);
  runManager->SetUserInitialization( geometry );
  runManager->SetUserInitialization( physicsList );
  runManager->SetUserInitialization( actionInitialization );
  runManager->Initialize(); // Initialize G4 kernel

  // Create visualization environment
  G4VisManager* visManager = new G4VisExecutive();
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* uiManager = G4UImanager::GetUIpointer();

  if ( macrofile != "" ) {
    G4String command = "/control/execute ";
    uiManager->ApplyCommand(command+macrofile);
  } else {
    // Interactive mode
    G4UIExecutive* uiExec = new G4UIExecutive(1, argv);
   // uiManager->ApplyCommand("/control/execute init_vis.mac");
    uiExec->SessionStart();
    delete uiExec;
  }


  // Job termination
   delete visManager;
   delete runManager;
   return 0;
 }
