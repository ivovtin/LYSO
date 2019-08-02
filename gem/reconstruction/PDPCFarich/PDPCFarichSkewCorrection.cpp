#include "PDPCFarichSkewCorrection.h"
#include "PDPCFarichFrameData.h"
#include "TreeFormats/PDPCFarichHitData.h"
#include "PDPCFarichGeometry.h"
#include <iostream>
#include <iomanip>
#include <fstream>

PDPCFarichSkewCorrection::PDPCFarichSkewCorrection(PDPCFarichGeometry* f, const char *fn) :
    PDPCFarichVFrameProcessing(), fGeometry(f), filename(fn)
{
}

PDPCFarichSkewCorrection::~PDPCFarichSkewCorrection()
{
    skewData.clear();
}

void PDPCFarichSkewCorrection::Initialize()
{
    std::cout << "Reading skew correction table from " << filename << "..." << std::endl;

    std::ifstream file(filename);
    if( file.fail() ) {
	std::cerr << "PDPCFarichSkewCorrection::Initialize(): Can not open file " << filename << std::endl;
	return;
    }

    skewData.clear();
    skewData.resize(fGeometry->GetNdies(),0);

    Bool_t error = kFALSE;
    Int_t line = 0, target, die, timebias;
    while( 1 ) {
        file >> std::hex >> target >> std::dec >> die >> timebias;
        if( file.eof() ) break;
        line++;
	if( !file ) {
            std::cerr << "PDPCFarichSkewCorrection::Initialize(): Failed to read line " << line << std::endl;
            error = kTRUE;
	    break;
	}
        Int_t dieSID = fGeometry->GetDieSerialID(target,die);
        if( dieSID<0 ) {
            std::cerr << "PDPCFarichSkewCorrection::Initialize(): Skew calibration conflicts with geometry data" << std::endl;
            error = kTRUE;
            break;
        }
        skewData[dieSID] = timebias;
    }

    file.close();

    if( !error )
        initialized = kTRUE;
}

void PDPCFarichSkewCorrection::ProcessFrame(PDPCFarichFrameData* frame)
{
    if( !initialized ) return;

    Int_t nHits = frame->GetRawHitCollectionSize();

    for(Int_t i = 0; i < nHits; i++) {
	PDPCFarichHitData* rawHit = frame->GetRawHit(i);
	Int_t time = rawHit->GetTimestamp();
        Int_t target = rawHit->GetTarget();
        Int_t die = rawHit->GetDie();
        Int_t dieSID = fGeometry->GetDieSerialID(target, die);
        Int_t newTime = time + skewData[dieSID];
        rawHit->SetTimestamp(newTime);
    }
}
