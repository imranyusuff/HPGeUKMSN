#ifndef HPGeUKMSNDetectorConstruction_h
#define HPGeUKMSNDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"


class G4VPhysicalVolume;
class G4GenericMessenger;
class G4Material;


class HPGeUKMSNDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    HPGeUKMSNDetectorConstruction(G4int geometrySelection,
                                  G4double deadLayerThicknessMM,
                                  G4bool detailedIRWindow = false);
    virtual ~HPGeUKMSNDetectorConstruction();

    virtual G4VPhysicalVolume *Construct();
    virtual void ConstructSDandField();

    void SetSourceBaseDistance(G4double distance);
    void SetSourceXYPos(G4double x, G4double y);
    void SetSoilDensity(G4double rho);
    G4double GetSourceBaseDistance() { return fSrcBaseDistance; }

  private:
    void DefineMaterials();
    void DefineSoilMaterial();

    G4VPhysicalVolume *DefineVolumes();

    void DefineExperimentGeometry0(
      G4LogicalVolume * const worldLV,
      const double baseShieldThickness,
      const double endcapHeight,
      const double endcapTopThickness
    );

    void DefineExperimentGeometry1(
      G4LogicalVolume * const worldLV,
      const double baseShieldThickness
    );

    void DefineExperimentGeometry2(
      G4LogicalVolume * const worldLV,
      const double baseShieldThickness,
      const double endcapHeight,
      const double endcapTopThickness,
      const int variant,
      const bool pointSourceOnTop,
      const bool noSoil
    );

    void DefineCommands();

    G4int fGeometrySelection;
    G4double fDeadLayerThickness;
    G4bool fDetailedIRWindow;

    G4GenericMessenger* fMessenger = nullptr;
    G4GenericMessenger* fSoilMessenger = nullptr;

    G4Material *fDetMaterial = nullptr;
    G4Material *fDetHolderMaterial = nullptr;
    G4Material *fDetIRKaptonMaterial = nullptr;
    G4Material *fDetIRMylarMaterial = nullptr;
    G4Material *fDetIRAlMaterial = nullptr;
    G4Material *fEndcapMaterial = nullptr;
    G4Material *fEndcapTopMaterial = nullptr;
    G4Material *fVacuumMaterial = nullptr;
    G4Material *fStopMaterial = nullptr;
    G4Material *fSrcMaterial = nullptr;
    G4Material *fStandMaterial = nullptr;
    G4Material *fContainerMaterial = nullptr;
    G4Material *fIAEA375SoilMaterial = nullptr;

    G4VPhysicalVolume *fSrcPV = nullptr;
    G4VPhysicalVolume *fCylSrcHolderPV = nullptr;
    G4VPhysicalVolume *fCylSrcHolderWallPV = nullptr;

    G4bool fCheckOverlaps;

    G4double fSrcBaseDistance = 180.*mm;
    G4double fSoilDensity = 1.223 * g/cm3;
};


#endif

