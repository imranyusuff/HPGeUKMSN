#include "HPGeUKMSNActionInitialization.hh"
#include "HPGeUKMSNRunAction.hh"
#include "HPGeUKMSNEventAction.hh"
#include "HPGeUKMSNPrimaryGeneratorAction.hh"


HPGeUKMSNActionInitialization::HPGeUKMSNActionInitialization()
 : G4VUserActionInitialization()
{}


HPGeUKMSNActionInitialization::~HPGeUKMSNActionInitialization()
{}


void HPGeUKMSNActionInitialization::Build() const
{
  HPGeUKMSNRunAction *runAction = new HPGeUKMSNRunAction();
  SetUserAction(runAction);

  SetUserAction(new HPGeUKMSNEventAction(runAction));
  SetUserAction(new HPGeUKMSNPrimaryGeneratorAction);
}

