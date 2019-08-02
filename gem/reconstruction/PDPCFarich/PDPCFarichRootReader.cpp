
#include "PDPCFarichRootReader.h"
#include "TreeFormats/PDPCFarichHitData.h"
#include "TreeFormats/PDPCFarichRecoEvent.h"
#include "PDPCFarichFrameData.h"
#include "PDPCFarichGeometry.h"
#include "TreeFormats/pdpcevent.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cassert>

using namespace std;

PDPCFarichRootReader::PDPCFarichRootReader(const char* fn) : PDPCFarichVDataReader(fn),
    treeName("daq"),
    file(NULL),
    tree(NULL),
    fGeometry(NULL),
    pdpcRawData(NULL),
    triggerCount(0)
{
    nevents_read = 0;
}

PDPCFarichRootReader::~PDPCFarichRootReader()
{
    if( file ) Close();
}

void PDPCFarichRootReader::Open()
{
    flag_eof = kFALSE;
    flag_error = kFALSE;
    if( !filename || !*filename ) {
	cerr << "PDPCFarichRootReader::Open(): Filename is not specified" << endl;
	flag_error = kTRUE;
	return;
    }

    cout << "\nOpening root data file " << filename << "..." << endl;
    file = TFile::Open(filename,"READ");
    if( !file ) {
	cerr << "PDPCFarichRootReader::Open(): Can not open file " << filename << endl;
	flag_error = kTRUE;
	return;
    }

    file->GetObject(treeName,tree);
    if( !tree ) {
        cerr << "PDPCFarichRootReader::Open(): File contains no TTree " << treeName << endl;
	flag_error = kTRUE;
        Close();
        return;
    }

    if( tree->SetBranchAddress("triggerCount",&triggerCount) != 0 ) {
        cerr << "PDPCFarichRootReader::Open(): No triggerCount branch or it has wrong type" << endl;
	flag_error = kTRUE;
        Close();
        return;
    }

    if( tree->SetBranchAddress("pdpc",&pdpcRawData) != 0 ) {
        cerr << "PDPCFarichRootReader::Open(): No 'pdpc' branch or it has wrong type" << endl;
	flag_error = kTRUE;
        Close();
        return;
    }

    nevents_read = 0;
//    tree->SetBranchStatus("*",0);
//    tree->SetBranchStatus("triggerCount",1);
//    tree->SetBranchStatus("pdpc",1);

    cout << "Found TTree " << treeName << " with 'pdpc' branch and " << tree->GetEntries() << " events." << endl;
}

void PDPCFarichRootReader::Close()
{
    if( !file ) return;
    if( nevents_read )
        cout << nevents_read << " events read from root file. Closing it... " << endl;
    file->Close();
    delete file;
    file = NULL;
}

ULong_t PDPCFarichRootReader::SkipFrames(ULong_t nframes)
{
    if( !file ) return 0;

    //Frame means event here
    if( nevents_read+nframes>(UInt_t)tree->GetEntries() )
        nframes = tree->GetEntries() - nevents_read;
    nevents_read += nframes;

    cout << "Skipped " << nframes << " events (frames)" << endl;

    return nframes;
}

void PDPCFarichRootReader::ProcessFrame(PDPCFarichFrameData* frameData)
{
    if( !fGeometry ) {
	cerr << "PDPCFarichRootReader::ProcessFrame(): Geometry is not defined" << endl;
	return;
    }
    if( !file ) {
	cerr << "PDPCFarichRootReader::ProcessFrame(): Root file is not opened" << endl;
	return;
    }
    assert(tree != NULL);

    if( nevents_read>=tree->GetEntries() ) {
        flag_eof = kTRUE;
        return;
    }

    tree->GetEntry(nevents_read++);

    assert(pdpcRawData != NULL);

    UInt_t nhits = pdpcRawData->GetEntries();
    //cout << "PDPCFarichRootReader: " << nhits << " raw hits" << endl;
    for(UInt_t i=0; i<nhits; i++) {
        PdpcHit* rootHit = (PdpcHit*)(*pdpcRawData)[i];
        assert(rootHit != NULL);
        UInt_t target = rootHit->target;
        Int_t dieSID = fGeometry->GetDieSerialID(target,rootHit->die);
        PDPCFarichHitData *hit = frameData->AddRawHit();
        hit->SetFirstHit(i==0);
        hit->SetTarget(target);
        hit->SetTileUID(fGeometry->GetTileUID(target));
        hit->SetBusboard(fGeometry->TranslateBusboardID(target));
        hit->SetModule(fGeometry->TranslateModuleID(target));
        hit->SetTile(fGeometry->TranslateTileID(target));
        hit->SetDie(rootHit->die);
        hit->SetDieSerialID(dieSID);
        hit->SetSync(rootHit->sync);
        hit->SetFrameSequence(rootHit->total_frame_time);
        hit->SetFrame(rootHit->frame_number);
        hit->SetTimestamp(rootHit->ts);
        hit->SetCellCount(0,rootHit->ampl[0]);
        hit->SetCellCount(1,rootHit->ampl[1]);
        hit->SetCellCount(2,rootHit->ampl[2]);
        hit->SetCellCount(3,rootHit->ampl[3]);
        hit->SetXYindex(fGeometry->GetDieIndexX(dieSID),fGeometry->GetDieIndexY(dieSID));
    }
}
