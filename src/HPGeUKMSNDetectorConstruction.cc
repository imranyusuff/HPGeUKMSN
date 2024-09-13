#include "HPGeUKMSNDetectorConstruction.hh"
#include "HPGeUKMSNDetectorSD.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

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
  DefineCommands();
}


HPGeUKMSNDetectorConstruction::~HPGeUKMSNDetectorConstruction()
{
  delete fMessenger;
}


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
  fDetHolderMaterial = nistManager->FindOrBuildMaterial("G4_Al");
  fEndcapMaterial = nistManager->FindOrBuildMaterial("G4_Al");
  fEndcapTopMaterial = nistManager->FindOrBuildMaterial("G4_Al");
  fStopMaterial = nistManager->FindOrBuildMaterial("G4_Pb");
  fSrcMaterial = nistManager->FindOrBuildMaterial("G4_PLEXIGLASS");
  fStandMaterial = nistManager->FindOrBuildMaterial("G4_PLEXIGLASS");
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

  const double detectorRadius = 29.5*mm;
  //const double detectorHeight = 207.4*mm;
  const double detectorHeight = 50.0*mm;

  const double detectorHolderInnerRadius = 29.5*mm;
  const double detectorHolderOuterRadius = 31.0*mm;
  const double detectorHolderHeight = 202.4*mm;

  const double endcapOuterRadius = 71.5*mm;
  const double endcapThickness   =  1.5*mm;
  const double endcapHeight      = 105*mm;  /* endcap + baseShield = 105 + 102.4 = 207.4 */

  const double endcapTopThickness = 1.5*mm;

  const double sourceRadius = 25.4*mm;
  const double sourceHeight = 3.175*mm;

  const double cylStandOuterRadius   = 70*mm;
  const double cylStandThickness     = 3*mm;
  const double cylStandHeight        = 270*mm;
  const double cylSrcHolderThickness = 3*mm;

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
  G4ThreeVector det3V = G4ThreeVector(0, detectorHolderHeight - detectorHeight/2, 0);
  G4Transform3D detTR = G4Transform3D(detRM, det3V);
  G4VPhysicalVolume *detPV = new G4PVPlacement(detTR, detLV, "Detector", worldLV, false, 0, fCheckOverlaps);

  G4Tubs *detHolderS = new G4Tubs("detectorHolder", detectorHolderInnerRadius, detectorHolderOuterRadius, detectorHolderHeight/2, 0.*deg, 360.*deg);
  G4LogicalVolume *detHolderLV = new G4LogicalVolume(detHolderS, fDetHolderMaterial, "DetectorHolder");
  G4RotationMatrix detHolderRM = G4RotationMatrix();
  detHolderRM.rotateX(90.*deg);
  G4ThreeVector detHolder3V = G4ThreeVector(0, detectorHolderHeight/2, 0);
  G4Transform3D detHolderTR = G4Transform3D(detHolderRM, detHolder3V);
  G4VPhysicalVolume *detHolderPV = new G4PVPlacement(detHolderTR, detHolderLV, "DetectorHolder", worldLV, false, 0, fCheckOverlaps);

  G4Tubs *endcapS = new G4Tubs("endcap", endcapOuterRadius - endcapThickness, endcapOuterRadius, endcapHeight/2, 0.*deg, 360.*deg);
  G4LogicalVolume *endcapLV = new G4LogicalVolume(endcapS, fEndcapMaterial, "Endcap");
  G4RotationMatrix endcapRM = G4RotationMatrix();
  endcapRM.rotateX(90.*deg);
  G4ThreeVector endcap3V = G4ThreeVector(0, baseShieldThickness + endcapHeight/2, 0);
  G4Transform3D endcapTR = G4Transform3D(endcapRM, endcap3V);
  G4VPhysicalVolume *endcapPV = new G4PVPlacement(endcapTR, endcapLV, "Endcap", worldLV, false, 0, fCheckOverlaps);

  G4Tubs *endcapTopS = new G4Tubs("endcapTop", 0, endcapOuterRadius, endcapTopThickness/2, 0.*deg, 360.*deg);
  G4LogicalVolume *endcapTopLV = new G4LogicalVolume(endcapTopS, fEndcapTopMaterial, "EndcapTop");
  G4RotationMatrix endcapTopRM = G4RotationMatrix();
  endcapTopRM.rotateX(90.*deg);
  G4ThreeVector endcapTop3V = G4ThreeVector(0, baseShieldThickness + endcapHeight + endcapTopThickness/2, 0);
  G4Transform3D endcapTopTR = G4Transform3D(endcapTopRM, endcapTop3V);
  G4VPhysicalVolume *endcapTopPV = new G4PVPlacement(endcapTopTR, endcapTopLV, "EndcapTop", worldLV, false, 0, fCheckOverlaps);

  G4Tubs *srcS = new G4Tubs("source", 0, sourceRadius, sourceHeight/2, 0.*deg, 360.*deg);
  G4LogicalVolume *srcLV = new G4LogicalVolume(srcS, fSrcMaterial, "Source");
  G4RotationMatrix srcRM = G4RotationMatrix();
  srcRM.rotateX(90.*deg);
  G4ThreeVector src3V = G4ThreeVector(0, (baseShieldThickness + endcapHeight + endcapTopThickness + fSrcBaseDistance + sourceHeight/2), 0);
  G4Transform3D srcTR = G4Transform3D(srcRM, src3V);
  fSrcPV = new G4PVPlacement(srcTR, srcLV, "Source", worldLV, false, 0, fCheckOverlaps);

  G4Tubs *cylStandS = new G4Tubs("cylindricalStand", cylStandOuterRadius - cylStandThickness, cylStandOuterRadius, cylStandHeight/2, 0.*deg, 360.*deg);
  G4LogicalVolume *cylStandLV = new G4LogicalVolume(cylStandS, fStandMaterial, "CylindricalStand");
  G4RotationMatrix cylStandRM = G4RotationMatrix();
  cylStandRM.rotateX(90.*deg);
  G4ThreeVector cylStand3V = G4ThreeVector(0, baseShieldThickness + endcapHeight + endcapTopThickness + cylStandHeight/2, 0);
  G4Transform3D cylStandTR = G4Transform3D(cylStandRM, cylStand3V);
  new G4PVPlacement(cylStandTR, cylStandLV, "CylindricalStand", worldLV, false, 0, fCheckOverlaps);

  G4Tubs *cylSrcHolderS = new G4Tubs("cylSrcHolder", 0, cylStandOuterRadius - cylStandThickness, cylSrcHolderThickness/2, 0.*deg, 360.*deg);
  G4LogicalVolume *cylSrcHolderLV = new G4LogicalVolume(cylSrcHolderS, fStandMaterial, "CylStandSrcHolder");
  G4RotationMatrix cylSrcHolderRM = G4RotationMatrix();
  cylSrcHolderRM.rotateX(90.*deg);
  G4ThreeVector cylSrcHolder3V = G4ThreeVector(0, baseShieldThickness + endcapHeight + endcapTopThickness + fSrcBaseDistance - cylSrcHolderThickness/2, 0);
  G4Transform3D cylSrcHolderTR = G4Transform3D(cylSrcHolderRM, cylSrcHolder3V);
  fCylSrcHolderPV = new G4PVPlacement(cylSrcHolderTR, cylSrcHolderLV, "CylStandSrcHolder", worldLV, false, 0, fCheckOverlaps);

  G4VisAttributes invisible(G4VisAttributes::GetInvisible());
  G4VisAttributes red(G4Colour::Red());
  G4VisAttributes green(G4Colour::Green());
  G4VisAttributes blue(G4Colour::Blue());
  G4VisAttributes yellow(G4Colour::Yellow());
  G4VisAttributes magenta(G4Colour::Magenta());
  G4VisAttributes cyan(G4Colour::Cyan());

  worldLV->SetVisAttributes(invisible);
  baseshieldLV->SetVisAttributes(red);
  bodyshieldLV->SetVisAttributes(red);
  doorshieldLV->SetVisAttributes(magenta);
  detLV->SetVisAttributes(cyan);
  detHolderLV->SetVisAttributes(blue);
  endcapLV->SetVisAttributes(blue);
  endcapTopLV->SetVisAttributes(blue);
  srcLV->SetVisAttributes(yellow);
  cylStandLV->SetVisAttributes(green);
  cylSrcHolderLV->SetVisAttributes(green);

  return worldPV;
}


