#ifndef HPGeUKMSNDetectorHit_h
#define HPGeUKMSNDetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"


class HPGeUKMSNDetectorHit : public G4VHit
{
  public:
    HPGeUKMSNDetectorHit();
    HPGeUKMSNDetectorHit(const HPGeUKMSNDetectorHit &);
    virtual ~HPGeUKMSNDetectorHit();

    const HPGeUKMSNDetectorHit & operator=(const HPGeUKMSNDetectorHit &);
    G4bool operator==(const HPGeUKMSNDetectorHit &) const;

    inline void *operator new(size_t);
    inline void  operator delete(void *);

    virtual void Draw();
    virtual void Print();

    void SetEdep(G4double de)  { fEdep = de; }

    G4double GetEdep() const   { return fEdep; }

  private:
    G4double fEdep;
};


typedef G4THitsCollection<HPGeUKMSNDetectorHit> HPGeUKMSNDetectorHitsCollection;

extern G4ThreadLocal G4Allocator<HPGeUKMSNDetectorHit> *HPGeUKMSNDetectorHitAllocator;


inline void *HPGeUKMSNDetectorHit::operator new(size_t)
{
  if (!HPGeUKMSNDetectorHitAllocator)
    HPGeUKMSNDetectorHitAllocator = new G4Allocator<HPGeUKMSNDetectorHit>;
  return (void *) HPGeUKMSNDetectorHitAllocator->MallocSingle();
}


inline void HPGeUKMSNDetectorHit::operator delete(void *hit)
{
  HPGeUKMSNDetectorHitAllocator->FreeSingle((HPGeUKMSNDetectorHit *) hit);
}


#endif

