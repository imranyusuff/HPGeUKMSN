#include "HPGeUKMSNPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "Randomize.hh"


HPGeUKMSNPrimaryGeneratorAction::HPGeUKMSNPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction()
{
  G4int nParticles = 1;
  fParticleGun = new G4ParticleGun(nParticles);

  G4ParticleDefinition *particleDefinition
    = G4ParticleTable::GetParticleTable()->FindParticle("gamma");

  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1., 0., 0.));
  fParticleGun->SetParticleEnergy(1.46 * MeV);
}


HPGeUKMSNPrimaryGeneratorAction::~HPGeUKMSNPrimaryGeneratorAction()
{
  delete fParticleGun;
}


void HPGeUKMSNPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
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
  //G4LogicalVolume *srcLV = srcPV->GetLogicalVolume();
  //G4Tubs *srcS = dynamic_cast<G4Tubs *>(srcLV->GetSolid());
  //G4double srcRadius = srcS->GetOuterRadius();
  //G4double srcZov2 = srcS->GetZHalfLength();
  //G4double genTheta = 2. * CLHEP::pi * G4UniformRand();
  //G4double genR = std::sqrt(G4UniformRand()) * srcRadius;
  //genX = genR * std::cos(genTheta);
  //genY = genR * std::sin(genTheta);
  //genZ = 2. * G4UniformRand() * srcZov2 - srcZov2;
  //const G4ThreeVector genPos = src3V + G4ThreeVector(genX, genY, genZ);
  const G4ThreeVector genPos = src3V;
  fParticleGun->SetParticlePosition(genPos);

  // Now generate the gamma ray photon
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

