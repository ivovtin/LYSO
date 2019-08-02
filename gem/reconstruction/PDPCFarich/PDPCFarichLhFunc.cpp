#include "PDPCFarichLhFunc.h"
#include "TreeFormats/PDPCFarichRecoEvent.h"
#include "PDPCFarichGeometry.h"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

const unsigned int PDPCFarichLhFunc::nPars = 4;
const float PDPCFarichLhFunc::excludeRadius = 10.; //mm
const float PDPCFarichLhFunc::defPixelDCprob = 5e-7; // (TDC_bin)^(-1)

PDPCFarichLhFunc::PDPCFarichLhFunc(PDPCFarichGeometry *f, const char* calfn) :
    ROOT::Math::IGradientFunctionMultiDim(),
    fGeometry(f),
    event(0),
    pixelEfficiency(fGeometry->GetNpixels(),1.0),
    pixelDCprob(fGeometry->GetNpixels(),defPixelDCprob)
{
    if( calfn && *calfn ) {
        ifstream in(calfn);
	if( in.fail() ) {
	    cerr << "Failed to open " << calfn << endl;
	} else {
	    cout << "Reading pixel efficiency and dark count probability from " << calfn << endl;
	    Float_t eff, dcProb;
	    Int_t pixelSID;
	    UInt_t line=0;
	    while( in.good() ) {
		in>>pixelSID>>eff>>dcProb;

		if( in.fail() ) break;

                if( pixelSID<0 || pixelSID>=fGeometry->GetNpixels() ) {
                    cerr << "Line " << line << ": illegal pixel serial ID " << pixelSID << endl;
                    continue;
                }

                pixelEfficiency[pixelSID] = (eff>=0.?eff:0.);

                pixelDCprob[pixelSID] = (dcProb>=0.?dcProb:0.);

                line++;
	    }
	    in.close();
	    cout << line << " lines read" << endl;
	}
    }
}

void PDPCFarichLhFunc::FdF(const double *xx, double &f, double *grad) const
{
    static const float Spixel = PDPCFarichGeometry::pixelSizeX*PDPCFarichGeometry::pixelSizeY;

    f = 0.0;
    if( grad )
	for(int i=0; i<nPars; i++)
	    grad[i] = 0.0;

    if( !event ) return;

    const double Npe = event->GetNpe();
    const double SigR2 = event->GetSigmaRadius()*event->GetSigmaRadius();
    const double Sigt2 = event->GetSigmaTime()*event->GetSigmaTime();
    const double C = Npe*Spixel/(4*M_PI*M_PI*event->GetSigmaRadius()*event->GetSigmaTime());

    double x0 = xx[0];
    double y0 = xx[1];
    double R  = fabs(xx[2]);
    double t0 = xx[3];

    Int_t pixelSID;
    Int_t nHits = event->GetNhits();
    double xi, yi, ti, di, SN, F, LogN;
    f = 0;
    for(Int_t ihit = 0; ihit < nHits; ihit++) {
	PDPCFarichRecoHit *ahit = event->GetHit(ihit);

	if( !ahit->UsedForFit() ) continue;

	pixelSID = ahit->GetPixelSerialID();

        if( pixelDCprob[pixelSID]<=0. ) continue;

	xi = ahit->GetXposition();
	yi = ahit->GetYposition();
	ti = ahit->GetTime();
	di = sqrt((x0-xi)*(x0-xi)+(y0-yi)*(y0-yi));

//        if( di < excludeRadius ) continue;

        LogN = log(pixelDCprob[pixelSID]);
        SN = pixelEfficiency[pixelSID]*C/R*exp(-(di-R)*(di-R)/(2*SigR2)-(ti-t0)*(ti-t0)/(2*Sigt2)-LogN);

	if( SN < 1e-8 )
	    f-= SN + LogN;
        else
	    f-= log( SN + 1 ) + LogN;

	if( grad ) {
	    F = 1/(1+1./SN);
	    if( di>0.0 ) {
		grad[0] += F*(R-di)/SigR2*(xi-x0)/di;
		grad[1] += F*(R-di)/SigR2*(yi-y0)/di;
	    }
	    grad[2] += F*(1./R + (R-di)/SigR2);
	    grad[3] += F*(t0-ti)/Sigt2;
	}
    }
}
