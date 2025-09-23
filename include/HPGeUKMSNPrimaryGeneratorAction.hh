#ifndef HPGeUKMSNPrimaryGeneratorAction_h
#define HPGeUKMSNPrimaryGeneratorAction_h 1

#include <vector>
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Types.hh"


class G4ParticleGun;
class G4Event;


class HPGeUKMSNPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    HPGeUKMSNPrimaryGeneratorAction(G4int geometrySelection, G4bool alwaysSingleSource);
    virtual ~HPGeUKMSNPrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event *);

    G4ParticleGun *GetParticleGun() { return fParticleGun; }

  private:
    G4ParticleGun *fParticleGun;
    G4int          fGeometrySelection;
    G4bool         fAlwaysSingleSource;

    std::vector<G4int> fIAEA375IsotopesZ;
    std::vector<G4int> fIAEA375IsotopesA;
    std::vector<G4double> fIAEA375IsotopesI;
    std::vector<G4double> fIAEA375IsotopesC;
    std::vector<G4double> fIAEA375IsotopesP;
    G4double fIAEA375IsotopesSumI;
};


#endif

