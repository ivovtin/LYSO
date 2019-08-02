
#include "PDPCFarichFrameData.h"
#include "TreeFormats/PDPCFarichHitData.h"
#include "TreeFormats/PDPCFarichRecoEvent.h"
#include <iostream>

using std::cout;
using std::endl;

TClonesArray* PDPCFarichFrameData::recRawHits=0;
TClonesArray* PDPCFarichFrameData::recRecoEvents=0;

TClonesArray* PDPCFarichFrameData::GetRawHitCollection() {
    if( recRawHits==NULL ) recRawHits = new TClonesArray("PDPCFarichHitData",200);
    return recRawHits;
}
TClonesArray* PDPCFarichFrameData::GetRecoEventCollection() {
    if( recRecoEvents==NULL ) recRecoEvents = new TClonesArray("PDPCFarichRecoEvent",100);
    return recRecoEvents;
}

PDPCFarichFrameData::PDPCFarichFrameData()
{
    GetRawHitCollection();
    GetRecoEventCollection();
    recRawHits->Delete();
    recRecoEvents->Delete();
}

void PDPCFarichFrameData::Clear(Option_t* opt)
{
    recRawHits->Delete();
    recRecoEvents->Delete(); //ensure that PDPCFarichRecoEvent d-tor is called
}

PDPCFarichHitData *PDPCFarichFrameData::AddRawHit()
{
    if( recRawHits->GetEntries()==recRawHits->GetSize() )
        recRawHits->Expand(2*recRawHits->GetSize());
    return (PDPCFarichHitData*)recRawHits->New(recRawHits->GetEntries());
}

PDPCFarichRecoEvent *PDPCFarichFrameData::AddRecoEvent()
{
    if( recRecoEvents->GetEntries()==recRecoEvents->GetSize() )
        recRecoEvents->Expand(2*recRecoEvents->GetSize());
    return (PDPCFarichRecoEvent*)recRecoEvents->New(recRecoEvents->GetEntries());
}

