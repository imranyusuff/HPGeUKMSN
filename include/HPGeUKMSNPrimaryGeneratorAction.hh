#ifndef HPGeUKMSNPrimaryGeneratorAction_h
#define HPGeUKMSNPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"


class G4ParticleGun;
class G4Event;


class HPGeUKMSNPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    HPGeUKMSNPrimaryGeneratorAction();
    virtual ~HPGeUKMSNPrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event *);

    G4ParticleGun *GetParticleGun() { return fParticleGun; }

  private:
    G4ParticleGun *fParticleGun;
};


#endif

