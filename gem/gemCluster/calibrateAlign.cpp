#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <memory>
#include <cassert>

#include <unistd.h> //getopt, access

#include <boost/foreach.hpp>

#include <TROOT.h>
#include <TRint.h>
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


static gem::Geometry geom;
static gem::Geometry geom_err;
static TChain gemChain("gem");
static TClonesArray *clusters2d = 0;

static bool batch = false;																								//-B
static bool doMinuit = true;																							//-M
static bool FineAlignment = false;																						//-F
static int base = 2; // number of base detector 0,1,2,...; geom.index(base) give you index of base detector				//-b
static int tail = 0; // number of tail detector for track construction													//-t
static const char *icfgfn = "gem_geom_config.dat";																		//-i
static const char *ocfgfn = "gem_geom_config_align.dat";																//-o
static Float_t squareSize = 20;	//mm																					//-s
static Float_t rectWidth = 25;	//mm																					//-w
static Float_t rectHeight = squareSize;


static TH2F **hxy;
static TH2F **dxdy;


static double **arr_x;
static double **arr_y;
static double **arr_dx;
static double **arr_dy;
static Long64_t arr_n = 0;


static int minuit_det = 0;
static double sigma_dx = 0;
static double sigma_dy = 0;


//calibrateAlign.cpp is operate only with serial number of GEM detectors and connect it with number histogram
//index of GEM detectors is used in GemGeometry.h only for determine type of detector. geom.index(num) is convert number in index for GemGeometry methods



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------Definition of functions-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------



