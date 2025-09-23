#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"
#include <sstream>
#include <time.h>
#include <unistd.h>

#include "HPGeUKMSNDetectorConstruction.hh"
#include "HPGeUKMSNActionInitialization.hh"
#include "QGSP_BERT.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"


long theSeed;

int main(int argc, char *argv[])
{
  G4int geometrySelection = 0;
  G4bool alwaysSingleSource = false;

  std::stringstream strCmdLine;
  strCmdLine << "Invoked as: ";
  for (int i=0; i<argc; i++) {
    strCmdLine << " " << argv[i];
  }

  int opt;
  while ((opt = getopt(argc, argv, "g:s")) != -1) {
    switch(opt) {
    case 'g':
      geometrySelection = atoi(optarg);
      break;
    case 's':
      alwaysSingleSource = true;
      break;
    default:
      G4cerr << "Usage: " << argv[0] << " [-g geom#] [-s] <other args for Geant4...>" << G4endl;
      return 1;
    }
  }

  G4UIExecutive *ui = 0;
  if (argc == optind) {
    ui = new G4UIExecutive(argc, argv);
  }

  G4cout << strCmdLine.str() << std::endl;
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  theSeed = time(NULL);
  G4Random::setTheSeed(theSeed);
  G4cout << "Random number seed is: " << theSeed << G4endl;

  G4RunManager *runManager = new G4RunManager;

  runManager->SetUserInitialization(new HPGeUKMSNDetectorConstruction(geometrySelection));

  G4VModularPhysicsList *physicsList = new QGSP_BERT;
  physicsList->SetVerboseLevel(1);
  physicsList->RegisterPhysics(new G4RadioactiveDecayPhysics);
  runManager->SetUserInitialization(physicsList);

  runManager->SetUserInitialization(new HPGeUKMSNActionInitialization(geometrySelection, alwaysSingleSource));

  G4VisManager *visManager = new G4VisExecutive;
  visManager->Initialize();

  G4UImanager *UImanager = G4UImanager::GetUIpointer();

  if (!ui) {
    G4String command = "/control/execute ";
    G4String fileName = argv[optind];
    UImanager->ApplyCommand(command + fileName);
  }
  else {
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  delete visManager;
  delete runManager;

  return 0;
}

