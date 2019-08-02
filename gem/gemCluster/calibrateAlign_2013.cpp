#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <memory>

#include <unistd.h> //getopt, access

#include <boost/foreach.hpp>

#include <TROOT.h>
#include <TMath.h>
#include <TH2.h>
#include <TF1.h>
#include <TFile.h>
#include <TChain.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TEntryList.h>
#include <TClonesArray.h>
#include <TApplication.h>
#include <TString.h>
#include <TLatex.h>
#include <TSystem.h>
#include <TVector3.h>
#include <TProfile.h>
#include <TMinuit.h>

#include "TreeFormats/GemEvent.h"
#include "reconstruction/GEM/GemDetector.h"
#include "reconstruction/GEM/GemGeometry.h"
#include "TGraphErrors.h"
#include "TFormula.h"

using namespace std;

TGraphErrors zx(0);
TGraphErrors zy(0);
static double x_gem_err = 0.07;//mm
static double y_gem_err = 0.15;//mm

static int tail = 3;

static gem::Geometry geom;
static TChain gemChain("gem");
static TClonesArray *clusters2d = 0;
static TH2F* hxy[4] = {0,0,0,0};
static TH2F* dxdy[4] = {0,0,0,0};
static TProfile* ydx[4] = {0,0,0,0};
static TProfile* xdy[4] = {0,0,0,0};
static TProfile* xy[4] = {0,0,0,0};
static TProfile* yx[4] = {0,0,0,0};

static int base = 2; // base detector
static const char* icfgfn = "gem_geom_config.dat";
static const char* ocfgfn = "tmp.dat";
static Float_t squareSize = 100; //mm
static Float_t rectWidth = 25; //mm
static bool batch = false;

void RectangleCutAndFill(Int_t base, Float_t xMin, Float_t xMax, Float_t yMin, Float_t yMax){
    for(int i = 0; i < 4; ++i) {
        assert( hxy[i] );
        hxy[i]->Reset();
    }

    TVector3 position[4];
    Long64_t nListEntries = gemChain.GetEntryList()->GetN(), nselected = 0;
    for(Long64_t entry = 0; entry < nListEntries; ++entry) {
        Long64_t entryNumber = gemChain.GetEntryNumber(entry);
        gemChain.GetEntry(entryNumber);
        Bool_t selected = kFALSE;
        for(size_t i = 0; i < 4; ++i) {
            const GemCluster & cluster = *dynamic_cast<GemCluster*>(clusters2d->At(i));
            assert(cluster.dx>0 && cluster.dy>0);
            position[i].SetXYZ(cluster.x, cluster.y, 0.);
            geom.transformToLabFrame(cluster.detector, position[i]);
            if( cluster.detector==base && position[i].X()>=xMin && position[i].X()<xMax && position[i].Y()>=yMin && position[i].Y()<yMax ){
                selected = kTRUE;
			}
        }
        if( !selected ) continue;
        nselected++;
        for(size_t i = 0; i < 4; ++i) {
            hxy[i]->Fill(position[i].X(),position[i].Y());
        }
        gSystem->ProcessEvents();
    }
    cout << nselected << " entries selected with hits in rectangle" << endl;
}


void TrackConstructionAndFill(Int_t base, Float_t xMin, Float_t xMax, Float_t yMin, Float_t yMax, Int_t tail){
    for(int i = 0; i < 4; ++i) {
        assert(dxdy[i]); dxdy[i]->Reset();
		assert(ydx[i]); ydx[i]->Reset();
		assert(xdy[i]); xdy[i]->Reset();
		assert(xy[i]); xy[i]->Reset();
		assert(yx[i]); yx[i]->Reset();
	}

    TVector3 position[4];
	double_t x_track, y_track, dx, dy;
    Long64_t nListEntries = gemChain.GetEntryList()->GetN(), nselected = 0;
    for(Long64_t entry = 0; entry < nListEntries; ++entry) {
        Long64_t entryNumber = gemChain.GetEntryNumber(entry);
        gemChain.GetEntry(entryNumber);
        Bool_t selected = kFALSE;
        for(size_t i = 0; i < 4; ++i) {
            const GemCluster & cluster = *dynamic_cast<GemCluster*>(clusters2d->At(i));
            assert(cluster.dx>0 && cluster.dy>0);
            position[i].SetXYZ(cluster.x, cluster.y, 0.);
            geom.transformToLabFrame(cluster.detector, position[i]);
            if( cluster.detector==base && position[i].X()>=xMin && position[i].X()<xMax && position[i].Y()>=yMin && position[i].Y()<yMax ){
                selected = kTRUE;
			}
        }
        if( !selected ) continue;
        nselected++;

		for(int j = 0; j < 4; j++){
			//int tail=3;
			x_track = ((position[j].Z() - position[base].Z()) / (position[tail].Z() - position[base].Z())) * (position[tail].X() - position[base].X()) + position[base].X();
			y_track = ((position[j].Z() - position[base].Z()) / (position[tail].Z() - position[base].Z())) * (position[tail].Y() - position[base].Y()) + position[base].Y();
			dx = -position[j].X() + x_track;
			dy = -position[j].Y() + y_track;

			dxdy[j]->Fill(dx, dy);
			ydx[j]->Fill(position[j].Y(), dx);
			xdy[j]->Fill(position[j].X(), dy);
			xy[j]->Fill(position[j].X(), position[j].Y());
			yx[j]->Fill(position[j].Y(), position[j].X());
		}
        gSystem->ProcessEvents();
    }
}

