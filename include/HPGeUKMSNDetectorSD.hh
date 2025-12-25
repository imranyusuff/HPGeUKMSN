#ifndef HPGeUKMSNDetectorSD_h
#define HPGeUKMSNDetectorSD_h 1

#include "G4VSensitiveDetector.hh"

#include "HPGeUKMSNDetectorHit.hh"
#include "G4Navigator.hh"


class G4Step;
class G4HCofThisEvent;


class HPGeUKMSNDetectorSD : public G4VSensitiveDetector
{
  public:
    HPGeUKMSNDetectorSD(const G4String & name,
                        const G4String & hitsCollectionName,
                        G4double deadLayerThickness);
    virtual ~HPGeUKMSNDetectorSD();

    virtual void Initialize(G4HCofThisEvent *hitCollection);
    virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);
    virtual void EndOfEvent(G4HCofThisEvent *hitCollection);

  private:
    HPGeUKMSNDetectorHitsCollection *fHitsCollection;
    G4Navigator *fNavigator = nullptr;
    G4double fDeadLayerThickness;
};


#endif

