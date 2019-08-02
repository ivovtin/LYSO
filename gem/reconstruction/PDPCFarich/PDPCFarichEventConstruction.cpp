#include "PDPCFarichEventConstruction.h"
#include "PDPCFarichFrameData.h"
#include "TreeFormats/PDPCFarichHitData.h"
#include "TreeFormats/PDPCFarichRecoEvent.h"
#include "PDPCFarichGeometry.h"
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "PDPCFarichLhFunc.h"
#include "TSortedList.h"
#include <iostream>
#include <fstream>
#include <climits>
#include <vector>
#include <set>
#include <algorithm>
#include <cassert>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


using namespace std;

void PDPCFarichEventConstruction::ReadConfigFile(const char *filename){
    ifstream in;
    in.open(filename);

    if( !in.is_open() ) {
        cerr << "PDPCFarichEventConstruction::ReadConfigFile(): Can not open file " << filename << endl;
        return;
    }

    while( !in.fail() ) {
        string line;
        Bool_t CA=0, TW=0, X1=0, X2=0, Y1=0, Y2=0, CT=0;

        Bool_t useCentralArea_clone;
        Int_t timeWindow_clone;
        Float_t x0min_clone, y0min_clone;
        Float_t x0max_clone, y0max_clone;
        Int_t centralTarget_clone;
        
        
        while (getline (in, line)){
            string token;
            istringstream is(line);
            is>>token;
            if (token == "useCentralArea" && CA == 0){
                is>>token;
                if (token == "1" || token == "yes" || token == "true") {
                    useCentralArea_clone=kTRUE;
                    CA = 1;
                } else if (token == "0" || token == "no" || token == "false") {
                    useCentralArea_clone=kFALSE;
                    CA = 1;
                }
            } else if (token == "centralTarget" && CT == 0){
                is>>token;
                centralTarget_clone = atoi(token.c_str());
                CT = 1;
            } else if (token == "timeWindow" && TW == 0){
                is>>token;            
                timeWindow_clone = atoi(token.c_str());
                TW = 1;
            } else if (token == "x0min" && X1 == 0){
                is>>token;
                x0min_clone = atof(token.c_str());
                X1 = 1;
            } else if (token == "x0max" && X2 == 0){
                is>>token;
                x0max_clone = atof(token.c_str());
                X2 = 1;
            } else if (token == "y0min" && Y1 == 0){
                is>>token;
                y0min_clone = atof(token.c_str());
                Y1 = 1;
            } else if (token == "y0max" && Y2 == 0){
                is>>token;
                y0max_clone = atof(token.c_str());
                Y2 = 1;
            }
        }

        if (CA==1 && TW==1 && X1==1 && X2==1 && Y1==1 && Y2==1 && CT==1) {
            SetUseCentralArea(useCentralArea_clone);
            SetTimeWindow(timeWindow_clone);
            SetCentralTarget(centralTarget_clone);
            SetCenterWindow(x0min_clone, x0max_clone, y0min_clone, y0max_clone);
            cout <<"PDPCFarichEventConstruction::Reading the configuration file successfully" <<endl;
        }

       	in.close();
    }
}




