#include "HPGeUKMSNActionInitialization.hh"
#include "HPGeUKMSNRunAction.hh"
#include "HPGeUKMSNEventAction.hh"
#include "HPGeUKMSNPrimaryGeneratorAction.hh"


HPGeUKMSNActionInitialization::HPGeUKMSNActionInitialization(G4int geometrySelection)
 : G4VUserActionInitialization(),
   fGeometrySelection(geometrySelection)
{}


HPGeUKMSNActionInitialization::~HPGeUKMSNActionInitialization()
{}


void HPGeUKMSNActionInitialization::Build() const
{
  HPGeUKMSNRunAction *runAction = new HPGeUKMSNRunAction();
  SetUserAction(runAction);

  SetUserAction(new HPGeUKMSNEventAction(runAction));
  SetUserAction(new HPGeUKMSNPrimaryGeneratorAction(fGeometrySelection));
}

