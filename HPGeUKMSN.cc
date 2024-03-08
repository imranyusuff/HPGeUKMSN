#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"
#include <time.h>

#include "HPGeUKMSNDetectorConstruction.hh"
#include "HPGeUKMSNActionInitialization.hh"
#include "QGSP_BERT.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"


long theSeed;

int main(int argc, char *argv[])
{
  G4UIExecutive *ui = 0;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }

  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  theSeed = time(NULL);
  G4Random::setTheSeed(theSeed);
  G4cout << "Random number seed is: " << theSeed << G4endl;

  G4RunManager *runManager = new G4RunManager;

  runManager->SetUserInitialization(new HPGeUKMSNDetectorConstruction);

  G4VModularPhysicsList *physicsList = new QGSP_BERT;
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);

  runManager->SetUserInitialization(new HPGeUKMSNActionInitialization);

  G4VisManager *visManager = new G4VisExecutive;
  visManager->Initialize();

  G4UImanager *UImanager = G4UImanager::GetUIpointer();

  if (!ui) {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
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