void PDPCFarichEventConstruction::Initialize()
{
    event_seq_number = 0;
    events_fitted = 0;
    events_triedtofit = 0;
    last_reinit_event = 0;
    nreinit = 0;

    lhFunc = new PDPCFarichLhFunc(fGeometry,calFilename);

    fMinimizer = 
	ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
//   fMinimizer = 
//	ROOT::Math::Factory::CreateMinimizer("Minuit2", "Simplex");
//   fMinimizer = 
//	ROOT::Math::Factory::CreateMinimizer("Minuit2", "Combined");
//   fMinimizer = 
//	ROOT::Math::Factory::CreateMinimizer("Minuit2", "Scan");
//   fMinimizer = 
//	ROOT::Math::Factory::CreateMinimizer("Minuit2", "Fumili");
//   fMinimizer = 
//	ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "ConjugateFR");
//   fMinimizer = 
//	ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "ConjugatePR");
//   fMinimizer = 
//	ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "BFGS");
//   fMinimizer = 
//	ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "BFGS2");
//   fMinimizer = 
//	ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "SteepestDescent");
//   fMinimizer = 
//	ROOT::Math::Factory::CreateMinimizer("GSLMultiFit", "");
//   fMinimizer = 
//	ROOT::Math::Factory::CreateMinimizer("GSLSimAn", "");

    fMinimizer->SetFunction(*lhFunc);
    fMinimizer->SetVariable(0,"X0",0.,10.);
    fMinimizer->SetVariable(1,"Y0",0.,10.);
    fMinimizer->SetLimitedVariable(2,"R",60.,5.,PDPCFarichLhFunc::GetMinimumRadius(),100.);
    fMinimizer->SetVariable(3,"T0",0.,20.);
    fMinimizer->SetPrintLevel(verbose?1:0);
    fMinimizer->SetStrategy(1);
    fMinimizer->SetMaxFunctionCalls(500);
    fMinimizer->SetErrorDef(0.5);

    if( coincidenceWindow>0 )
        cout << "Use coincidence window of " << coincidenceWindow << " TDC ticks to separate events" << endl;
    else
        cout << "Separate events by frame number" << endl;

    if( beamAerogel )
        cout << "Central tile of aerogel is installed" << endl;

    if( useCentralArea ) {
        cout << "Use central area limited by x0=" << x0min << ".." << x0max << ", y0=" << y0min << ".." << y0max << endl;
        centralTarget = 0;
    } else if( centralTarget>0 ) 
        cout << "Central area is given by target 0x" << ios::hex << centralTarget << endl;
    else 
        cout << "Do use central area" << endl;

    initialized = kTRUE;
}

void PDPCFarichEventConstruction::Reinitialize()
{
    if( !fMinimizer ) {
        Initialize();
        return;
    }

    cerr << "Try " << nreinit+1 << " to reinitialize minimizer after " << (Long64_t)events_triedtofit-last_reinit_event-1
	 << " events\n" << endl;

    fMinimizer->Clear();
    fMinimizer->SetFunction(*lhFunc);
    fMinimizer->SetVariable(0,"X0",0.,10.);
    fMinimizer->SetVariable(1,"Y0",0.,10.);
    fMinimizer->SetLimitedVariable(2,"R",60.,5.,PDPCFarichLhFunc::GetMinimumRadius(),100.);
    fMinimizer->SetVariable(3,"T0",0.,20.);
    fMinimizer->SetPrintLevel(verbose?1:0);
    fMinimizer->SetStrategy(1);
    fMinimizer->SetMaxFunctionCalls(500);
    fMinimizer->SetErrorDef(0.5);

    last_reinit_event = events_triedtofit-1;
    nreinit++;
}

void PDPCFarichEventConstruction::Terminate()
{
    if( doFit && events_triedtofit ) {
        streamsize p = cout.precision(4);
	cout << events_fitted << " fitted of " << events_triedtofit << " try-to-fit events ("
	    << 100.*events_fitted/events_triedtofit << "%)" << endl;
	cout.precision(p);
    }
    PDPCFarichRecoEvent::Reset();
    delete fMinimizer;
    delete lhFunc;
    initialized = kFALSE;
}

