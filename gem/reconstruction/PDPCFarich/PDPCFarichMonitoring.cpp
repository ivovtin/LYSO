#include <iostream>
#include <ctime>
#include <limits>
#include <cassert>
#include <map>

#include "PDPCFarichMonitoring.h"
#include "PDPCFarichGeometry.h"
#include "PDPCFarichFrameData.h"
#include "TreeFormats/PDPCFarichHitData.h"
#include "TreeFormats/PDPCFarichRecoEvent.h"

#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TApplication.h"
#include "TFile.h"
#include "TH2.h"
#include "TF1.h"
#include "TTree.h"
#include "TGraph.h"
#include "TEllipse.h"
#include "TCanvas.h"
#include "TMarker.h"
#include "TString.h"
#include "TClonesArray.h"

using namespace std;

static struct timespec start={0,0}, current;
static void reset_timer() {
    clock_gettime(CLOCK_REALTIME, &start);
}
static bool is_timer_elapsed(double period) {
    if( period<=0. || start.tv_sec==0 && start.tv_nsec==0 ) return true;
    clock_gettime(CLOCK_REALTIME, &current);
    return (current.tv_sec-start.tv_sec+(double)(current.tv_nsec-start.tv_nsec)/1e9) >= period;
}

const Double_t PDPCFarichMonitoring::tdcBin_ns = 0.0195; //ns

PDPCFarichMonitoring::PDPCFarichMonitoring(PDPCFarichGeometry* f) :
    PDPCFarichVFrameProcessing()
{
    fGeometry = f;
    filename = "farich.root";
    file             = NULL;
    storeEmptyEvents = kTRUE;
    storeRawHits     = kFALSE;
    enableDrawing    = kFALSE;
    drawingInterval  = 2.;
    enablePrintingEventDisplay = kFALSE;
    edPrintDirectory = ".";
    timingLimit      = 50000;

    event            = NULL;
    rawHits          = NULL;
    outTree          = NULL;
    rawHitsBranch    = NULL;
    eventBranch      = NULL;
    hPixelOccupancy  = NULL;
    hDieOccupancy    = NULL;
    hDieAmplitude1   = NULL;
    hDieAmplitude2   = NULL;
    hPixelAmplitude1 = NULL;
    hPixelAmplitude2 = NULL;
    hDiePixelCount   = NULL;
    hPixelCount      = NULL;
    hCellCount       = NULL;
    hAllTimes        = NULL;
    hRingCenter      = NULL;
    hRadius          = NULL;
    hTime            = NULL;
    hNlogLH          = NULL;
    gSelHits         = NULL;
    gUnselHits       = NULL;
    hEventTimes      = NULL;
    ring             = NULL;
    ringCenter       = NULL;
    canvas           = NULL;
    app              = NULL;
}

void PDPCFarichMonitoring::Initialize()
{
    if( !fGeometry ) {
	cerr << "PDPCFarichMonitoring::Initialize(): Geometry is not defined" << endl;
	return;
    }

    gStyle->SetPalette(1,0);
    gStyle->SetOptStat(111110);
    gStyle->SetHistLineWidth(2);

    file = new TFile(filename, "RECREATE");

    cout << "Store histograms and event tree to a root file " << filename << endl;

    BuildTree();

    BuildHistograms();

    if( enableDrawing ) {
	int argc = 1;
	char *argv[] = {"-l"};
	app = new TApplication("PDPCFarichMonitoring",&argc,argv);
        ring = new TEllipse(0,0,50);
        ringCenter = new TMarker(0,0,7);
	canvas = new TCanvas("c1","Event display",0,20,800,840);
        canvas->Divide(2,2);
        canvas->GetPad(1)->Range(-100,-100,100,100);
	canvas->cd(2);
	hEventTimes->Draw();
        canvas->cd(3)->SetLogz();
        hPixelOccupancy->Draw("col");
        canvas->cd(4);
        hTime->Draw();
        cout << "Draw event display and cumulative hitmap" << endl;
    }
    if( enablePrintingEventDisplay )
        cout << "Event display will be printed to directory " << edPrintDirectory << endl;

    nevents = 0;

    initialized = kTRUE;
}

void PDPCFarichMonitoring::Terminate()
{
    if( !initialized ) return;

    hPixelOccupancy->Sumw2();
    hDieOccupancy->Sumw2();
    hDieAmplitude2->Divide(hDieOccupancy);
    hPixelAmplitude2->Divide(hPixelOccupancy);
    hPixelOccupancy->Scale(1./nevents);
    hDieOccupancy->Scale(1./nevents);

    outTree->SetEntries(nevents);

    if( enableDrawing ) {
	delete gSelHits;
	delete gUnselHits;
	delete ring;
	delete hEventTimes;
	delete canvas;
	delete app;
    }

    if( file ) {
        file->Write();
	file->Close();
	delete file;
	file = NULL;
    }

    if( event ) {
        delete event;
        event = NULL;
    }
    rawHits = NULL;
    //rawHits is taken from PDPCFarichFrameData. It must not be deleted here.

    initialized = kFALSE;
}

