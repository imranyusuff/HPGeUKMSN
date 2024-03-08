#ifndef HPGeUKMSNEventAction_h
#define HPGeUKMSNEventAction_h 1

#include "HPGeUKMSNDetectorHit.hh"

#include "G4UserEventAction.hh"


class HPGeUKMSNRunAction;


class HPGeUKMSNEventAction : public G4UserEventAction
{
  public:
    HPGeUKMSNEventAction(HPGeUKMSNRunAction *runAction);
    virtual ~HPGeUKMSNEventAction();

    virtual void BeginOfEventAction(const G4Event *event);
    virtual void   EndOfEventAction(const G4Event *event);

  private:
    HPGeUKMSNDetectorHitsCollection *GetHitsCollection(G4int hcID, const G4Event *event) const;

    HPGeUKMSNRunAction *fRunAction;

    G4double fEdep;
    G4int fDetHCID;
};


#endif