void PDPCFarichEventConstruction::ProcessFrame(PDPCFarichFrameData* frame)
{
    if( !initialized ) return;

    Int_t nRawHits = frame->GetRawHitCollectionSize();

    //cout << "PDPCFarichEventConstruction: " << nRawHits << " raw hits" << endl;

    PDPCFarichRecoEvent *event = 0;
    if( nRawHits==0 ) { //if no hits read event should be created anyway to store in the tree
        event = frame->AddRecoEvent();
        event->SetNumber(event_seq_number++);
        return;
    }

    //Sort hits by timestamp
    TSortedList sortedRawHits;
    for(Int_t i=0; i<nRawHits; i++)
        sortedRawHits.Add(frame->GetRawHit(i));

    PDPCFarichHitData *rawHit = 0;
    for(Int_t i=0; i<nRawHits; i++) {
        if( coincidenceWindow>0 )
            rawHit = (PDPCFarichHitData*)sortedRawHits.At(i);
        else
            rawHit = frame->GetRawHit(i);

        if( rawHit->GetCellCount()==0 ) continue; //skip raw hits with zero cell count in all pixels

        Int_t target = rawHit->GetTarget();
        Int_t die = rawHit->GetDie();
        Int_t time = rawHit->GetTimestamp();

        bool createEvent = false;
        if( !event )
            createEvent=true;
        else if( coincidenceWindow>0 && event->GetNhits()>0 &&
                 time>event->GetHit(0)->GetTime()+coincidenceWindow ) {
            if( verbose )
                cout << "First time window hit for frame " << rawHit->GetFrameSequence() << endl;
            createEvent=true;
        } else if( coincidenceWindow==0 && rawHit->IsFirstHit() ) {
            if( verbose )
                cout << "First coincidence hit for frame " << rawHit->GetFrameSequence() << endl;
            createEvent=true;
        }

        if( createEvent ) {
            event = frame->AddRecoEvent();
            event->SetFrame(rawHit->GetFrameSequence());
            event->SetNumber(event_seq_number++);
            event->SetSync(rawHit->GetSync());
        }

	    for(UInt_t ipx=0; ipx<PDPCFarichGeometry::nPixelsPerDie; ipx++) {
	        if( rawHit->GetCellCount(ipx)==0 ) continue;
	        PDPCFarichRecoHit* recoHit = event->AddHit();
            UInt_t pixelSID = fGeometry->GetPixelSerialID(target,die,ipx);
	        recoHit->SetTarget(target);
	        recoHit->SetDie(die);
	        recoHit->SetPixel(ipx);
	        recoHit->SetPixelSerialID(pixelSID);
            recoHit->SetXYindex(fGeometry->GetPixelIndexX(pixelSID),fGeometry->GetPixelIndexY(pixelSID));
            Float_t x = fGeometry->GetPixelPositionX(pixelSID), y = fGeometry->GetPixelPositionY(pixelSID);
            recoHit->SetPosition(x,y);
	        recoHit->SetCellCount(rawHit->GetCellCount(ipx));
            recoHit->SetTime(time);
            if( useCentralArea ) {
                if( x>=x0min && x<x0max && y>=y0min && y<y0max )
                    recoHit->SetCentral();
            } else if( target==centralTarget )
                recoHit->SetCentral();
        }
    }

    Int_t nEvents = frame->GetRecoEventCollectionSize();
    for(Int_t ievent=0; ievent<nEvents; ievent++) {
        PDPCFarichRecoEvent* event = frame->GetRecoEvent(ievent);
        Double_t eventTime = 0.;
        Int_t maxCentralAmp = 0, centralTime = 0;
        UInt_t nHits = event->GetNhits();
        TSortedList sortedRecoHits;
        for(Int_t ihit=0; ihit<nHits; ihit++) {
            PDPCFarichRecoHit* hit = event->GetHit(ihit);
            Int_t time = hit->GetTime();
            sortedRecoHits.Add(hit);
            eventTime += 1.*time/nHits;

            Int_t ncells = hit->GetCellCount();
            if( beamAerogel && hit->IsCentral() && maxCentralAmp<ncells ) {
                maxCentralAmp = ncells;
                centralTime = time;
            }
        }
        event->SetTime(eventTime); //set mean time of all hits as the event time

        if( beamAerogel ) {
            Double_t particleTime = 0., particleTimeWeight = 0.;
            if( maxCentralAmp>0 ) {
                for(Int_t ihit=0; ihit<nHits; ihit++) {
                    PDPCFarichRecoHit* hit = event->GetHit(ihit);
                    Int_t ncells = hit->GetCellCount();
                    if( hit->IsCentral() && abs(hit->GetTime()-centralTime)<timeWindow ) {
                        particleTime += ncells*hit->GetTime();
                        particleTimeWeight += ncells;
                    }
                }
            }
            if( particleTimeWeight!=0. )
                particleTime/=particleTimeWeight;
            event->SetCentralTime(particleTime); //set mean time of hits in the central area
        }

        if( doFit ) {
            //Search a window with the most number of hits in it
            Int_t first=0, last=0, pos1=0, pos2=0;
            for( ; first<nHits && last<nHits; first++) {
                Int_t tf = ((PDPCFarichRecoHit*)sortedRecoHits.At(first))->GetTime();
                Int_t tl = ((PDPCFarichRecoHit*)sortedRecoHits.At(last))->GetTime();
                while( last<nHits-1 && tl<tf+timeWindow ) {
                    last++;
                    tl = ((PDPCFarichRecoHit*)sortedRecoHits.At(last))->GetTime();
                }
                if( pos2-pos1<last-first ) { pos1=first; pos2=last; }
            }

            UInt_t nSelected = 0;
            eventTime = 0;
            for(first=pos1; first!=pos2; first++) {
                PDPCFarichRecoHit* hit = (PDPCFarichRecoHit*)sortedRecoHits.At(first);
                if( hit->IsCentral() ) continue; //exclude central hits from fit
                eventTime += hit->GetTime();
                hit->UseForFit();
                nSelected++;
            }
            eventTime /= nSelected;

            event->SetTime(eventTime); //set more exact event time to the mean time of hits in the found narrow window
            event->SetSelectedHits(nSelected);

            if( nSelected>=nHitsFitThreshold ) { //do fitting if number of selected hits is not less than defined one
                events_triedtofit++;
                event->SetNpe(15.);
                event->SetSigmaRadius(10.);
                event->SetSigmaTime(30.);
                CoarseSearch(event,radius_est,x0min,x0max,y0min,y0max,step);
                if( !Fit(event) ) {
                    Reinitialize();
                    Fit(event);
                }
                if( event->IsFitted() ) {
                    event->SetSigmaRadius(2.);
                    if( skewCorrected )
                        event->SetSigmaTime(2.5);
                    if( Fit(event) )
                        events_fitted++;
                }
            }
        }

        if( verbose )
            event->Print("hits");
    }
}

