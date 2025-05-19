#ifndef HPGeUKMSNActionInitialization_h
#define HPGeUKMSNActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "G4Types.hh"


class HPGeUKMSNActionInitialization : public G4VUserActionInitialization
{
  public:
    HPGeUKMSNActionInitialization(G4int geometrySelection);
    virtual ~HPGeUKMSNActionInitialization();

    virtual void Build() const;

  private:
    G4int fGeometrySelection;
};


#endif

