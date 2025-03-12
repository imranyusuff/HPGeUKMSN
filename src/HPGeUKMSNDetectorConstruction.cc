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


HPGeUKMSNDetectorConstruction::HPGeUKMSNDetectorConstruction(G4int geometrySelection)
: G4VUserDetectorConstruction(),
  fGeometrySelection(geometrySelection),
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

  const double endcapOuterRadius = (71.5*mm)/2;
  const double endcapThickness   =  1.5*mm;
  const double endcapHeight      = 105*mm;  /* endcap + baseShield = 105 + 102.4 = 207.4 */

  const double endcapTopThickness = 1.5*mm;

  G4Material *air = G4Material::GetMaterial("G4_AIR");

  //G4GeometryManager::GetInstance()->SetWorldMaximumExtent(20*cm);

  G4Box *worldS = new G4Box("world", worldSizeX/2, worldSizeY/2, worldSizeZ/2);
  G4LogicalVolume *worldLV = new G4LogicalVolume(worldS, air, "World");
  G4VPhysicalVolume *worldPV = new G4PVPlacement(nullptr, G4ThreeVector(), worldLV, "World", 0, false, fCheckOverlaps);

  G4Tubs *baseshieldS = new G4Tubs("baseShield", baseShieldInnerRadius, shieldRadius, baseShieldThickness/2, 0.*deg, 360.*deg);
  G4LogicalVolume *baseshieldLV = new G4LogicalVolume(baseshieldS, fStopMaterial, "BaseShield");
  G4ThreeVector baseshieldPos = G4ThreeVector(0, 0, baseShieldThickness/2);
  G4VPhysicalVolume *baseshieldPV = new G4PVPlacement(nullptr, baseshieldPos, baseshieldLV, "BaseShield", worldLV, false, 0, fCheckOverlaps);

  G4Tubs *bodyshieldS = new G4Tubs("bodyShield", bodyShieldInnerRadius, shieldRadius, bodyShieldHeight/2, 0.*deg, 360.*deg);
  G4LogicalVolume *bodyshieldLV = new G4LogicalVolume(bodyshieldS, fStopMaterial, "BodyShield");
  G4ThreeVector bodyshieldPos = G4ThreeVector(0, 0, baseShieldThickness + bodyShieldHeight/2);
  G4VPhysicalVolume *bodyshieldPV = new G4PVPlacement(nullptr, bodyshieldPos, bodyshieldLV, "BodyShield", worldLV, false, 0, fCheckOverlaps);

  G4Tubs *doorshieldS = new G4Tubs("doorShield", doorShieldInnerRadius, shieldRadius, doorShieldThickness/2, 0.*deg, 360.*deg);
  G4LogicalVolume *doorshieldLV = new G4LogicalVolume(doorshieldS, fStopMaterial, "DoorShield");
  G4ThreeVector doorshieldPos = G4ThreeVector(0, 0, baseShieldThickness + bodyShieldHeight + doorShieldThickness/2);
  G4VPhysicalVolume *doorshieldPV = new G4PVPlacement(nullptr, doorshieldPos, doorshieldLV, "DoorShield", worldLV, false, 0, fCheckOverlaps);

  G4Tubs *detS = new G4Tubs("detector", 0, detectorRadius, detectorHeight/2, 0.*deg, 360.*deg);
  G4LogicalVolume *detLV = new G4LogicalVolume(detS, fDetMaterial, "Detector");
  G4ThreeVector detPos = G4ThreeVector(0, 0, detectorHolderHeight - detectorHeight/2);
  G4VPhysicalVolume *detPV = new G4PVPlacement(nullptr, detPos, detLV, "Detector", worldLV, false, 0, fCheckOverlaps);

  G4Tubs *detHolderS = new G4Tubs("detectorHolder", detectorHolderInnerRadius, detectorHolderOuterRadius, detectorHolderHeight/2, 0.*deg, 360.*deg);
  G4LogicalVolume *detHolderLV = new G4LogicalVolume(detHolderS, fDetHolderMaterial, "DetectorHolder");
  G4ThreeVector detHolderPos = G4ThreeVector(0, 0, detectorHolderHeight/2);
  G4VPhysicalVolume *detHolderPV = new G4PVPlacement(nullptr, detHolderPos, detHolderLV, "DetectorHolder", worldLV, false, 0, fCheckOverlaps);

  G4Tubs *endcapS = new G4Tubs("endcap", endcapOuterRadius - endcapThickness, endcapOuterRadius, endcapHeight/2, 0.*deg, 360.*deg);
  G4LogicalVolume *endcapLV = new G4LogicalVolume(endcapS, fEndcapMaterial, "Endcap");
  G4ThreeVector endcapPos = G4ThreeVector(0, 0, baseShieldThickness + endcapHeight/2);
  G4VPhysicalVolume *endcapPV = new G4PVPlacement(nullptr, endcapPos, endcapLV, "Endcap", worldLV, false, 0, fCheckOverlaps);

  G4Tubs *endcapTopS = new G4Tubs("endcapTop", 0, endcapOuterRadius, endcapTopThickness/2, 0.*deg, 360.*deg);
  G4LogicalVolume *endcapTopLV = new G4LogicalVolume(endcapTopS, fEndcapTopMaterial, "EndcapTop");
  G4ThreeVector endcapTopPos = G4ThreeVector(0, 0, baseShieldThickness + endcapHeight + endcapTopThickness/2);
  G4VPhysicalVolume *endcapTopPV = new G4PVPlacement(nullptr, endcapTopPos, endcapTopLV, "EndcapTop", worldLV, false, 0, fCheckOverlaps);

  if (fGeometrySelection == 0) {
    DefineExperimentGeometry0(worldLV, baseShieldThickness, endcapHeight, endcapTopThickness);
  }
  else if (fGeometrySelection == 1) {
    DefineExperimentGeometry1(worldLV, baseShieldThickness);
  }
  else {
    G4cout << "Geometry selection error." << G4endl;
    exit(1);    // TODO this is too harsh
  }

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

  return worldPV;
}