//Coarse search for ring center
void PDPCFarichEventConstruction::CoarseSearch(PDPCFarichRecoEvent* event,
                                               Float_t r,Float_t x0min,Float_t x0max,Float_t y0min,Float_t y0max,Float_t step)
{
    lhFunc->SetRecoEvent(event);

    Float_t x0 = 0., y0 = 0.;
    Double_t par[4] = {x0, y0, r, event->GetTime()};
    Float_t minimum = (*lhFunc)(par);

    for(par[0] = x0min; par[0] <= x0max; par[0]+=step) {
        for(par[1] = y0min; par[1] <= y0max; par[1]+=step) {
            Double_t val = (*lhFunc)(par);
            if( minimum > val ) {
                minimum = val;
                x0 = par[0];
                y0 = par[1];
            }
        }
    }
    event->SetRadius(r);
    event->SetCenterPosition(x0,y0);
    event->SetNlogLH(minimum);
}

//Fit ring
Bool_t PDPCFarichEventConstruction::Fit(PDPCFarichRecoEvent* event,Option_t *opt)
{
    lhFunc->SetRecoEvent(event);

    event->ResetFit();

    TString sopt(opt);
    sopt.ToLower();

    if( sopt.Contains("e") )
	event->Estimate();

    fMinimizer->SetVariableValue(0,event->GetXcenter());
    fMinimizer->SetVariableValue(1,event->GetYcenter());
    fMinimizer->SetVariableValue(2,event->GetRadius());
    fMinimizer->SetVariableValue(3,event->GetTime());

    Bool_t fitted = fMinimizer->Minimize();

    if( fitted ) {
	const double *par = fMinimizer->X();
	event->SetCenterPosition(par[0],par[1]);
	event->SetRadius(par[2]);
        event->SetTime(par[3]);
        Float_t nLogLH = fMinimizer->MinValue();
        event->SetNlogLH(nLogLH);
        Float_t savenpe = event->GetNpe();
        event->SetNpe(0.);
        event->SetConsistency((*lhFunc)(par)-nLogLH);
        event->SetNpe(savenpe);
        event->SetFitted(fitted);
    }

    return fitted;
}
