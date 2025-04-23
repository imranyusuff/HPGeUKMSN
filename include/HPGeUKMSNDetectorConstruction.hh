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
    HPGeUKMSNDetectorConstruction(G4int geometrySelection);
    virtual ~HPGeUKMSNDetectorConstruction();

    virtual G4VPhysicalVolume *Construct();
    virtual void ConstructSDandField();

    void SetSourceBaseDistance(G4double distance);
    void SetSourceXYPos(G4double x, G4double y);
    G4double GetSourceBaseDistance() { return fSrcBaseDistance; }

  private:
    void DefineMaterials();
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
      const int variant
    );

    void DefineCommands();

    G4int fGeometrySelection;

    G4GenericMessenger* fMessenger;

    G4Material *fDetMaterial;
    G4Material *fDetHolderMaterial;
    G4Material *fEndcapMaterial;
    G4Material *fEndcapTopMaterial;
    G4Material *fStopMaterial;
    G4Material *fSrcMaterial;
    G4Material *fStandMaterial;
    G4Material *fContainerMaterial;

    G4VPhysicalVolume *fSrcPV;
    G4VPhysicalVolume *fCylSrcHolderPV;
    G4VPhysicalVolume *fCylSrcHolderWallPV;

    G4bool fCheckOverlaps;

    G4double fSrcBaseDistance = 180.*mm;
};


#endif

