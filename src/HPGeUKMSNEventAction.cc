#include "HPGeUKMSNEventAction.hh"
#include "HPGeUKMSNRunAction.hh"
#include "HPGeUKMSNDetectorSD.hh"
#include "HPGeUKMSNDetectorHit.hh"

#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

//#include "g4root.hh"
//#include "g4xml.hh"
//#include "g4csv.hh"
#include "G4AnalysisManager.hh"


HPGeUKMSNEventAction::HPGeUKMSNEventAction(HPGeUKMSNRunAction *runAction)
 : G4UserEventAction(),
   fRunAction(runAction),
   fEdep(0.),
   fDetHCID(-1),
   fEventEdepPrintCount(0),
   fEventEdepPrintCountMax(100)
{}


HPGeUKMSNEventAction::~HPGeUKMSNEventAction()
{}


HPGeUKMSNDetectorHitsCollection *
HPGeUKMSNEventAction::GetHitsCollection(G4int hcID, const G4Event *event) const
{
  auto hitsCollection = static_cast<HPGeUKMSNDetectorHitsCollection *>(event->GetHCofThisEvent()->GetHC(hcID));

  if (!hitsCollection) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsColelction ID " << hcID;
    G4Exception("HPGeUKMSNEventAction::GetHitsCollection()", "MyCode0003", FatalException, msg);
  }

  return hitsCollection;
}


void HPGeUKMSNEventAction::BeginOfEventAction(const G4Event *event)
{
  const G4int evtID = event->GetEventID();
  if (((evtID < 1000) && (evtID % 100 == 0)) ||
      (evtID % 1000 == 0)) {
    G4cout << "Processing event " << evtID << G4endl;
  }
}


void HPGeUKMSNEventAction::EndOfEventAction(const G4Event *event)
{
  // Get hits collection ID (once)
  if (fDetHCID == -1) {
    fDetHCID = G4SDManager::GetSDMpointer()->GetCollectionID("DetectorHitsCollection");
  }
  auto detHCID = GetHitsCollection(fDetHCID, event);

  // sum up hits to find total energy deposited into the detector
  G4double totalEdep = 0;
  if (detHCID) {
    for (unsigned long i = 0; i < detHCID->GetSize(); ++i) {
      auto hit = static_cast<HPGeUKMSNDetectorHit *>(detHCID->GetHit(i));
      G4double edep = hit->GetEdep();
      totalEdep += edep;
    }
  }
  if (totalEdep > 0.) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0, totalEdep);
    analysisManager->AddNtupleRow();
  }

  // if verbose then print total deposit and find if there are any positrons
  //if (verboseLevel > 1) {
  if (totalEdep > 0. && fEventEdepPrintCount < fEventEdepPrintCountMax) {
    const G4int evtID = event->GetEventID();

    // print total deposit
    G4cout << "Event " << evtID << " total energy deposit = " << G4BestUnit(totalEdep, "Energy") << G4endl;
    fEventEdepPrintCount++;

    if (fEventEdepPrintCount == fEventEdepPrintCountMax) {
      G4cout << "Maximum event count reached for total energy deposit info." << G4endl;
    }

    /* (no need for now)
    // detect and inform if event has positron
    G4TrajectoryContainer *trajs = event->GetTrajectoryContainer();
    if (trajs) {
      const G4int nTrajs = trajs->entries();
      for (G4int i = 0; i < nTrajs; ++i) {
        const G4String particleName = (*trajs)[i]->GetParticleName();
        if (particleName == "e+") {
          G4cout << "e+e+e+ POSITRON DETECTED !!! Event no " << event->GetEventID() << " +e+e+e" << G4endl;
          break;
        }
      }
    }
    */
  }
}

