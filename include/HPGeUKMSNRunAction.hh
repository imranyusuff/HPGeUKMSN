#ifndef HPGeUKMSNRunAction_h
#define gamamspec1RunAction_h 1

#include "G4UserRunAction.hh"


class G4Run;


class HPGeUKMSNRunAction : public G4UserRunAction
{
  public:
    HPGeUKMSNRunAction();
    virtual ~HPGeUKMSNRunAction();

    virtual void BeginOfRunAction(const G4Run *);
    virtual void   EndOfRunAction(const G4Run *);
};


#endif

