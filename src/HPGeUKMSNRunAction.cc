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
}


HPGeUKMSNRunAction::~HPGeUKMSNRunAction()
{
  delete G4AnalysisManager::Instance();
}


void HPGeUKMSNRunAction::BeginOfRunAction(const G4Run * /* run */)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile();
}


void HPGeUKMSNRunAction::EndOfRunAction(const G4Run * /* run */)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}