/*void TrackConstructionAndFill(Int_t base, Float_t xMin, Float_t xMax, Float_t yMin, Float_t yMax){
    for(int i = 0; i < 4; ++i) {
        assert(dxdy[i]); dxdy[i]->Reset();
		assert(ydx[i]); ydx[i]->Reset();
		assert(xdy[i]); xdy[i]->Reset();
		assert(xy[i]); xy[i]->Reset();
		assert(yx[i]); yx[i]->Reset();
	}

    TVector3 position[4];
	double_t x_track, y_track, dx, dy;
	double z[4] = {135., 1650., 1680., 2663.};

	//TCanvas can_fit("can_fit","Two Graph",600,640);
	//can_fit.Divide(2,1);
	//can_fit.SetGrid();

    Long64_t nListEntries = gemChain.GetEntryList()->GetN(), nselected = 0;
    for(Long64_t entry = 0; entry < nListEntries; ++entry) {
        Long64_t entryNumber = gemChain.GetEntryNumber(entry);
        gemChain.GetEntry(entryNumber);
        Bool_t selected = kFALSE;
        for(size_t i = 0; i < 4; ++i) {
            const GemCluster & cluster = *dynamic_cast<GemCluster*>(clusters2d->At(i));
            assert(cluster.dx>0 && cluster.dy>0);
            position[i].SetXYZ(cluster.x, cluster.y, 0.);
            geom.transformToLabFrame(cluster.detector, position[i]);
            if( cluster.detector==base && position[i].X()>=xMin && position[i].X()<xMax && position[i].Y()>=yMin && position[i].Y()<yMax ){
                selected = kTRUE;
			}
        }
        if( !selected ) continue;
        nselected++;

		for(int i = 0; i < 4; i++){
			zx.SetPoint(i, z[i], position[i].X());
			zx.SetPointError(i, 0.0, x_gem_err);

			zy.SetPoint(i, z[i], position[i].Y());
			zy.SetPointError(i, 0.0, y_gem_err);
		}

		TF1 *x_fit = new TF1("x_fit","[0] + [1] * x", z[0], z[3]);
		zx.Fit("x_fit");
		//double Ax = x_fit->GetParameter(0);
		//double Bx = x_fit->GetParameter(1);

		TF1 *y_fit = new TF1("y_fit","[0] + [1] * x", z[0], z[3]);
		zy.Fit("y_fit");
		//double Ay = y_fit->GetParameter(0);
		//double By = y_fit->GetParameter(1);
/*
		can_fit.cd(1);
		zx.SetTitle("TGraphErrors X");
		zx.SetLineColor(4);
		zx.SetMarkerStyle(33);
		zx.Draw("alp");

		can_fit.cd(2);
		zy.SetTitle("TGraphErrors Y");
		zy.SetLineColor(4);
		zy.SetMarkerStyle(33);
		zy.Draw("alp");

		can_fit.Update();
		can_fit.SaveAs("fit.root");
*//*
		for(int i = 0; i < 4; i++){
			int tail=3;
			x_track = x_fit->Eval(i);//Ax + Bx*z[j];
			y_track = y_fit->Eval(i);//Ay + By*z[j];
			dx = -position[i].X() + x_track;
			dy = -position[i].Y() + y_track;

			dxdy[i]->Fill(dx, dy);
			ydx[i]->Fill(position[i].Y(), dx);
			xdy[i]->Fill(position[i].X(), dy);
			xy[i]->Fill(position[i].X(), position[i].Y());
			yx[i]->Fill(position[i].Y(), position[i].X());
		}

        gSystem->ProcessEvents();
    }
}*/


