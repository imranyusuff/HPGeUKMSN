#ifndef HPGeUKMSNPrimaryGeneratorAction_h
#define HPGeUKMSNPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Types.hh"


class G4ParticleGun;
class G4Event;


class HPGeUKMSNPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    HPGeUKMSNPrimaryGeneratorAction(G4int geometrySelection);
    virtual ~HPGeUKMSNPrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event *);

    G4ParticleGun *GetParticleGun() { return fParticleGun; }

  private:
    G4ParticleGun *fParticleGun;
    G4int          fGeometrySelection;
};


#endif