void PDPCFarichMonitoring::BuildTree()
{
    cout << "Creating farich tree" << endl;

    outTree = new TTree("farich", "FARICH event tree");
    outTree->SetAutoSave(50000000);
    TTree::SetBranchStyle(1);

    event = new PDPCFarichRecoEvent;
    eventBranch = outTree->Branch("event", &event, 16000, 2);
    eventBranch->SetAutoDelete(kFALSE);

    outTree->SetAlias("hittimeps","(hits.time-event.time)*19.53");
    outTree->SetAlias("hitrad","sqrt((hits.x-event.x0)**2+(hits.y-event.y0)**2)");

    if( storeRawHits ) {
        rawHits = PDPCFarichFrameData::GetRawHitCollection();
        rawHitsBranch = outTree->Branch("rawhits", &rawHits, 32000, 2);
    }
}

void PDPCFarichMonitoring::BuildHistograms()
{
    hPixelOccupancy = new TH2F("pixelOccupancy","Pixel occupancy;Nx;Ny;Pixel occupancy",PDPCFarichGeometry::nxTotalPixels, 0, 1.*PDPCFarichGeometry::nxTotalPixels, PDPCFarichGeometry::nxTotalPixels,0,1.*PDPCFarichGeometry::nxTotalPixels);
    hPixelOccupancy->SetStats(0);

    hDieOccupancy = new TH2F("dieOccupancy","Occupancy of dies;Nx;Ny;Die occupancy",PDPCFarichGeometry::nxTotalDies,0,1.*PDPCFarichGeometry::nxTotalDies,PDPCFarichGeometry::nxTotalDies,0,1.*PDPCFarichGeometry::nxTotalDies);
    hDieOccupancy->SetStats(0);

    hDieAmplitude1 = new TH1F("dieAmplitude1","Die distribution on amplitude;Amplitude;Number of dies", 100,-0.5,99.5);

    hDieAmplitude2 = new TH2F("dieAmplitude2","Die amplitude map;Nx;Ny;Die amplitude",PDPCFarichGeometry::nxTotalDies,0,1.*PDPCFarichGeometry::nxTotalDies,PDPCFarichGeometry::nxTotalDies,0,1.*PDPCFarichGeometry::nxTotalDies);

    hPixelAmplitude1 = new TH1F("pixelAmplitude1","Pixel distribution on amplitude;Amplitude;Number of pixels", 100,-0.5,99.5);

    hPixelAmplitude2 = new TH2F("pixelAmplitude2","Pixel amplitude map;Nx;Ny;Pixel amplitude", PDPCFarichGeometry::nxTotalPixels, 0, 1.*PDPCFarichGeometry::nxTotalPixels,PDPCFarichGeometry::nxTotalPixels,0,1.*PDPCFarichGeometry::nxTotalPixels);
    hPixelAmplitude2->SetStats(0);

    hDiePixelCount = new TH1F("diePixelCount","Die distribution on number of pixels hit;number of pixels", 5, -0.5, 4.5);

    hPixelCount = new TH1F("pixelCount","Event distribution on number of fired pixels;number of pixels", 200, -0.5, 199.5);

    hCellCount = new TH1F("cellCount","Event distribution on number of fired cells;number of cells", 1000, -0.5, 999.5);

    hAllTimes = new TH1F("allTimes","Times of all dies;timestamp", 1024, 0., 16777216.);

    hRingCenter = new TH2F("ringCenter","Ring center;X, mm;Y, mm", 120, -30., 30., 120, -30., 30.);

    hRadius = new TH1F("ringRadius","Ring radius;R, mm", 200, 0, 100);

    hTime = new TH1F("hitTime","Hit time w.r.t. mean time;hit_time-event_time, TDC ticks", 1000, -1.*timingLimit, 1.*timingLimit);

    hNlogLH = new TH1F("nLogLH", "Minimal negative log likelihood;-ln(LH)", 100, 0, 400);

    gSelHits = new TGraph;
    gSelHits->SetMarkerStyle(20);
    gSelHits->SetMarkerColor(kBlack);

    gUnselHits = new TGraph;
    gUnselHits->SetMarkerStyle(24);
    gUnselHits->SetMarkerColor(kGray+1);

    hEventTimes = new TH1F("hEventTimes","Event display of hit times;hit_time-event_time, TDC ticks",1000,-1.*timingLimit,1.*timingLimit);
    hEventTimes->SetDirectory(0); //do not store this histogram
}

