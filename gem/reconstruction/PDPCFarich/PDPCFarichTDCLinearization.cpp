
#include "PDPCFarichTDCLinearization.h"
#include "PDPCFarichFrameData.h"
#include "TreeFormats/PDPCFarichHitData.h"
#include <iostream>
#include "TMath.h"
#include <vector>

void PDPCFarichTDCLinearization::ProcessFrame(PDPCFarichFrameData* frame)
{
    if( !initialized ) return;

    Int_t nHits = frame->GetRawHitCollectionSize();

    for(Int_t i = 0; i < nHits; i++) {
	PDPCFarichHitData *rawHit = frame->GetRawHit(i);
	//Possible linearization job
    }

}
