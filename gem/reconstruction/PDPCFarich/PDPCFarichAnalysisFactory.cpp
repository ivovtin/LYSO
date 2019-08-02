#include "PDPCFarichAnalysisFactory.h"
#include "TreeFormats/PDPCFarichHitData.h"
#include "TreeFormats/PDPCFarichRecoEvent.h"
#include "PDPCFarichFrameData.h"
#include "PDPCFarichVDataReader.h"
#include "PDPCFarichVFrameProcessing.h"
#include "PDPCFarichGeometry.h"
#include <limits.h>
#include <iostream>
#include <omp.h>

using namespace std;

PDPCFarichAnalysisFactory::PDPCFarichAnalysisFactory()
{
    initialized = kFALSE;
    fGeometry = NULL;
    fReader = NULL;
    fTDCLinearization = NULL;
    fSkewCorrection = NULL;
    fEventConstruction = NULL;
}

PDPCFarichAnalysisFactory::~PDPCFarichAnalysisFactory()
{
}

void PDPCFarichAnalysisFactory::Initialize()
{
    cout << "Initializing analysis..." << endl;
    if( !fGeometry ) {
	cerr << "PDPCFarichAnalysisFactory::Initialize(): Geometry is undefined" << endl;
	return;
    }
    if( !fGeometry->IsInitialized() ) {
	cerr << "PDPCFarichAnalysisFactory::Initialize(): Geometry is uninitialized" << endl;
	return;
    }
    if( !fReader ) {
	cerr << "PDPCFarichAnalysisFactory::Initialize(): Data reader is not specified" << endl;
	return;
    }
    if( fTDCLinearization )
	fTDCLinearization->Initialize();
    if( fSkewCorrection )
	fSkewCorrection->Initialize();
    if( fEventConstruction )
	fEventConstruction->Initialize();

    nframes_reconstructed = 0;
    nhits_reconstructed = 0;
    initialized = kTRUE;
}

void PDPCFarichAnalysisFactory::Terminate()
{

    cout << "Analysis finished\n "<< nhits_reconstructed << " hits processed\n "
         << nframes_reconstructed << " frames processed" << endl;
    if( fTDCLinearization )
	fTDCLinearization->Terminate();
    if( fSkewCorrection )
	fSkewCorrection->Terminate();
    if( fEventConstruction )
	fEventConstruction->Terminate();
    initialized = kFALSE;
}

ULong64_t PDPCFarichAnalysisFactory::Process(ULong64_t start, ULong64_t nframes)
{
    if( !initialized ) {
	cerr << "PDPCFarichAnalysisFactory::Process(): Analysis is not initialized" << endl;
	return 0;
    }

    fReader->Open();

    if( fReader->IsError() )
	return 0;

    if( fMonitor )
        fMonitor->Initialize();

    if( start ) {
	ULong64_t nskipped = fReader->SkipFrames(start-1);
	if( nskipped<start-1 ) return 0;
    }

    if( nframes==0 )
	nframes = ULLONG_MAX;

    ULong64_t nframes_file=0, nhits_file = 0;

    UInt_t frame_id=0, frame_seq=0, prev_frame_id, prev_frame_seq;

    for(ULong64_t i = 0; i < nframes; i++) {
	PDPCFarichFrameData frame;

        fReader->ProcessFrame(&frame);

	if( frame.GetRawHitCollectionSize()==0 && fReader->IsComplete() ) break;

        if( fTDCLinearization )
            fTDCLinearization->ProcessFrame(&frame);
        if( fSkewCorrection )
            fSkewCorrection->ProcessFrame(&frame);
        if( fEventConstruction )
            fEventConstruction->ProcessFrame(&frame);
	if( fMonitor )
            fMonitor->ProcessFrame(&frame);

        nframes_file++;
	nhits_file += frame.GetRawHitCollectionSize();

	if( (nframes_file%100)==0 )
	    cout << nframes_file << " frames analysed             \r" << flush;

	if( fReader->IsComplete() ) break;
    }

    fReader->Close();

    if( fMonitor )
	fMonitor->Terminate();

    nhits_reconstructed += nhits_file;
    nframes_reconstructed += nframes_file;

    cout << nhits_file << " hits, " << nframes_file
         << " frames processed for " << fReader->GetFilename() << endl;

    return nframes_file;
}