void HPGeUKMSNDetectorConstruction::ConstructSDandField()
{
  HPGeUKMSNDetectorSD *detSD = new HPGeUKMSNDetectorSD("/Detector", "DetectorHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(detSD);
  SetSensitiveDetector("Detector", detSD, true);
}


void HPGeUKMSNDetectorConstruction::SetSourceBaseDistance(G4double distance)
{
  if (!fSrcPV || !fCylSrcHolderPV) {
      G4cerr << "Detector has not yet been constructed." << G4endl;
      return;
  }

  const double oldDistance = fSrcBaseDistance;
  fSrcBaseDistance = distance;
  fSrcPV->SetTranslation(G4ThreeVector(
    0, fSrcPV->GetTranslation().y() - oldDistance + fSrcBaseDistance, 0));
  fCylSrcHolderPV->SetTranslation(G4ThreeVector(
    0, fCylSrcHolderPV->GetTranslation().y() - oldDistance + fSrcBaseDistance, 0));

  // tell G4RunManager that we change the geometry
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}


void HPGeUKMSNDetectorConstruction::DefineCommands()
{
  fMessenger = new G4GenericMessenger(this, "/hpge/source/", "Source control");

  auto& srcDistCmd = fMessenger->DeclareMethodWithUnit("baseDistance", "mm",
                       &HPGeUKMSNDetectorConstruction::SetSourceBaseDistance,
                       "Distance from base of holder to base of source (default mm).");
  srcDistCmd.SetParameterName("distance", true);
  srcDistCmd.SetRange("distance>=0.");
  srcDistCmd.SetDefaultValue("180.");
}

