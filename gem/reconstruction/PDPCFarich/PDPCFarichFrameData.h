
#ifndef PDPCFarichFrameData_H
# define PDPCFarichFrameData_H 1

# include <vector>
# include "TObject.h"
# include "TClonesArray.h"

//Static TClonesArray-s are used to store raw hits and reconstructed events.
//Only one PDPCFarichFrameData object can be processed at a time. It's not thread safe.

class PDPCFarichHitData;
class PDPCFarichRecoEvent;

class PDPCFarichFrameData : public TObject
{

public:
    PDPCFarichFrameData();
    ~PDPCFarichFrameData() { Clear(); }

public:
    PDPCFarichHitData *AddRawHit();
    PDPCFarichRecoEvent *AddRecoEvent();

    void Clear(Option_t* opt="");

    void PrintRawHitData() const { recRawHits->Print(); }
    void PrintRecoEventData() const { recRecoEvents->Print(); }

public:
    PDPCFarichHitData *GetRawHit(UInt_t i) const
    {
        return (PDPCFarichHitData*)recRawHits->At(i);
    }
    PDPCFarichRecoEvent *GetRecoEvent(UInt_t i) const
    {
        return (PDPCFarichRecoEvent*)recRecoEvents->At(i);
    }

    Int_t GetRawHitCollectionSize() const
    {
        return recRawHits->GetEntries();
    }
    Int_t GetRecoEventCollectionSize() const
    {
        return recRecoEvents->GetEntries();
    }

    static TClonesArray* GetRawHitCollection();
    static TClonesArray* GetRecoEventCollection();

private:
    static TClonesArray* recRawHits;
    static TClonesArray* recRecoEvents;

};
#endif