void HPGeUKMSNDetectorConstruction::DefineExperimentGeometry0(
  G4LogicalVolume * const worldLV,
  const double baseShieldThickness,
  const double endcapHeight,
  const double endcapTopThickness)
{
  const double sourceRadius = 25.4*mm;
  const double sourceHeight = 3.175*mm;

  const double cylStandOuterRadius   = 70*mm;
  const double cylStandThickness     = 3*mm;
  const double cylStandHeight        = 270*mm;
  const double cylSrcHolderThickness = 3*mm;

  G4Tubs *srcS = new G4Tubs("source", 0, sourceRadius, sourceHeight/2, 0.*deg, 360.*deg);
  G4LogicalVolume *srcLV = new G4LogicalVolume(srcS, fSrcMaterial, "Source");
  G4ThreeVector srcPos = G4ThreeVector(0, 0, (baseShieldThickness + endcapHeight + endcapTopThickness + fSrcBaseDistance + sourceHeight/2));
  fSrcPV = new G4PVPlacement(nullptr, srcPos, srcLV, "Source", worldLV, false, 0, fCheckOverlaps);

  G4Tubs *cylStandS = new G4Tubs("cylindricalStand", cylStandOuterRadius - cylStandThickness, cylStandOuterRadius, cylStandHeight/2, 0.*deg, 360.*deg);
  G4LogicalVolume *cylStandLV = new G4LogicalVolume(cylStandS, fStandMaterial, "CylindricalStand");
  G4ThreeVector cylStandPos = G4ThreeVector(0, 0, baseShieldThickness + endcapHeight + endcapTopThickness + cylStandHeight/2);
  new G4PVPlacement(nullptr, cylStandPos, cylStandLV, "CylindricalStand", worldLV, false, 0, fCheckOverlaps);

  G4Tubs *cylSrcHolderS = new G4Tubs("cylSrcHolder", 0, cylStandOuterRadius - cylStandThickness, cylSrcHolderThickness/2, 0.*deg, 360.*deg);
  G4LogicalVolume *cylSrcHolderLV = new G4LogicalVolume(cylSrcHolderS, fStandMaterial, "CylStandSrcHolder");
  G4ThreeVector cylSrcHolderPos = G4ThreeVector(0, 0, baseShieldThickness + endcapHeight + endcapTopThickness + fSrcBaseDistance - cylSrcHolderThickness/2);
  fCylSrcHolderPV = new G4PVPlacement(nullptr, cylSrcHolderPos, cylSrcHolderLV, "CylStandSrcHolder", worldLV, false, 0, fCheckOverlaps);

  G4VisAttributes yellow(G4Colour::Yellow());
  G4VisAttributes green(G4Colour::Green());

  srcLV->SetVisAttributes(yellow);
  cylStandLV->SetVisAttributes(green);
  cylSrcHolderLV->SetVisAttributes(green);
}


