#include "HPGeUKMSNRunAction.hh"

#include "G4Run.hh"

//#include "g4root.hh"
//#include "g4xml.hh"
//#include "g4csv.hh"
#include "G4AnalysisManager.hh"


extern long theSeed;

HPGeUKMSNRunAction::HPGeUKMSNRunAction()
 : G4UserRunAction()
{
  auto analysisManager = G4AnalysisManager::Instance();
  std::stringstream fn;
  fn << "HPGeUKMSNrun_" << theSeed;
  analysisManager->SetDefaultFileType("root");
  analysisManager->SetFileName(fn.str());
  analysisManager->CreateNtuple("energies", "Energies");
  analysisManager->CreateNtupleDColumn("Energy");
  analysisManager->FinishNtuple();
  G4cout << "Output file name is: " << fn.str() << G4endl;
}


HPGeUKMSNRunAction::~HPGeUKMSNRunAction()
{
  delete G4AnalysisManager::Instance();
}


void HPGeUKMSNRunAction::BeginOfRunAction(const G4Run * /* run */)
{
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Opening output file..." << G4endl;
  G4bool ret = analysisManager->OpenFile();
  if (ret) {
    G4cout << "File open success!" << G4endl;
  }
  else {
    G4cout << "FAILED to open file for output!" << G4endl;
  }
}


void HPGeUKMSNRunAction::EndOfRunAction(const G4Run *run)
{
  const G4int runID = run->GetRunID();
  G4cout << "Run " << runID << " finished." << G4endl;
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Writing output file..." << G4endl;
  G4bool ret = analysisManager->Write();
  if (ret) {
    G4cout << "File write success!" << G4endl;
  }
  else {
    G4cout << "FAILED to write output file!" << G4endl;
  }
  ret = analysisManager->CloseFile();
  if (ret) {
    G4cout << "File close success!" << G4endl;
  }
  else {
    G4cout << "FAILED to close file!" << G4endl;
  }
}