void EventSelectionAndSave(Int_t base, Float_t xMin, Float_t xMax, Float_t yMin, Float_t yMax, Int_t tail=-1, Bool_t Rshift=false){
	assert(base != tail);

	for(int i = 0; i < geom.numdet(); i++) {
		hxy[i]->Reset();
		if( tail>=0 ){
			dxdy[i]->Reset();
		}
	}

	TVector3 position[geom.numdet()];
	Long64_t nListEntries = gemChain.GetEntryList()->GetN();
	arr_n = 0;
	for(Long64_t entry = 0; entry < nListEntries; ++entry) {
		Long64_t entryNumber = gemChain.GetEntryNumber(entry);
		gemChain.GetEntry(entryNumber);

		Bool_t selected = true;
		if (Rshift) selected = false;
		
		for(size_t i = 0; i < geom.numdet(); i++) {
			assert(clusters2d->At(i) != 0);		
			const GemCluster & cluster = *dynamic_cast<GemCluster*>(clusters2d->At(i));
			assert(cluster.dx>0 && cluster.dy>0);
			size_t index = cluster.detector;
			position[i].SetXYZ(cluster.x, cluster.y, 0.);
			geom.transformToLabFrame(index, position[i]);

			if (Rshift) {
				if( index==geom.index(base) && position[i].X()>=xMin && position[i].X()<=xMax && position[i].Y()>=yMin && position[i].Y()<=yMax ){
					selected = true;
				}
			} else {
				if( position[i].X()<xMin || position[i].X()>xMax || position[i].Y()<yMin || position[i].Y()>yMax ) {
					selected = false;
				}
			}
		}

		if( !selected ) continue;

		for(int i = 0; i < geom.numdet(); i++){
			hxy[i]->Fill(position[i].X(), position[i].Y());
			if( tail>=0) {
				arr_x[i][arr_n] = position[i].X();
				arr_y[i][arr_n] = position[i].Y();
				double x_track = ((position[i].Z() - position[base].Z()) / (position[tail].Z() - position[base].Z())) * (position[tail].X() - position[base].X()) + position[base].X();
				double y_track = ((position[i].Z() - position[base].Z()) / (position[tail].Z() - position[base].Z())) * (position[tail].Y() - position[base].Y()) + position[base].Y();
				arr_dx[i][arr_n] = -position[i].X() + x_track;
				arr_dy[i][arr_n] = -position[i].Y() + y_track;
				
				dxdy[i]->Fill(-position[i].X() + x_track, -position[i].Y() + y_track);
			}

		}
		
		arr_n++;
		gSystem->ProcessEvents();
	}
	cout <<"EventSelectionAndSave: "<< arr_n << " entries selected with hits in rectangle" << endl;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

void calc_hi (Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){
	double FCN_x = 0;
	double FCN_y = 0;
	double x0 = par[0];
	double y0 = par[1];
	double phi= par[2];

	for(Long64_t entry = 0; entry < arr_n; ++entry) {
		FCN_x += pow((arr_dx[minuit_det][entry] - x0 - arr_x[minuit_det][entry]*(cos(phi) - 1) + arr_y[minuit_det][entry]*sin(phi)), 2) / (sigma_dx*sigma_dx);
		FCN_y += pow((arr_dy[minuit_det][entry] - y0 - arr_y[minuit_det][entry]*(cos(phi) - 1) - arr_x[minuit_det][entry]*sin(phi)), 2) / (sigma_dy*sigma_dy);
		gSystem->ProcessEvents();
	}

	f = FCN_x + FCN_y;
	//cout <<"fcn(" <<minuit_det <<") = " <<f <<endl;
	return;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------



//=============================================================================================================================================================

const char* progname = "calibrateAlign";

void Usage(){
	cout << "Usage: " << progname << " [options] file.root...\n"
			"Align GEM geometry\n"
			" -B				- work in batch mode without ROOT interpreter\n"
			" -M				- do not invoke Minuit for fine track alignment, do this by default\n"
			" -F				- Enabling fine alignment, by default rough alignment\n"
			" -b number			- detector number to be asumed base (0..3..), default " << base << "\n"
			" -t number			- detector number to be asumed tail (0..3..), default " << tail << "\n"
			" -i cfgfile		- input GEM geometry configuation file, default " << icfgfn << "\n"
			" -o cfgfile		- output GEM geometry configuation file, default " << ocfgfn << " Ignore for Rough Alignment\n"
			" -s size			- square size of cut for shift alignment, default " << squareSize << " mm\n"
			" -w size			- rectangle width size of cut for rotation alignment, default "<< rectWidth << " mm\n"
			" -h				- show this help" << endl;
}
//"hBMFi:o:b:t:s:w:"

//=============================================================================================================================================================

int main(int argc, char* argv[]){
	progname = argv[0];
	
	int opt;
	while((opt = getopt(argc, argv, "BMFb:t:i:o:s:w:h"))!=-1) {
		switch(opt) {
		case 'B': batch = true; break;
		case 'M': doMinuit = false; break;
		case 'F': FineAlignment = true; break;
		case 'b': base = atoi(optarg); break;
		case 't': tail = atoi(optarg); break;
		case 'i': icfgfn = optarg; break;
		case 'o': ocfgfn = optarg; break;
		case 's': squareSize = atof(optarg); break;
		case 'w': rectWidth = atof(optarg); break;
		case 'h': Usage(); return 0;
		default: Usage(); return 1;
		}
	}

	const char *fn;
	for (int argIdx = optind ; argIdx  < argc; ++argIdx) {
		fn = argv[argIdx];
		
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
	if(!geom.readConfiguration(icfgfn)) {
		return 3;
	}

	cout << "Initial " <<geom;
	cout << "Detector " <<geom.index(base) << "(" << base << ") is used as a base\n";

	int ac = 2;
	char *av[2] = {argv[0], "-n"};
	TRint app("rootint", &ac, av, 0, 0);
	
	if(gemChain.SetBranchAddress("detClusters", &clusters2d) != 0) {
		cerr << "'gem' branch is absent in the input or has wrong type" << endl;
		return 4;
	}

	cout << "Chain 'gem' with " << nentries << " entries is composed" << endl;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------


	int divx, divy;
	if(geom.numdet()<=2){
		divx=2; divy=1;
	} else if(geom.numdet()<=4){
		divx=2; divy=2;	
	} else if(geom.numdet()<=6){
		divx=3; divy=2;	
	} else if(geom.numdet()<=9){
		divx=3; divy=3;	
	} else if(geom.numdet()<=12){
		divx=4; divy=3;
	}
	gStyle->SetPalette(1,0);

	TString name, title;


	TLatex t(0.,40.,"");
	t.SetTextAlign(22);
	t.SetTextSize(0.05);
	t.SetTextColor(kBlack);

//---------------------------------------------------------Initialize hxy and canvas for it--------------------------------------------------------------------

	hxy = new TH2F* [geom.numdet()];
	for(int i=0; i<geom.numdet(); i++) {
		name.Form("hxy%d",geom.index(i));
		title.Form("GEM detector %d;X, mm;Y, mm",geom.index(i));
		hxy[i] = new TH2F(name.Data(),title.Data(),128,-64.,64.,100,-50.,50.);
		hxy[i]->SetStats(1);
	}

	TCanvas can("c1","GEM alignment",600,640);
	can.Divide(divx,divy);



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------Fill Entry List-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------



	cout << "First get all possible cluster positions and compute entry list" << endl;
	TEntryList *entryList = new TEntryList;
	TVector3 clusterCoords;
	
	for(Long64_t entry = 0; entry < nentries; ++entry) {
		gemChain.GetEntry(entry);
		assert(clusters2d->GetEntries() == geom.numdet());
		size_t nclusters=0;
		for (size_t i = 0; i < geom.numdet(); i++) {
			assert(clusters2d->At(i) != 0);
			const GemCluster & cluster = *dynamic_cast<GemCluster*>(clusters2d->At(i));
			size_t index = cluster.detector;
			if( !geom.defined(index) || cluster.dx<=0 || cluster.dy<=0 ) continue;
			nclusters++;
			//cout <<"entry = " <<entry <<"\ti = "<<i <<"\tcluster.detector = "<< index <<"\tcluster.x = "<< cluster.x << "\tcluster.y = "<< cluster.y <<endl;
			clusterCoords.SetXYZ(cluster.x, cluster.y, 0.);
			geom.transformToLabFrame(index, clusterCoords);
			hxy[i]->Fill(clusterCoords.X(),clusterCoords.Y());
		}
		if( nclusters==geom.numdet() ) entryList->Enter(entry,&gemChain);
	}

	cerr << entryList->GetN() << " entries (" << setprecision(3) << 100.*entryList->GetN()/gemChain.GetEntries() << "%) seleted with clusters in every detector" << endl;


	if( !entryList->GetN() )
		return 4;

	gemChain.SetEntryList(entryList);

	//----------------------------------------------
	for(int i = 0; i < geom.numdet(); i++) {
		can.cd(i+1)->Clear();
		hxy[i]->Draw("col");
	}
	
	if( !batch ){
		can.Update();
		app.Run(kTRUE);
	}
	
	
	
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------Rough track shift&rotation alignment----------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

	if (!FineAlignment){
	//---------------------------------------------------------Rough shift alignment-------------------------------------------------------------------------------

		//Use beam mean position in X as a reference for alignment. In Y sensitive size of GEMs is too small for this.
		Float_t xMeanPos = hxy[base]->GetMean(1);
		Float_t xMin = xMeanPos-squareSize/2., xMax = xMeanPos+squareSize/2., yMin = -squareSize/2., yMax = squareSize/2.;

		cout.precision(3);
		cout << "Square cut " << squareSize << "x" << squareSize << " mm^2 at (x,y)=(" << xMeanPos << ",0.0) mm in the only base detector" << endl;

		EventSelectionAndSave(base, xMin, xMax, yMin, yMax, -1, true); //Rough shift
	
		//Correct GEM geometry parameters with shifts::stage -> draw
		for(int i = 0; i < geom.numdet(); i++) {
			//plot a proportional range
			can.cd(i+1)->Clear();
			hxy[i]->SetAxisRange(xMeanPos - 2*squareSize, xMeanPos + 2*squareSize,"X");
			hxy[i]->SetAxisRange(-2*squareSize, 2*squareSize,"Y");
			hxy[i]->Draw("colz");
		}
		//Correct GEM geometry parameters with shifts::stage -> setx & sety
		for(int i = 0; i < geom.numdet(); i++) {
			float dx = hxy[base]->GetMean(1)-hxy[i]->GetMean(1);
			float dy = hxy[base]->GetMean(2)-hxy[i]->GetMean(2);
			cout <<"Detector " <<i <<": dx = " <<dx <<", dy = " <<dy <<endl;
			int GEM_i = geom.index(i);
			geom.setx(GEM_i, geom.x(GEM_i)+dx);
			geom.sety(GEM_i, geom.y(GEM_i)+dy);
		
			can.cd(i+1);
			name.Form("#DeltaX=%.1fmm, #DeltaY=%.1fmm",dx,dy);
			t.DrawLatex(xMeanPos, 1.6*squareSize, name.Data());
		}
		cout << "Modified geometry by Rough shift" <<geom;
	
		if( !batch ){
			can.Update();
			app.Run(kTRUE);
		}

	//---------------------------------------------------------Rough rotation alignment----------------------------------------------------------------------------

		xMin = xMeanPos-rectWidth/2.; xMax = xMeanPos+rectWidth/2.;	yMin = -rectHeight/2.; yMax = rectHeight/2.;

		cout.precision(3);
		cout << "Rectangular cut "  << rectWidth << "x" << rectHeight << " mm^2 at (x,y)=(" << xMeanPos << ",0.0) mm in all detector" << endl;

		EventSelectionAndSave(base, xMin, xMax, yMin, yMax);

		float *dphi = new float [geom.numdet()];
		//Fit X-profile of XY-distributions with linear function
		for(int i = 0; i < geom.numdet(); i++) {
			//plot a proportional range
			can.cd(i+1)->Clear();
			hxy[i]->SetAxisRange(xMeanPos-rectWidth,xMeanPos+rectWidth,"X");
			hxy[i]->SetAxisRange(-rectHeight,rectHeight,"Y");
			hxy[i]->Draw("colz");
		
			TProfile* p = hxy[i]->ProfileX("_px");
			p->SetStats(1);
			p->Fit("pol1","Q","same",xMin,xMax);
			dphi[i] = atan(p->GetFunction("pol1")->GetParameter(1));
		}
		//Correct GEM geometry parameters with rotations
		for(int i = 0; i < geom.numdet(); i++) {
			dphi[i] = dphi[base]-dphi[i];
			cout << "Detector "<<i<<": dphi = " << setprecision(4) << dphi[i]*180/TMath::Pi() << " deg\n";
			int GEM_i = geom.index(i);
			geom.setphi(GEM_i, geom.phi(GEM_i)+dphi[i]);

			can.cd(i+1);
			name.Form("#Deltaphi = %.3f^{0}",dphi[i]*180/TMath::Pi());
			t.DrawLatex(xMeanPos,rectWidth*0.7,name.Data());
		}
		delete [] dphi;
		cout << "Modified geometry by Rough rotation" <<geom;
	
		if( !batch ){
			can.Update();
			app.Run(kTRUE);
		}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	}//end of Rough Alignment

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------End of Rough Alignment------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------Fine track shift&rotation alignment-----------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

	if (FineAlignment) {

		//---------------------------------------------------------Initialize arrays for temporary storage-----------------------------------------------------

		arr_x = new double* [geom.numdet()];
		arr_y = new double* [geom.numdet()];
		arr_dx = new double* [geom.numdet()];
		arr_dy = new double* [geom.numdet()];
		for(int i = 0; i < geom.numdet(); i++) {
			arr_x[i] = new double [entryList->GetN()];
			arr_y[i] = new double [entryList->GetN()];
			arr_dx[i] = new double [entryList->GetN()];
			arr_dy[i] = new double [entryList->GetN()];
		}

		//---------------------------------------------------------Initialize dxdy and canvas for it-----------------------------------------------------------

		dxdy = new TH2F* [geom.numdet()];
		for(int i=0; i<geom.numdet(); i++) {
			name.Form("dxdy%d",geom.index(i));
			title.Form("GEM detector %d;dx, mm;dy, mm",geom.index(i));
			dxdy[i] = new TH2F(name.Data(),title.Data(),400,-20.,20., 400,-20.,20.);
			dxdy[i]->SetStats(1);
		}

		TCanvas can_dxdy("c2","deviate from track dx dy",600,640);
		can_dxdy.Divide(divx,divy);	

		//---------------------------------------------------------Track Construction--------------------------------------------------------------------------

		Float_t xMeanPos = hxy[base]->GetMean(1);
		Float_t xMin = xMeanPos-squareSize/2., xMax = xMeanPos+squareSize/2., yMin = -squareSize/2., yMax = squareSize/2.;
		
		cout.precision(3);
		cout << "Square cut for track aligh" << squareSize << "x" << squareSize << " mm^2 at (x,y)=(" << xMeanPos << ",0.0) mm in the all detector" << endl;
		cout <<xMin<<" < X < "<<xMax <<"\t\t" <<yMin<<" < Y < "<<yMax  <<endl;

		cout <<"\"The track is constructing between the base Gem#" <<base <<"(" <<geom.index(base) <<")" <<" and Gem#" <<tail <<"(" <<geom.index(tail) <<")" <<endl;
		EventSelectionAndSave(base, xMin, xMax, yMin, yMax, tail);

		cout <<"Print result of track calibration:" <<endl;
		for(int i=0; i<geom.numdet(); i++) {
			can_dxdy.cd(i+1)->Clear();
			dxdy[i]->Draw("colz");
	
			float dx = dxdy[i]->GetMean(1);
			float dy = dxdy[i]->GetMean(2);
			cout <<"TrackConstruction::Detector num " <<i <<": dx = " <<dx <<", dy = " <<dy <<endl;
			int GEM_i = geom.index(i);
			geom.setx(GEM_i, geom.x(GEM_i)+dx);
			geom.sety(GEM_i, geom.y(GEM_i)+dy);
	
			can_dxdy.cd(i+1);
			name.Form("#DeltaX=%.1fmm, #DeltaY=%.1fmm", dx, dy);
			t.DrawLatex(0, 0.7*squareSize, name.Data());
		}
		cout <<"Modified geometry by track " <<base <<"->" <<tail <<" construction\n" <<geom;

		if( !batch ){
			can.Update();
			can_dxdy.Update();
			app.Run(kTRUE);
		}

		EventSelectionAndSave(base, xMin, xMax, yMin, yMax, tail);

		//-------------------------------------------------Fine minuit alignment inside each track construction------------------------------------------------

		for(minuit_det=0; minuit_det<geom.numdet(); minuit_det++){
			if (!doMinuit ) continue;
			if (minuit_det == tail || minuit_det == base) continue;


			int index_min = geom.index(minuit_det);
			cout <<"Detector for minuit alignment" <<index_min << "(" << minuit_det << ")\n";

			sigma_dx = dxdy[minuit_det]->GetRMS(1);
			sigma_dy = dxdy[minuit_det]->GetRMS(2);
			cout <<"\tRMS_x dxdy = sigma_dx = " <<sigma_dx <<endl;
			cout <<"\tRMS_y dxdy = sigma_dy = " <<sigma_dy <<endl<<endl;


			Int_t npar=3;
			TMinuit *pt = new TMinuit(npar);
			pt->SetPrintLevel(1);
			pt->SetFCN(calc_hi);

			Double_t arglist[10];
			Int_t ierflg = 0;
			arglist[0] = 10;
			arglist[1] = 0.5;

			// Set starting values and step sizes for parameters
			static Double_t step = 0.05;
			static Double_t bMax = 15.0;
			static Double_t bMin = -15.0;
			pt->mnparm(0, "x0", geom.x(index_min), step, bMin, bMax, ierflg);
			pt->mnparm(1, "y0", geom.y(index_min), step, bMin, bMax, ierflg);

			static Double_t stepPhi = 0.01;
			static Double_t bMaxPhi = 3.0;
			static Double_t bMinPhi = -3.0;
			pt->mnparm(2, "phi", geom.phi(index_min), stepPhi, bMinPhi, bMaxPhi, ierflg);
	
			// Now ready for minimization step
			arglist[0] = 500;
			arglist[1] = 1.;
			pt->SetErrorDef(1);
			pt->Command("SET STRATEGY 1");
			pt->Command("SET ERR 1");
			pt->mnexcm("MIGRAD", arglist ,2,ierflg);

			cout << "\nPrint results from minuit\n";
			double fParamVal, fParamErr;

			pt->GetParameter(0, fParamVal, fParamErr);
				cout << "dx = " << fParamVal <<"\terror = " <<fParamErr << "\n";
				geom.setx(index_min, geom.x(index_min)+fParamVal);
				geom_err.setx(index_min, fParamErr);
		
			pt->GetParameter(1, fParamVal, fParamErr);
				cout << "dy = " << fParamVal <<"\terror = " <<fParamErr  << "\n";
				geom.sety(index_min, geom.y(index_min)+fParamVal);
				geom_err.sety(index_min, fParamErr);
		
			pt->GetParameter(2, fParamVal, fParamErr);
				cout << "dphi = " << fParamVal <<"\terror = " <<fParamErr  << "\n";
				geom.setphi(index_min, geom.phi(index_min)+fParamVal);
				geom_err.setphi(index_min, fParamErr);

			cout <<"Modified geometry by minuit for " <<minuit_det <<" GEM detector; track " <<base <<"->" <<tail <<" is used\n" <<geom;
	
			if( !batch ){
				can.Update();
				can_dxdy.Update();
				app.Run(kTRUE);
			}

		}//end of minuit

	}//end of Fine Alignment

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------End of Fine Alignment-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

	if (!FineAlignment) ocfgfn = "gem_geom_config_rough_align.dat";

	cout << "Writing new GEM geometry configuration in " << ocfgfn << endl;
	ofstream fout(ocfgfn);
	fout <<geom;
	fout.close();



//=============================================================================================================================================================



//---------------------------------------------------------Check geometry--------------------------------------------------------------------------------------

	if( !batch && !FineAlignment){
		geom.readConfiguration(ocfgfn);
		
		Float_t xMeanPos = hxy[base]->GetMean(1);
		Float_t xMin = xMeanPos-squareSize/2., xMax = xMeanPos+squareSize/2., yMin = -squareSize/2., yMax = squareSize/2.;

		EventSelectionAndSave(base, xMin, xMax, yMin, yMax);	
		for(int i = 0; i < geom.numdet(); i++) {
			can.cd(i+1)->Clear();
			hxy[i]->SetAxisRange(xMeanPos - 2*squareSize,xMeanPos + 2*squareSize,"X");
			hxy[i]->SetAxisRange(-2*squareSize,2*squareSize,"Y");
			hxy[i]->Draw("colz");
		}

		can.Update();
		app.Run(kTRUE);
	
		EventSelectionAndSave(base, xMeanPos+10, xMeanPos+20, 10, 20, -1, true);
		for(int i=0; i<geom.numdet(); i++) {
			can.cd(i+1);
			hxy[i]->Draw("same, box, colz");
		}

		can.Update();
		app.Run(kTRUE);
	}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

	if( batch && FineAlignment){
		ofstream data_out;
		data_out.open("result.dat",ios::app);

		geom.readConfiguration(ocfgfn);

		string str = fn;
		TString ss = str.substr(6, 21);	//"2013-01-27_08:07.root"
		//cout <<"fn = " <<fn <<endl;
		//cout <<"ss = " <<ss <<endl;
		
		ss.ReplaceAll("_"," ");
		ss.ReplaceAll(".root",":00");
		
		TDatime dt;
		dt.Set(ss.Data());
		int time = dt.Convert();

		data_out <<time <<"\t";

		for(int i=0; i<geom.numdet(); i++) {
			int GEM_i = geom.index(i);

			data_out <<geom.x(GEM_i) <<"\t" <<geom_err.x(GEM_i) <<"\t";
			data_out <<geom.y(GEM_i) <<"\t" <<geom_err.y(GEM_i) <<"\t";
			data_out <<geom.z(GEM_i) <<"\t";
			data_out <<geom.phi(GEM_i) <<"\t" <<geom_err.phi(GEM_i) <<"\t";
		}
		
		data_out <<endl;
		data_out.close();
	}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------



//=============================================================================================================================================================



   	delete [] hxy;

	if(FineAlignment){
		delete [] dxdy;
		for (int i = 0; i < geom.numdet(); i++){
			delete []arr_x[i];
			delete []arr_y[i];
			delete []arr_dx[i];
			delete []arr_dy[i];
		}
	}
	
	return 0;
}
