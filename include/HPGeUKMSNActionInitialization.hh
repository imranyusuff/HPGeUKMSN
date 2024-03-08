#ifndef HPGeUKMSNActionInitialization_h
#define HPGeUKMSNActionInitialization_h 1

#include "G4VUserActionInitialization.hh"


class HPGeUKMSNActionInitialization : public G4VUserActionInitialization
{
  public:
    HPGeUKMSNActionInitialization();
    virtual ~HPGeUKMSNActionInitialization();

    virtual void Build() const;
};


#endif

