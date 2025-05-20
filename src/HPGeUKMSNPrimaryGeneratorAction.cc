#include "HPGeUKMSNPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4IonTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "Randomize.hh"


HPGeUKMSNPrimaryGeneratorAction::HPGeUKMSNPrimaryGeneratorAction(G4int geometrySelection)
 : G4VUserPrimaryGeneratorAction(),
   fGeometrySelection(geometrySelection)
{
  G4int nParticles = 1;
  fParticleGun = new G4ParticleGun(nParticles);

  G4ParticleDefinition *particleDefinition
    = G4ParticleTable::GetParticleTable()->FindParticle("gamma");

  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1., 0., 0.));
  fParticleGun->SetParticleEnergy(0.661657 * MeV);

  // if soil geometry then populate constituent isotope table
  if (fGeometrySelection == 2 ||
      fGeometrySelection == 2000 ||
      fGeometrySelection == 2001 ||
      fGeometrySelection == 2002) {
    // Information from https://analytical-reference-materials.iaea.org/iaea-375
    // Reference decay rates at 1991-12-31  (TODO: extrapolate to desired date)
    //                     Cs   Cs   Sr    K   Ra   Th   Th    U    U   Pu   Pu   Am   Sb   Ru    I
    fIAEA375IsotopesZ = {  55,  55,  38,  19,  88,  90,  90,  92,  92,  94,  94,  95,  51,  44,  53 };
    fIAEA375IsotopesA = { 137, 134,  90,  40, 226, 228, 232, 234, 238, 239, 238, 241, 125, 106, 129 };
    fIAEA375IsotopesI = {5280, 463, 108, 424,  20,  21, 20.5, 25, 24.4, 0.3, 0.071, 0.13, 77, 56, 0.0017};
    fIAEA375IsotopesSumI = 0.0;
    // find activity sum and build cumulative activity
    for (auto activity : fIAEA375IsotopesI) {
      fIAEA375IsotopesSumI += activity;
      fIAEA375IsotopesC.push_back(fIAEA375IsotopesSumI);
    }
    // normalize cumulatives
    for (unsigned int i = 0; i < fIAEA375IsotopesC.size(); ++i) {
      fIAEA375IsotopesC[i] /= fIAEA375IsotopesSumI;
    }
  }
}


HPGeUKMSNPrimaryGeneratorAction::~HPGeUKMSNPrimaryGeneratorAction()
{
  delete fParticleGun;
}


void HPGeUKMSNPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  // Randomize source isotope for soil geometries
  if (fGeometrySelection == 2 ||
      fGeometrySelection == 2000 ||
      fGeometrySelection == 2001 ||
      fGeometrySelection == 2002) {
    G4IonTable* ionTable = G4IonTable::GetIonTable();
    G4double rand = G4UniformRand();
    G4ParticleDefinition* ion = nullptr;
    // pick which isotope will decay
    const int iMax = fIAEA375IsotopesC.size() - 1;
    int iDecay = -1;
    do rand -= fIAEA375IsotopesC[++iDecay];
    while (rand > 0. && iDecay < iMax);
    ion = ionTable->GetIon(fIAEA375IsotopesZ[iDecay], fIAEA375IsotopesA[iDecay], 0.0);
    fParticleGun->SetParticleDefinition(ion);
    fParticleGun->SetParticleEnergy(0.0);  // Energy is ignored for decaying ions
  }

  // Isotropic gamma ray photon direction
  G4double theta = std::acos(2. * G4UniformRand() - 1.0);
  G4double phi = 2. * CLHEP::pi * G4UniformRand();
  G4double genX = std::sin(theta) * std::cos(phi);
  G4double genY = std::sin(theta) * std::sin(phi);
  G4double genZ = std::cos(theta);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(genX, genY, genZ));

  // Generate gamma ray photon uniformly and randomly within source
  G4VPhysicalVolume *srcPV = G4PhysicalVolumeStore::GetInstance()->GetVolume("Source");
  const G4ThreeVector src3V = srcPV->GetObjectTranslation();
  G4ThreeVector displacement;
  if (fGeometrySelection == 2 ||
      fGeometrySelection == 2000 ||
      fGeometrySelection == 2001 ||
      fGeometrySelection == 2002) {
    G4LogicalVolume *srcLV = srcPV->GetLogicalVolume();
    G4Tubs *srcS = dynamic_cast<G4Tubs *>(srcLV->GetSolid());
    G4double srcRadius = srcS->GetOuterRadius();
    G4double srcZov2 = srcS->GetZHalfLength();
    G4double genTheta = 2. * CLHEP::pi * G4UniformRand();
    G4double genR = std::sqrt(G4UniformRand()) * srcRadius;
    genX = genR * std::cos(genTheta);
    genY = genR * std::sin(genTheta);
    genZ = 2. * G4UniformRand() * srcZov2 - srcZov2;
    displacement.set(genX, genY, genZ);
  }
  const G4ThreeVector genPos = src3V + displacement;
  fParticleGun->SetParticlePosition(genPos);

  // Now generate the gamma ray photon
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

