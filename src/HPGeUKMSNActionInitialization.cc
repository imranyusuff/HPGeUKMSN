#include "HPGeUKMSNActionInitialization.hh"
#include "HPGeUKMSNRunAction.hh"
#include "HPGeUKMSNEventAction.hh"
#include "HPGeUKMSNPrimaryGeneratorAction.hh"


HPGeUKMSNActionInitialization::HPGeUKMSNActionInitialization(G4String outfnPrefix, G4int geometrySelection, G4bool alwaysSingleSource)
 : G4VUserActionInitialization(),
   fOutfnPrefix(outfnPrefix),
   fGeometrySelection(geometrySelection),
   fAlwaysSingleSource(alwaysSingleSource)
{}


HPGeUKMSNActionInitialization::~HPGeUKMSNActionInitialization()
{}


void HPGeUKMSNActionInitialization::Build() const
{
  HPGeUKMSNRunAction *runAction = new HPGeUKMSNRunAction(fOutfnPrefix);
  SetUserAction(runAction);

  SetUserAction(new HPGeUKMSNEventAction(runAction));
  SetUserAction(new HPGeUKMSNPrimaryGeneratorAction(fGeometrySelection, fAlwaysSingleSource));
}