void calc_hi_det_1 (Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){
	int det = 1;
	int I = ydx[det]->GetNbinsX();
	int J = xdy[det]->GetNbinsX();
	float dx=0, dy=0, x=0, y=0, sigma_dx=0, sigma_dy=0, FCN_x=0, FCN_y=0;
	double x0 = par[0];
	double y0 = par[1];
	double phi= par[2];

	for (int i = 0; i < I; i++){
		if (ydx[det]->GetBinEntries(i) > 1){
			dx = ydx[det]->GetBinContent(i);
			y = ydx[det]->GetBinCenter(i);
			x = yx[det]->GetBinContent(i);
			sigma_dx = ydx[det]->GetBinError(i);
			FCN_x += pow((dx - x0 - x*(cos(phi) - 1) + y*sin(phi)), 2) / pow(sigma_dx, 2);
		}
	}
	for (int j = 0; j < J; j++){
		if (xdy[det]->GetBinEntries(j) > 1){
			dy = xdy[det]->GetBinContent(j);
			y = xy[det]->GetBinContent(j);
			x = xdy[det]->GetBinCenter(j);
			sigma_dy = xdy[det]->GetBinError(j); 
			FCN_y += pow((dy - y0 - y*(cos(phi) - 1) - x*sin(phi)), 2) / pow(sigma_dy, 2);
		}
	}
	f = FCN_x + FCN_y;
	cout <<"f = " <<f <<endl;
	return;
}
void calc_hi_det_0 (Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){
	int det = 0;
	int I = ydx[det]->GetNbinsX();
	int J = xdy[det]->GetNbinsX();
	float dx=0, dy=0, x=0, y=0, sigma_dx=0, sigma_dy=0, FCN_x=0, FCN_y=0;
	double x0 = par[0];
	double y0 = par[1];
	double phi= par[2];

	for (int i = 0; i < I; i++){
		if (ydx[det]->GetBinEntries(i) > 1){
			dx = ydx[det]->GetBinContent(i);
			y = ydx[det]->GetBinCenter(i);
			x = yx[det]->GetBinContent(i);
			sigma_dx = ydx[det]->GetBinError(i);
			FCN_x += pow((dx - x0 - x*(cos(phi) - 1) + y*sin(phi)), 2) / pow(sigma_dx, 2);
		}
	}
	for (int j = 0; j < J; j++){
		if (xdy[det]->GetBinEntries(j) > 1){
			dy = xdy[det]->GetBinContent(j);
			y = xy[det]->GetBinContent(j);
			x = xdy[det]->GetBinCenter(j);
			sigma_dy = xdy[det]->GetBinError(j); 
			FCN_y += pow((dy - y0 - y*(cos(phi) - 1) - x*sin(phi)), 2) / pow(sigma_dy, 2);
		}
	}
	f = FCN_x + FCN_y;
	cout <<"f = " <<f <<endl;
	return;
}
void calc_hi_det_3 (Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){
	int det = 3;
	int I = ydx[det]->GetNbinsX();
	int J = xdy[det]->GetNbinsX();
	float dx=0, dy=0, x=0, y=0, sigma_dx=0, sigma_dy=0, FCN_x=0, FCN_y=0;
	double x0 = par[0];
	double y0 = par[1];
	double phi= par[2];

	for (int i = 0; i < I; i++){
		if (ydx[det]->GetBinEntries(i) > 1){
			dx = ydx[det]->GetBinContent(i);
			y = ydx[det]->GetBinCenter(i);
			x = yx[det]->GetBinContent(i);
			sigma_dx = ydx[det]->GetBinError(i);
			FCN_x += pow((dx - x0 - x*(cos(phi) - 1) + y*sin(phi)), 2) / pow(sigma_dx, 2);
		}
	}
	for (int j = 0; j < J; j++){
		if (xdy[det]->GetBinEntries(j) > 1){
			dy = xdy[det]->GetBinContent(j);
			y = xy[det]->GetBinContent(j);
			x = xdy[det]->GetBinCenter(j);
			sigma_dy = xdy[det]->GetBinError(j); 
			FCN_y += pow((dy - y0 - y*(cos(phi) - 1) - x*sin(phi)), 2) / pow(sigma_dy, 2);
		}
	}
	f = FCN_x + FCN_y;
	cout <<"f = " <<f <<endl;
	return;
}

double nextDouble (string &line, int &index){
	int size=line.length();
	while(index<size && (line[index]==' ' || line[index]=='\t')){
		index++;
	}
	int start=index;
	while(index<size && line[index]!=' ' && line[index] != '\t'){
		index++;
	}
	string text = line.substr(start,index-start);
	return atof(text.c_str());
}

double rootSquare (double x, double y){
	return sqrt(x*x + y*y);
}

