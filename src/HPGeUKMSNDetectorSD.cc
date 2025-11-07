#include "HPGeUKMSNDetectorSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4TransportationManager.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"


HPGeUKMSNDetectorSD::HPGeUKMSNDetectorSD(const G4String & name, const G4String & hitsCollectionName)
 : G4VSensitiveDetector(name),
   fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
  fNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
}


HPGeUKMSNDetectorSD::~HPGeUKMSNDetectorSD()
{}


void HPGeUKMSNDetectorSD::Initialize(G4HCofThisEvent *hce)
{
  fHitsCollection = new HPGeUKMSNDetectorHitsCollection(SensitiveDetectorName, collectionName[0]);

  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);
}


G4bool HPGeUKMSNDetectorSD::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
  const G4double deadLayerThickness = 1.*mm;      // HPGe dead-layer thickness
  const G4double threshold = 0.*CLHEP::keV;       // HPGe detection threshold (disable for now)

  const G4StepPoint *prePoint = aStep->GetPreStepPoint();
  const G4ThreeVector globalPos = prePoint->GetPosition();
  //const G4ThreeVector direction = prePoint->GetMomentumDirection().unit();

  const G4double safety = fNavigator->ComputeSafety(globalPos);
  //G4cout << "Safety is " << safety/mm << " mm" << G4endl;

  if (safety < deadLayerThickness) {
    // Hard cutoff
    return false;
  }

  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep < threshold) return false;       // No hit creation below threshold
  HPGeUKMSNDetectorHit *newHit = new HPGeUKMSNDetectorHit();
  newHit->SetEdep(edep);
  fHitsCollection->insert(newHit);
  return true;
}


void HPGeUKMSNDetectorSD::EndOfEvent(G4HCofThisEvent *hce)
{
  if (verboseLevel > 1) {
    G4int nHits = fHitsCollection->entries();
    G4cout << "Hits collection:  Hit count = " << nHits << G4endl;
    for (G4int i = 0; i < nHits; ++i)
      (*fHitsCollection)[i]->Print();
  }
}

