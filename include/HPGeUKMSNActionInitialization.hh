#ifndef HPGeUKMSNActionInitialization_h
#define HPGeUKMSNActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "G4String.hh"
#include "G4Types.hh"


class HPGeUKMSNActionInitialization : public G4VUserActionInitialization
{
  public:
    HPGeUKMSNActionInitialization(G4String outfnPrefix, G4int geometrySelection, G4bool alwaysSingleSource);
    virtual ~HPGeUKMSNActionInitialization();

    virtual void Build() const;

  private:
    G4String fOutfnPrefix;
    G4int fGeometrySelection;
    G4bool fAlwaysSingleSource;
};


#endif