void HPGeUKMSNDetectorConstruction::DefineExperimentGeometry1(
  G4LogicalVolume * const worldLV,
  const double baseShieldThickness
)
{
  fSrcBaseDistance = 315*mm;

  const double sourceRadius = 25.4*mm;
  const double sourceHeight = 3.175*mm;

  const double cubSrcHolderThickness = 3*mm;

  const double cubStandLength        = 110*mm;
  const double cubStandThickness     = 4*mm;
  const double cubStandHeight        = fSrcBaseDistance - cubSrcHolderThickness;

  const double cubStandBackThickness = 5*mm;
  const double cubStandBarHeight     = 39*mm;

  G4Tubs *srcS = new G4Tubs("source", 0, sourceRadius, sourceHeight/2, 0.*deg, 360.*deg);
  G4LogicalVolume *srcLV = new G4LogicalVolume(srcS, fSrcMaterial, "Source");
  G4ThreeVector srcPos = G4ThreeVector(0, 0, (baseShieldThickness + fSrcBaseDistance + sourceHeight/2));
  fSrcPV = new G4PVPlacement(nullptr, srcPos, srcLV, "Source", worldLV, false, 0, fCheckOverlaps);

  G4Box *cubStandSideS = new G4Box("cubStandSide", cubStandThickness/2, cubStandLength/2, cubStandHeight/2); //for both sides

  G4LogicalVolume *cubStandSide1LV = new G4LogicalVolume(cubStandSideS, fStandMaterial, "CubicalStandSide1");
  G4ThreeVector cubStandSide1Pos = G4ThreeVector(cubStandLength/2 - cubStandThickness/2, 0, baseShieldThickness + cubStandHeight/2);
  new G4PVPlacement(nullptr, cubStandSide1Pos, cubStandSide1LV, "CubicalStandSide1", worldLV, false, 0, fCheckOverlaps);

  G4LogicalVolume *cubStandSide2LV = new G4LogicalVolume(cubStandSideS, fStandMaterial, "CubicalStandSide2");
  G4ThreeVector cubStandSide2Pos = G4ThreeVector(-(cubStandLength/2 - cubStandThickness/2), 0, baseShieldThickness + cubStandHeight/2);
  new G4PVPlacement(nullptr, cubStandSide2Pos, cubStandSide2LV, "CubicalStandSide2", worldLV, false, 0, fCheckOverlaps);

  G4Box *cubStandBackS = new G4Box("cubStandBack", cubStandLength/2, cubStandBackThickness/2, cubStandHeight/2);
  G4LogicalVolume *cubStandBackLV = new G4LogicalVolume(cubStandBackS, fStandMaterial, "CubicalStandBack");
  G4ThreeVector cubStandBackPos = G4ThreeVector(0, cubStandLength/2 + cubStandBackThickness/2, baseShieldThickness + cubStandHeight/2);
  new G4PVPlacement(nullptr, cubStandBackPos, cubStandBackLV, "CubicalStandBack", worldLV, false, 0, fCheckOverlaps);

  G4Box *cubStandBarS = new G4Box("cubStandBar", cubStandLength/2, cubStandBackThickness/2, cubStandBarHeight/2);
  G4LogicalVolume *cubStandBarLV = new G4LogicalVolume(cubStandBarS, fStandMaterial, "CubicalStandBar");
  G4ThreeVector cubStandBarPos = G4ThreeVector(0, -(cubStandLength/2 + cubStandBackThickness/2), baseShieldThickness + cubStandBarHeight/2);
  new G4PVPlacement(nullptr, cubStandBarPos, cubStandBarLV, "CubicalStandBar", worldLV, false, 0, fCheckOverlaps);

  G4Box *cubSrcHolderS = new G4Box("cubSrcHolder", cubStandLength/2, cubStandLength/2, cubSrcHolderThickness/2);
  G4LogicalVolume *cubSrcHolderLV = new G4LogicalVolume(cubSrcHolderS, fStandMaterial, "CubStandSrcHolder");
  G4ThreeVector cubSrcHolderPos = G4ThreeVector(0, 0, baseShieldThickness + cubStandHeight + cubSrcHolderThickness/2);
  new G4PVPlacement(nullptr, cubSrcHolderPos, cubSrcHolderLV, "CubStandSrcHolder", worldLV, false, 0, fCheckOverlaps);

  G4VisAttributes yellow(G4Colour::Yellow());
  G4VisAttributes green(G4Colour::Green());

  srcLV->SetVisAttributes(yellow);
  //cubStandLV->SetVisAttributes(green);
  cubStandSide1LV->SetVisAttributes(green);
  cubStandSide2LV->SetVisAttributes(green);
  cubStandBackLV->SetVisAttributes(green);
  cubStandBarLV->SetVisAttributes(green);
  cubSrcHolderLV->SetVisAttributes(green);
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
    0, 0, fSrcPV->GetTranslation().z() - oldDistance + fSrcBaseDistance));
  fCylSrcHolderPV->SetTranslation(G4ThreeVector(
    0, 0, fCylSrcHolderPV->GetTranslation().z() - oldDistance + fSrcBaseDistance));

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

