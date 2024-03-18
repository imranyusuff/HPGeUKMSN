#ifndef HPGeUKMSNDetectorConstruction_h
#define HPGeUKMSNDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"


class G4VPhysicalVolume;
class G4Material;


class HPGeUKMSNDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    HPGeUKMSNDetectorConstruction();
    virtual ~HPGeUKMSNDetectorConstruction();

    virtual G4VPhysicalVolume *Construct();
    virtual void ConstructSDandField();

  private:
    void DefineMaterials();
    G4VPhysicalVolume *DefineVolumes();

    G4Material *fDetMaterial;
    G4Material *fStopMaterial;
    G4Material *fSrcMaterial;
    G4Material *fStandMaterial;

    G4bool fCheckOverlaps;

    G4double fSrcBaseHeight = 180.*mm;
};


#endif

