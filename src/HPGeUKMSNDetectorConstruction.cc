#include "HPGeUKMSNDetectorConstruction.hh"
#include "HPGeUKMSNDetectorSD.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4SDManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"


HPGeUKMSNDetectorConstruction::HPGeUKMSNDetectorConstruction()
: G4VUserDetectorConstruction(),
  fCheckOverlaps(true)
{
}


HPGeUKMSNDetectorConstruction::~HPGeUKMSNDetectorConstruction()
{}


G4VPhysicalVolume *HPGeUKMSNDetectorConstruction::Construct()
{
  DefineMaterials();
  return DefineVolumes();
}


void HPGeUKMSNDetectorConstruction::DefineMaterials()
{
  G4NistManager *nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_AIR");
  fDetMaterial = nistManager->FindOrBuildMaterial("G4_Ge");
  fStopMaterial = nistManager->FindOrBuildMaterial("G4_Pb");
  G4Element *elK =  new G4Element("Potassium", "K",  19., 39.0983*g/mole);
  G4Element *elCl = new G4Element("Chlorine",  "Cl", 17., 35.45*g/mole);
  G4Material *mKCl = new G4Material("PotassiumChloride", 1.984*g/cm3, 2);
  mKCl->AddElement(elK, 1);
  mKCl->AddElement(elCl, 1);
  fSrcMaterial = mKCl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}


G4VPhysicalVolume *HPGeUKMSNDetectorConstruction::DefineVolumes()
{
  const double worldSizeX = 20.*m;
  const double worldSizeY = 40.*m;
  const double worldSizeZ = 20.*m;

  const double shieldRadius = 244.5*mm;

  const double baseShieldInnerRadius = 63.1*mm;
  const double baseShieldThickness   = 102.4*mm;

  const double bodyShieldInnerRadius = 142.1*mm;
  const double bodyShieldHeight      = 408.6*mm;

  const double doorShieldInnerRadius = 0.0;
  const double doorShieldThickness   = baseShieldThickness;

  const double detectorRadius = 35*mm;
  const double detectorHeight = 207.4*mm;

  G4Material *air = G4Material::GetMaterial("G4_AIR");

  //G4GeometryManager::GetInstance()->SetWorldMaximumExtent(20*cm);

  G4Box *worldS = new G4Box("world", worldSizeX/2, worldSizeY/2, worldSizeZ/2);
  G4LogicalVolume *worldLV = new G4LogicalVolume(worldS, air, "World");
  G4VPhysicalVolume *worldPV = new G4PVPlacement(0, G4ThreeVector(), worldLV, "World", 0, false, fCheckOverlaps);

  G4Tubs *baseshieldS = new G4Tubs("baseShield", baseShieldInnerRadius, shieldRadius, baseShieldThickness/2, 0.*deg, 360.*deg);
  G4LogicalVolume *baseshieldLV = new G4LogicalVolume(baseshieldS, fStopMaterial, "BaseShield");
  G4RotationMatrix baseshieldRM = G4RotationMatrix();
  baseshieldRM.rotateX(90.*deg);
  G4ThreeVector baseshield3V = G4ThreeVector(0, baseShieldThickness/2, 0);
  G4Transform3D baseshieldTR = G4Transform3D(baseshieldRM, baseshield3V);
  G4VPhysicalVolume *baseshieldPV = new G4PVPlacement(baseshieldTR, baseshieldLV, "BaseShield", worldLV, false, 0, fCheckOverlaps);

  G4Tubs *bodyshieldS = new G4Tubs("bodyShield", bodyShieldInnerRadius, shieldRadius, bodyShieldHeight/2, 0.*deg, 360.*deg);
  G4LogicalVolume *bodyshieldLV = new G4LogicalVolume(bodyshieldS, fStopMaterial, "BodyShield");
  G4RotationMatrix bodyshieldRM = G4RotationMatrix();
  bodyshieldRM.rotateX(90.*deg);
  G4ThreeVector bodyshield3V = G4ThreeVector(0, baseShieldThickness + bodyShieldHeight/2, 0);
  G4Transform3D bodyshieldTR = G4Transform3D(bodyshieldRM, bodyshield3V);
  G4VPhysicalVolume *bodyshieldPV = new G4PVPlacement(bodyshieldTR, bodyshieldLV, "BodyShield", worldLV, false, 0, fCheckOverlaps);

  G4Tubs *doorshieldS = new G4Tubs("doorShield", doorShieldInnerRadius, shieldRadius, doorShieldThickness/2, 0.*deg, 360.*deg);
  G4LogicalVolume *doorshieldLV = new G4LogicalVolume(doorshieldS, fStopMaterial, "DoorShield");
  G4RotationMatrix doorshieldRM = G4RotationMatrix();
  doorshieldRM.rotateX(90.*deg);
  G4ThreeVector doorshield3V = G4ThreeVector(0, baseShieldThickness + bodyShieldHeight + doorShieldThickness/2, 0);
  G4Transform3D doorshieldTR = G4Transform3D(doorshieldRM, doorshield3V);
  G4VPhysicalVolume *doorshieldPV = new G4PVPlacement(doorshieldTR, doorshieldLV, "DoorShield", worldLV, false, 0, fCheckOverlaps);

  G4Tubs *detS = new G4Tubs("detector", 0, detectorRadius, detectorHeight/2, 0.*deg, 360.*deg);
  G4LogicalVolume *detLV = new G4LogicalVolume(detS, fDetMaterial, "Detector");
  G4RotationMatrix detRM = G4RotationMatrix();
  detRM.rotateX(90.*deg);
  G4ThreeVector det3V = G4ThreeVector(0, detectorHeight/2, 0);
  G4Transform3D detTR = G4Transform3D(detRM, det3V);
  G4VPhysicalVolume *detPV = new G4PVPlacement(detTR, detLV, "Detector", worldLV, false, 0, fCheckOverlaps);

  G4Tubs *srcS = new G4Tubs("source", 0, 20.*mm, 35.*mm, 0.*deg, 360.*deg);
  G4LogicalVolume *srcLV = new G4LogicalVolume(srcS, fSrcMaterial, "Source");
  G4RotationMatrix srcRM = G4RotationMatrix();
  G4ThreeVector src3V = G4ThreeVector(0, (detectorHeight + 20.)*mm, 0);
  G4Transform3D srcTR = G4Transform3D(srcRM, src3V);
  new G4PVPlacement(srcTR, srcLV, "Source", worldLV, false, 0, fCheckOverlaps);

  worldLV->SetVisAttributes(G4VisAttributes::GetInvisible());
  baseshieldLV->SetVisAttributes(new G4VisAttributes(G4Colour(1.0, 0.0, 0.0)));
  bodyshieldLV->SetVisAttributes(new G4VisAttributes(G4Colour(1.0, 0.0, 0.0)));
  doorshieldLV->SetVisAttributes(new G4VisAttributes(G4Colour(1.0, 0.0, 1.0)));
  detLV->SetVisAttributes(new G4VisAttributes(G4Colour(0.0, 1.0, 1.0)));
  srcLV->SetVisAttributes(new G4VisAttributes(G4Colour(1.0, 1.0, 0.0)));

  return worldPV;
}


void HPGeUKMSNDetectorConstruction::ConstructSDandField()
{
  HPGeUKMSNDetectorSD *detSD = new HPGeUKMSNDetectorSD("/Detector", "DetectorHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(detSD);
  SetSensitiveDetector("Detector", detSD, true);
}

