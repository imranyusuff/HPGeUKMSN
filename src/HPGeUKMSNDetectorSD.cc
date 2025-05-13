#include "HPGeUKMSNDetectorSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"


HPGeUKMSNDetectorSD::HPGeUKMSNDetectorSD(const G4String & name, const G4String & hitsCollectionName)
 : G4VSensitiveDetector(name),
   fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
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
  const G4double threshold = 30.*CLHEP::keV;       // HPGe detection threshold
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

