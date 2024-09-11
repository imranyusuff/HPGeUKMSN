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
    HPGeUKMSNDetectorConstruction();
    virtual ~HPGeUKMSNDetectorConstruction();

    virtual G4VPhysicalVolume *Construct();
    virtual void ConstructSDandField();

    void SetSourceBaseDistance(G4double distance);
    G4double GetSourceBaseDistance() { return fSrcBaseDistance; }

  private:
    void DefineMaterials();
    G4VPhysicalVolume *DefineVolumes();
    void DefineCommands();

    G4GenericMessenger* fMessenger;

    G4Material *fDetMaterial;
    G4Material *fDetHolderMaterial;
    G4Material *fEndcapMaterial;
    G4Material *fEndcapTopMaterial;
    G4Material *fStopMaterial;
    G4Material *fSrcMaterial;
    G4Material *fStandMaterial;

    G4VPhysicalVolume *fSrcPV;
    G4VPhysicalVolume *fCylSrcHolderPV;

    G4bool fCheckOverlaps;

    G4double fSrcBaseDistance = 180.*mm;
};


#endif