const char* progname = "calibrateAlign";

void Usage()
{
    cout << "Usage: " << progname << " [options] file.root...\n"
            "Align GEM geometry\n"
            " -B         - work in batch mode without ROOT interpreter\n"
            " -b number  - detector index to be asumed base (0..3), default " << base << "\n"
            " -i cfgfile - input GEM geometry configuation file, default " << icfgfn << "\n"
            " -o cfgfile - output GEM geometry configuation file, default " << ocfgfn << "\n"
            " -s size    - square size of cut for shift alignment, default " << squareSize << " mm\n"
            " -w size    - rectangle width size of cut for rotation alignment, default "<< rectWidth << " mm\n"
            " -h         - show this help" << endl;
}

//==================================================================================================================
int main(int argc, char* argv[])
{
    progname = argv[0];
    int opt;

    while((opt = getopt(argc, argv, "hBi:o:b:s:w:"))!=-1) {
        switch(opt) {
        case 'B': batch = true; break;
        case 'b': base = atoi(optarg); break;
        case 'i': icfgfn = optarg; break;
        case 'o': ocfgfn = optarg; break;
        case 's': squareSize = atof(optarg); break;
        case 'w': rectWidth = atof(optarg); break;
        case 'h': Usage(); return 0;
        default: Usage(); return 1;
        }
    }
    Float_t rectHeight = squareSize;

    for (int argIdx = optind ; argIdx  < argc; ++argIdx) {
        const char * fn = argv[argIdx];
        TFile *file = TFile::Open(fn,"READ");
        if( !file ) {
            cerr << "Can not open " << fn << endl;
            continue;
        }
        TTree* tree;
        file->GetObject("gem",tree);
        if( !tree ) {
            cerr << "No 'gem' tree is found in " << fn << endl;
            continue;
        }
        file->Close();
        delete file;

        gemChain.AddFile(fn);
    }

    Long_t nentries = gemChain.GetEntries();
    if( !nentries ) {
        cerr << "No valid ROOT files are given" << endl;
        Usage();
        return 2;
    }

    geom.readConfiguration(icfgfn);
    if( !geom.isValid() )
        return 3;

    cout << "Initial " << geom;

    cout << "Detector " << base << " is used as a base\n";

    TApplication app("", 0, 0, 0, 0);

    TString name, title;
    for(int i=0; i<4; i++) {
        name.Form("hxy%d",i);
        title.Form("GEM detector %d;X, mm;Y, mm",i);
        hxy[i] = new TH2F(name.Data(),title.Data(),128,-64.,64.,100,-50.,50.);
        hxy[i]->SetStats(0);
    }

    if(gemChain.SetBranchAddress("detClusters", &clusters2d) != 0) {
        cerr << "'gem' branch is absent in the input or has wrong type" << endl;
        return 3;
    }

    cout << "Chain 'gem' with " << nentries << " entries is composed" << endl;

    //-----------Rough shift alignment----------------

    cout << "First get all possible cluster positions and compute entry list" << endl;
    TEntryList *entryList = new TEntryList;
    TVector3 clusterCoords;
    for(Long64_t entry = 0; entry < nentries; ++entry) {
        gemChain.GetEntry(entry);
        assert(clusters2d);
        assert(clusters2d->GetEntries()==4);
        Int_t ndet = 0;
        for (size_t i = 0; i < 4; ++i) {
            const GemCluster & cluster = *dynamic_cast<GemCluster*>(clusters2d->At(i));
            assert(cluster.detector == i);
            if( cluster.dx<=0 || cluster.dy<=0 ) continue;
            ndet++;
            clusterCoords.SetXYZ(cluster.x, cluster.y, 0.);
            geom.transformToLabFrame(cluster.detector, clusterCoords);
            hxy[cluster.detector]->Fill(clusterCoords.X(),clusterCoords.Y());
        }
        if( ndet==4 ) entryList->Enter(entry,&gemChain);
    }

    cerr << entryList->GetN() << " entries (" << setprecision(3) << 100.*entryList->GetN()/gemChain.GetEntries()
         << "%) seleted with clusters in every detector" << endl;

    if( !entryList->GetN() )
        return 4;

    gemChain.SetEntryList(entryList);

    gStyle->SetPalette(1,0);
    TCanvas can("c1","GEM alignment",600,640);
    can.Divide(2,2);

    for(int i = 0; i < 4; ++i) {
       can.cd(i+1);
       hxy[i]->DrawCopy("col");
    }
    can.Update();

    //Use beam mean position in X as a reference for alignment. In Y sensitive size of GEMs is too small for this.
    Float_t xMeanPos = hxy[base]->GetMean(1);
    Float_t xMin = xMeanPos-squareSize/2., xMax = xMeanPos+squareSize/2.,
            yMin = -squareSize/2., yMax = squareSize/2.;

    cout.precision(3);
    cout << "Square cut " << squareSize << "x" << squareSize << " mm^2 at (x,y)=(" << xMeanPos << ",0.0) mm in the base detector" << endl;
    RectangleCutAndFill(base,xMin,xMax,yMin,yMax);

    //Correct GEM geometry parameters with shifts
    TLatex t(0.,40.,"");
    t.SetTextAlign(22);
    t.SetTextSize(0.05);
    t.SetTextColor(kBlack);
    for(int i = 0; i < 4; ++i) {
        can.cd(i+1);
        TObject* obj = gPad->GetPrimitive(hxy[i]->GetName());
        if( obj ) delete obj;

        hxy[i]->SetAxisRange(xMeanPos - 2*squareSize,xMeanPos + 2*squareSize,"X");
        hxy[i]->SetAxisRange(-2*squareSize,2*squareSize,"Y");
        hxy[i]->DrawCopy("col");
        gPad->Update();

        Float_t dx = hxy[base]->GetMean(1)-hxy[i]->GetMean(1);
        Float_t dy = hxy[base]->GetMean(2)-hxy[i]->GetMean(2);
        cout << "Detector "<<i<<": dx = " << dx << ", dy = " << dy << "\n";
        name.Form("#DeltaX=%.1fmm, #DeltaY=%.1fmm",dx,dy);
        t.SetTitle(name.Data());
        t.DrawClone();
        geom.setx(i,geom.x(i)+dx);
        geom.sety(i,geom.y(i)+dy);
    }
    can.Update();
	//can.SaveAs("hxy.root");

    cout << "Rectangular cut "  << rectWidth << "x" << rectHeight << " mm^2 at (x,y)=(" << xMeanPos << ",0.0) mm in the base detector" << endl;
    xMin = xMeanPos-rectWidth/2.; xMax = xMeanPos+rectWidth/2.;
    yMin = -rectHeight/2.; yMax = rectHeight/2.;


    RectangleCutAndFill(base,xMin,xMax,yMin,yMax);

    Float_t dphi[4];

    //Fit X-profile of XY-distributions with linear function
    for(int i = 0; i < 4; ++i) {
        can.cd(i+1);
        TObject* obj = gPad->GetPrimitive(hxy[i]->GetName());
        if( obj ) delete obj;

        //plot a proportional range
        hxy[i]->SetAxisRange(xMeanPos-rectWidth,xMeanPos+rectWidth,"X");
        hxy[i]->SetAxisRange(-rectWidth,rectWidth,"Y");
        hxy[i]->DrawCopy("col");
        gPad->Update();

        TProfile* p = hxy[i]->ProfileX("_px");
        p->SetStats(0);
        p->Fit("pol1","Q","same",xMin,xMax);
        dphi[i] = atan(p->GetFunction("pol1")->GetParameter(1));
    }

    //Correct GEM geometry parameters with rotations
    for(int i = 0; i < 4; ++i) {
        dphi[i] = dphi[base]-dphi[i];

        cout << "Detector "<<i<<": dphi = " << setprecision(4) << dphi[i]*180/TMath::Pi() << " deg\n";

        can.cd(i+1);
        name.Form("#Deltaphi = %.3f^{0}",dphi[i]*180/TMath::Pi());
        t.SetTitle(name.Data());
        t.DrawClone();

        geom.setphi(i,geom.phi(i)+dphi[i]);
    }
    can.Update();

    cout << "Modified " << geom;
//-----------------------------------------------------------------------------------------------------------
    for(int i=0; i<4; i++) {
        name.Form("dxdy%d",i);
        title.Form("GEM detector %d;X, mm;Y, mm",i);
        dxdy[i] = new TH2F(name.Data(),title.Data(),128,-63.5,64.5, 100,-54.5,55.5);
	dxdy[i]->GetXaxis()->SetTitle("dx");
	dxdy[i]->GetYaxis()->SetTitle("dy");

        name.Form("ydx%d",i);
        title.Form("GEM detector %d;X, mm;Y, mm",i);
        ydx[i] = new TProfile(name.Data(),title.Data(),(int)(2*(-yMin+yMax)),yMin,yMax,-54.5,55.5);
		ydx[i]->GetXaxis()->SetTitle("y");
		ydx[i]->GetYaxis()->SetTitle("dx");

        name.Form("xdy%d",i);
        title.Form("GEM detector %d;X, mm;Y, mm",i);
		xdy[i] = new TProfile(name.Data(),title.Data(),(int)(2*(-xMin+xMax)),xMin,xMax,-54.5,55.5);
		xdy[i]->GetYaxis()->SetTitle("dy");

        name.Form("xy%d",i);
        title.Form("GEM detector %d;X, mm;Y, mm",i);
		xy[i] = new TProfile(name.Data(),title.Data(),(int)(2*(-xMin+xMax)),xMin,xMax,-54.5,55.5);

        name.Form("yx%d",i);
        title.Form("GEM detector %d;X, mm;Y, mm",i);
        yx[i] = new TProfile(name.Data(),title.Data(),(int)(2*(-yMin+yMax)),yMin,yMax,-54.5,55.5);
		
    }

	int det = 1;		
	xMax = hxy[det]->GetMean(1) + hxy[det]->GetRMS(1); cout <<"xMax = " <<xMax <<endl;//rectWidth/2.
	xMin = hxy[det]->GetMean(1) - hxy[det]->GetRMS(1); cout <<"xMin = " <<xMin <<endl;

	yMax = hxy[det]->GetMean(2) + hxy[det]->GetRMS(2); cout <<"yMax = " <<yMax <<endl;//rectHeight/2.
	yMin = hxy[det]->GetMean(2) - hxy[det]->GetRMS(2); cout <<"yMin = " <<yMin <<endl;

	TrackConstructionAndFill(base, xMin, xMax, yMin, yMax, tail);

/*
    TCanvas can_dxdy("c3","deviate from track dx dy",600,640); can_dxdy.Divide(2,2);
    //TCanvas can_ydx("c4","y dx",600,640); can_ydx.Divide(2,2);
    //TCanvas can_xdy("c5","x dy",600,640); can_xdy.Divide(2,2);
    //TCanvas can_xy("c5","x y",600,640); can_xy.Divide(2,2);
    //TCanvas can_yx("c5","y x",600,640); can_yx.Divide(2,2);

	for (int i = 0; i < 4; i++){
		can_dxdy.cd(i+1); dxdy[i]->Draw("colz");
		//can_ydx.cd(i+1); ydx[i]->Draw("colz");
		//can_xdy.cd(i+1); xdy[i]->Draw("colz");
		//can_xy.cd(i+1); xy[i]->Draw("colz");
		//can_yx.cd(i+1); yx[i]->Draw("colz");
	}
	can_dxdy.Update();
	//can_ydx.Update();
	//can_xdy.Update();
	//can_xy.Update();
	//can_yx.Update();
*/
	cout <<"Print resault of track calibration:" <<endl;
	for (int i = 0; i < 4; i++){
		Float_t dx = dxdy[base]->GetMean(1)-dxdy[i]->GetMean(1);
		Float_t dy = dxdy[base]->GetMean(2)-dxdy[i]->GetMean(2);
		cout <<"Detector " <<i <<": dx = " <<dx <<", dy = " <<dy <<endl;
		geom.setx(i,geom.x(i)+dx);
		geom.sety(i,geom.y(i)+dy);
	}
	cout << "Modified " << geom;

	//char fname[28];
	//sprintf(fname,"%s.root",ocfgfn);
	//can_dxdy.SaveAs("dxdy.root");
//------------------------------------------------------------------------------------------------------
	Int_t npar=3;
	TMinuit *pt = new TMinuit(npar);
	pt->SetPrintLevel(1);
	pt->SetFCN(calc_hi_det_1);

	Double_t arglist[10];
	Int_t ierflg = 0;
	arglist[0] = 10;
	arglist[1] = 0.5;

	// Set starting values and step sizes for parameters
	static Double_t vstart = 0.1;
	static Double_t step = 0.01;
	static Double_t bmin = -5;
	static Double_t bmax = 5;
	static Double_t bminphi = -0.1;
	static Double_t bmaxphi = 0.1;

	pt->mnparm(0, "x0", vstart, step, bmin, bmax,ierflg);
	pt->mnparm(1, "y0", vstart, step, bmin, bmax,ierflg);
	pt->mnparm(2, "phi", 0.01, 0.005, bminphi, bmaxphi,ierflg);

	// Now ready for minimization step
	arglist[0] = 500;
	arglist[1] = 1.;
	pt->SetErrorDef(1);
	pt->Command("SET STRATEGY 1");
	pt->Command("SET ERR 1");
	pt->mnexcm("MIGRAD", arglist ,2,ierflg);

	// Print results
	cout << "\nPrint results from minuit\n";
	double fParamVal;
	double fParamErr;

	pt->GetParameter(0,fParamVal,fParamErr);
	geom.setx(1,geom.x(1) + fParamVal); cout << "dx = " << fParamVal <<"\terror = " <<fParamErr << "\n";

	pt->GetParameter(1,fParamVal,fParamErr);
	geom.sety(1,geom.y(1) + fParamVal); cout << "dy = " << fParamVal <<"\terror = " <<fParamErr  << "\n";

	pt->GetParameter(2,fParamVal,fParamErr);
	geom.setphi(1,geom.phi(1)+fParamVal); cout << "phi = " << fParamVal <<"\terror = " <<fParamErr  << "\n";

    cout << "Modified minuit " << geom;

//---------------------------------------------------------------------------------------------------------
	if (tail == 3) {
		det = 0;
		xMax = hxy[det]->GetMean(1) + hxy[det]->GetRMS(1); cout <<"xMax = " <<xMax <<endl;//rectWidth/2.
		xMin = hxy[det]->GetMean(1) - hxy[det]->GetRMS(1); cout <<"xMin = " <<xMin <<endl;

		yMax = hxy[det]->GetMean(2) + hxy[det]->GetRMS(2); cout <<"yMax = " <<yMax <<endl;//rectHeight/2.
		yMin = hxy[det]->GetMean(2) - hxy[det]->GetRMS(2); cout <<"yMin = " <<yMin <<endl;

		TrackConstructionAndFill(base, xMin, xMax, yMin, yMax, tail);

		cout <<"Print resault of track calibration:" <<endl;
		for (int i = 0; i < 4; i++){
			Float_t dx = dxdy[base]->GetMean(1)-dxdy[i]->GetMean(1);
			Float_t dy = dxdy[base]->GetMean(2)-dxdy[i]->GetMean(2);
			cout <<"Detector " <<i <<": dx = " <<dx <<", dy = " <<dy <<endl;
			geom.setx(i,geom.x(i)+dx);
			geom.sety(i,geom.y(i)+dy);
		}
		cout << "Modified " << geom;
		
		Int_t npar=3;
		TMinuit *pt = new TMinuit(npar);
		pt->SetPrintLevel(1);
		pt->SetFCN(calc_hi_det_0);
	
		Double_t arglist[10];
		Int_t ierflg = 0;
		arglist[0] = 10;
		arglist[1] = 0.5;
	
		// Set starting values and step sizes for parameters
		static Double_t vstart = 0.1;
		static Double_t step = 0.01;
		static Double_t bmin = -5;
		static Double_t bmax = 5;
		static Double_t bminphi = -0.1;
		static Double_t bmaxphi = 0.1;
	
		pt->mnparm(0, "x0", vstart, step, bmin, bmax,ierflg);
		pt->mnparm(1, "y0", vstart, step, bmin, bmax,ierflg);
		pt->mnparm(2, "phi", 0.01, 0.005, bminphi, bmaxphi,ierflg);
	
		// Now ready for minimization step
		arglist[0] = 500;
		arglist[1] = 1.;
		pt->SetErrorDef(1);
		pt->Command("SET STRATEGY 1");
		pt->Command("SET ERR 1");
		pt->mnexcm("MIGRAD", arglist ,2,ierflg);
	
		// Print results
		cout << "\nPrint results from minuit\n";
		double fParamVal;
		double fParamErr;
	
		pt->GetParameter(0,fParamVal,fParamErr);
		geom.setx(1,geom.x(1) + fParamVal); cout << "dx = " << fParamVal <<"\terror = " <<fParamErr << "\n";
	
		pt->GetParameter(1,fParamVal,fParamErr);
		geom.sety(1,geom.y(1) + fParamVal); cout << "dy = " << fParamVal <<"\terror = " <<fParamErr  << "\n";
	
		pt->GetParameter(2,fParamVal,fParamErr);
		geom.setphi(1,geom.phi(1)+fParamVal); cout << "phi = " << fParamVal <<"\terror = " <<fParamErr  << "\n";
	
    	cout << "Modified minuit " << geom;
	}
	if (tail == 0) {
		det = 3;
		xMax = hxy[det]->GetMean(1) + hxy[det]->GetRMS(1); cout <<"xMax = " <<xMax <<endl;//rectWidth/2.
		xMin = hxy[det]->GetMean(1) - hxy[det]->GetRMS(1); cout <<"xMin = " <<xMin <<endl;

		yMax = hxy[det]->GetMean(2) + hxy[det]->GetRMS(2); cout <<"yMax = " <<yMax <<endl;//rectHeight/2.
		yMin = hxy[det]->GetMean(2) - hxy[det]->GetRMS(2); cout <<"yMin = " <<yMin <<endl;

		TrackConstructionAndFill(base, xMin, xMax, yMin, yMax, tail);

		cout <<"Print resault of track calibration:" <<endl;
		for (int i = 0; i < 4; i++){
			Float_t dx = dxdy[base]->GetMean(1)-dxdy[i]->GetMean(1);
			Float_t dy = dxdy[base]->GetMean(2)-dxdy[i]->GetMean(2);
			cout <<"Detector " <<i <<": dx = " <<dx <<", dy = " <<dy <<endl;
			geom.setx(i,geom.x(i)+dx);
			geom.sety(i,geom.y(i)+dy);
		}
		cout << "Modified " << geom;
		
		Int_t npar=3;
		TMinuit *pt = new TMinuit(npar);
		pt->SetPrintLevel(1);
		pt->SetFCN(calc_hi_det_3);
	
		Double_t arglist[10];
		Int_t ierflg = 0;
		arglist[0] = 10;
		arglist[1] = 0.5;
	
		// Set starting values and step sizes for parameters
		static Double_t vstart = 0.1;
		static Double_t step = 0.01;
		static Double_t bmin = -5;
		static Double_t bmax = 5;
		static Double_t bminphi = -0.1;
		static Double_t bmaxphi = 0.1;
	
		pt->mnparm(0, "x0", vstart, step, bmin, bmax,ierflg);
		pt->mnparm(1, "y0", vstart, step, bmin, bmax,ierflg);
		pt->mnparm(2, "phi", 0.01, 0.005, bminphi, bmaxphi,ierflg);
	
		// Now ready for minimization step
		arglist[0] = 500;
		arglist[1] = 1.;
		pt->SetErrorDef(1);
		pt->Command("SET STRATEGY 1");
		pt->Command("SET ERR 1");
		pt->mnexcm("MIGRAD", arglist ,2,ierflg);
	
		// Print results
		cout << "\nPrint results from minuit\n";
		double fParamVal;
		double fParamErr;
	
		pt->GetParameter(0,fParamVal,fParamErr);
		geom.setx(1,geom.x(1) + fParamVal); cout << "dx = " << fParamVal <<"\terror = " <<fParamErr << "\n";
	
		pt->GetParameter(1,fParamVal,fParamErr);
		geom.sety(1,geom.y(1) + fParamVal); cout << "dy = " << fParamVal <<"\terror = " <<fParamErr  << "\n";
	
		pt->GetParameter(2,fParamVal,fParamErr);
		geom.setphi(1,geom.phi(1)+fParamVal); cout << "phi = " << fParamVal <<"\terror = " <<fParamErr  << "\n";
	
    	cout << "Modified minuit " << geom;
	}
//---------------------------------------------------------------------------------------------------------
    cout << "Writing new GEM geometry configuration in " << ocfgfn << endl;
    ofstream fout(ocfgfn);
    fout << geom;
    fout.close();
//---------------------------------------------------------------------------------------------------------
    //Check geometry
    geom.readConfiguration(ocfgfn);
    RectangleCutAndFill(base, 0, 10, 10, 20);
    for(int i = 0; i < 4; ++i) {
        can.cd(i+1);
        hxy[i]->Draw("same");
    }
    can.Update();
	//cin >>base;	
//===========================================================================================================//
	ofstream result;
	result.open("result.dat", ios::app);	
	string line,str = ocfgfn;

	TString ss = str.substr(16, 20);	//"2013-01-27_08:07.dat"
	ss.ReplaceAll("_"," ");
	ss.ReplaceAll(".dat",":00");
	TDatime dt;
	dt.Set(ss.Data());

	result <<ocfgfn <<"\t";
	result <<dt.Convert() <<"\t|";

	double RMS_x, RMS_y, NEntr;
	double x,y,z,phi;
	int detect;
	ifstream data;
	data.open(ocfgfn);
	getline (data, line);
	getline (data, line);
	while (getline (data, line)){
		int index = 0;
		detect=nextDouble(line,index);
		x=nextDouble(line,index);
		y=nextDouble(line,index);
		z=nextDouble(line,index);
		phi=nextDouble(line,index);
		RMS_x = dxdy[detect]->GetRMS(1);
		NEntr = dxdy[detect]->GetEntries();
		RMS_y = dxdy[detect]->GetRMS(2);
		result <<x <<"\t" <<rootSquare(RMS_x/sqrt(NEntr), 0.07) <<"\t|" <<y <<"\t" <<rootSquare(RMS_y/sqrt(NEntr), 0.15) <<"\t|" <<phi <<"\t";
	}
	result <<nentries <<endl;

	data.close();
	result.close();

    if( !batch ) 
         app.Run(kTRUE);

    return 0;
}