void PDPCFarichMonitoring::ProcessFrame(PDPCFarichFrameData* frame)
{
    if( !initialized ) return;

    Int_t nRawHits = frame->GetRawHitCollectionSize();
    if( !nRawHits && !storeEmptyEvents )
        return;

    for(Int_t i=0; i<nRawHits; i++) {
	PDPCFarichHitData *rawHit = frame->GetRawHit(i);
	hAllTimes->Fill(rawHit->GetTimestamp());
    }
    if( storeRawHits ) {    
        rawHitsBranch->Fill();
    }

    Int_t nFrameEvents = frame->GetRecoEventCollectionSize();
    map<Int_t,pair<Int_t,Int_t>> die_data; //pixel and cell count per die
    for(Int_t ievent=0; ievent<nFrameEvents; ievent++) {
        frame->GetRecoEvent(ievent)->Copy(*event);
        UInt_t nHits = event->GetNhits();
        //assert(nHits>0);
        hEventTimes->Reset();
        //Int_t timeEnd=event->GetHit(nHits-1)->GetTime();
        Int_t ncells = 0, npixels = 0;
        for(Int_t ihit=0; ihit<nHits; ihit++) {
	    PDPCFarichRecoHit* hit = event->GetHit(ihit);
	    Double_t hitTime = (Double_t)hit->GetTime()-event->GetTime();
	    if( fabs(hitTime)>1.*timingLimit ) continue;
	    hTime->Fill(hitTime);
	    Int_t pixelAmp = hit->GetCellCount();
	    if( pixelAmp==0 ) continue;
	    Float_t xpixel = hit->GetXindex();
	    Float_t ypixel = hit->GetYindex();
	    hPixelOccupancy->Fill(xpixel,ypixel);
	    hPixelAmplitude1->Fill(pixelAmp);
	    hPixelAmplitude2->Fill(xpixel,ypixel,pixelAmp);
            npixels++;
            ncells += pixelAmp;
            Int_t dieSID = hit->GetPixelSerialID()/4;
            die_data[dieSID].first++;
            die_data[dieSID].second += pixelAmp;
	    hEventTimes->Fill(hitTime);
        }
        hPixelCount->Fill(npixels);
        hCellCount->Fill(ncells);

        map<Int_t,pair<Int_t,Int_t>>::const_iterator first=die_data.begin(), last=die_data.end();
        for( ; first!=last; first++) {
            Int_t dieSID = first->first;
            Int_t ndiepixels = first->second.first, ndiecells = first->second.second;
	    Float_t xdie = fGeometry->GetDieIndexX(dieSID);
	    Float_t ydie = fGeometry->GetDieIndexY(dieSID);
            hDiePixelCount->Fill(ndiepixels);
            hDieOccupancy->Fill(xdie,ydie);
            hDieAmplitude1->Fill(ndiecells);
            hDieAmplitude2->Fill(xdie,ydie,ndiecells);
        }
        die_data.clear();

        if( event->IsFitted() ) {
            hRingCenter->Fill(event->GetXcenter(),event->GetYcenter());
            hRadius->Fill(event->GetRadius());
            hNlogLH->Fill(event->GetNlogLH());
        }

        eventBranch->Fill();
        nevents++;

        static TEllipse* ring = new TEllipse;
        if( enableDrawing ) {
            if( is_timer_elapsed(drawingInterval) ) {
                UInt_t isel=0, iunsel=0;
                for(Int_t ihit=0; ihit<nHits; ihit++) {
                    PDPCFarichRecoHit* ahit = event->GetHit(ihit);
                    if( ahit->UsedForFit() )
                        gSelHits->SetPoint(isel++,ahit->GetXposition(),ahit->GetYposition());
                    else if( fabs(ahit->GetTime()-event->GetTime())<=timingLimit )
                        gUnselHits->SetPoint(iunsel++,ahit->GetXposition(),ahit->GetYposition());
                }
                gSelHits->Set(isel);
                gUnselHits->Set(iunsel);
                canvas->cd(1);
                gPad->GetListOfPrimitives()->Remove(gSelHits);
                gPad->GetListOfPrimitives()->Remove(gUnselHits);
                gPad->GetListOfPrimitives()->Remove(ring);
                if( gSelHits->GetN() )
                    gSelHits->Draw("p");
                if( gUnselHits->GetN() )
                    gUnselHits->Draw("p");
                ring->SetX1(event->GetXcenter());
                ring->SetY1(event->GetYcenter());
                ring->SetR1(event->GetRadius());
                ring->SetR2(event->GetRadius());
                ring->SetFillStyle(0);
                ring->SetLineWidth(2);
                if( event->IsFitted() )
                    ring->SetLineColor(kRed);
                else
                    ring->SetLineColor(kGray+2);
                ring->Draw();
                ringCenter->SetX(event->GetXcenter());
                ringCenter->SetY(event->GetYcenter());
                if( event->IsFitted() )
                    ringCenter->SetMarkerColor(kRed);
                else
                    ringCenter->SetMarkerColor(kGray+2);
                ringCenter->Draw();
                if( enablePrintingEventDisplay ) {
                    TString edfn = edPrintDirectory+"/event_display_";
                    edfn += event->GetNumber();
                    edfn += ".png";
                    gPad->Print(edfn);
                }
                canvas->GetPad(2)->Modified();
                canvas->GetPad(3)->Modified();
                canvas->GetPad(4)->Modified();
                canvas->Update();
                if( drawingInterval<0. )
                    gSystem->Sleep(-1000*drawingInterval);
                else if( drawingInterval==0. )
                    gSystem->Sleep(500.);
                else
                    reset_timer();
            }
            gSystem->ProcessEvents();
        }
    }
}
