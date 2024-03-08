#include "HPGeUKMSNDetectorHit.hh"
#include "G4UnitsTable.hh"

//#include <iomanip>


G4ThreadLocal G4Allocator<HPGeUKMSNDetectorHit> *HPGeUKMSNDetectorHitAllocator = 0;


HPGeUKMSNDetectorHit::HPGeUKMSNDetectorHit()
 : G4VHit(),
   fEdep(0.)
{}


HPGeUKMSNDetectorHit::~HPGeUKMSNDetectorHit()
{}


HPGeUKMSNDetectorHit::HPGeUKMSNDetectorHit(const HPGeUKMSNDetectorHit & right)
 : G4VHit()
{
  fEdep = right.fEdep;
}


const HPGeUKMSNDetectorHit & HPGeUKMSNDetectorHit::operator=(const HPGeUKMSNDetectorHit & right)
{
  fEdep = right.fEdep;

  return *this;
}


G4bool HPGeUKMSNDetectorHit::operator==(const HPGeUKMSNDetectorHit & right) const
{
  return (this == &right) ? true : false;
}


void HPGeUKMSNDetectorHit::Draw()
{}


void HPGeUKMSNDetectorHit::Print()
{
  G4cout << "Hit:  Edep = "
//         << std::setw(8)
         << G4BestUnit(fEdep, "Energy")
         << G4endl;
}

