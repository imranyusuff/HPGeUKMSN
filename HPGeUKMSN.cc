#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"
#include <sstream>
#include <time.h>
#include <unistd.h>

#include "HPGeUKMSNDetectorConstruction.hh"
#include "HPGeUKMSNActionInitialization.hh"
#include "FTFP_BERT.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4LossTableManager.hh"
#include "G4UAtomicDeexcitation.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"


long theSeed;

int main(int argc, char *argv[])
{
  G4double deadLayerThicknessMM = 0.6;
  G4int geometrySelection = 0;
  G4bool detailedIRWindow = false;
  G4bool alwaysSingleSource = false;

  std::stringstream strCmdLine;
  strCmdLine << "Invoked as: ";
  for (int i=0; i<argc; i++) {
    strCmdLine << " " << argv[i];
  }

  int opt;
  while ((opt = getopt(argc, argv, "d:g:is")) != -1) {
    switch(opt) {
    case 'd':
      deadLayerThicknessMM = atof(optarg);
      break;
    case 'g':
      geometrySelection = atoi(optarg);
      break;
    case 'i':
      detailedIRWindow = true;
      break;
    case 's':
      alwaysSingleSource = true;
      break;
    default:
      G4cerr << "Usage: " << argv[0] << " [-d deadlayer_thick/mm] [-g geom#] [-i] [-s] <other args for Geant4...>" << G4endl;
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

  runManager->SetUserInitialization(new HPGeUKMSNDetectorConstruction(geometrySelection, deadLayerThicknessMM, detailedIRWindow));

  G4VModularPhysicsList *physicsList = new FTFP_BERT;
  physicsList->SetVerboseLevel(1);
  physicsList->ReplacePhysics(new G4EmLivermorePhysics);
  physicsList->RegisterPhysics(new G4RadioactiveDecayPhysics);

  // enable atomic deexcitation
  auto de = new G4UAtomicDeexcitation();
  de->SetFluo(true);
  de->SetAuger(true);
  de->SetPIXE(true);
  G4LossTableManager::Instance()->SetAtomDeexcitation(de);

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

