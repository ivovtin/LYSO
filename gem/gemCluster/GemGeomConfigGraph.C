#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <TCanvas.h>
#include <TApplication.h>
#include <TGraphErrors.h>
#include <TSystem.h>

static TGraphErrors **gr_x;
static TGraphErrors **gr_y;
static TGraphErrors **gr_phi;

static TGraphErrors **gr_x2;
static TGraphErrors **gr_y2;
static TGraphErrors **gr_phi2;

static int NDET = 3;

static int N = 190; //numbres of point

using namespace std;

double nextDouble (string &line, int &index){
	int size=line.length();
	while(index<size && (line[index]==' ' || line[index]=='\t'|| line[index]=='|')){
		index++;
	}
	int start=index;
	while(index<size && line[index]!=' ' && line[index] != '\t' && line[index] != '|'){
		index++;
	}
	string text = line.substr(start,index-start);
	return atof(text.c_str());
}



//=============================================================================================================================================================



void GemGeomConfigGraph (){

	gr_x = new TGraphErrors* [NDET];
	gr_y = new TGraphErrors* [NDET];
	gr_phi = new TGraphErrors* [NDET];

	for(int i=0; i<NDET; i++) {
		gr_x[i] = new TGraphErrors(0);
		gr_y[i] = new TGraphErrors(0);
		gr_phi[i] = new TGraphErrors(0);
		
		gr_x[i]->SetMarkerColor(4);
		gr_x[i]->SetLineColor(4);
		gr_x[i]->SetMarkerStyle(23);
		gr_x[i]->SetMarkerSize(1);

		gr_y[i]->SetMarkerColor(4);
		gr_y[i]->SetLineColor(4);
		gr_y[i]->SetMarkerStyle(23);
		gr_y[i]->SetMarkerSize(1);

		gr_phi[i]->SetMarkerColor(4);
		gr_phi[i]->SetLineColor(4);
		gr_phi[i]->SetMarkerStyle(23);
		gr_phi[i]->SetMarkerSize(1);
	}
	
	gr_x2 = new TGraphErrors* [NDET];
	gr_y2 = new TGraphErrors* [NDET];
	gr_phi2 = new TGraphErrors* [NDET];

	for(int i=0; i<NDET; i++) {
		gr_x2[i] = new TGraphErrors(0);
		gr_y2[i] = new TGraphErrors(0);
		gr_phi2[i] = new TGraphErrors(0);
		
		gr_x2[i]->SetMarkerColor(2);
		gr_x2[i]->SetLineColor(2);
		gr_x2[i]->SetMarkerStyle(22);
		gr_x2[i]->SetMarkerSize(1);

		gr_y2[i]->SetMarkerColor(2);
		gr_y2[i]->SetLineColor(2);
		gr_y2[i]->SetMarkerStyle(22);
		gr_y2[i]->SetMarkerSize(1);

		gr_phi2[i]->SetMarkerColor(2);
		gr_phi2[i]->SetLineColor(2);
		gr_phi2[i]->SetMarkerStyle(22);
		gr_phi2[i]->SetMarkerSize(1);
	}

	can = new TCanvas("c1","GEM alignment",1080,720);
	can->Divide(NDET,3);
   
	ifstream file;
	file.open("result.dat");

    
	string str;
	int time;
			
	double *x = new double[NDET];
	double *sigma_x = new double[NDET];
	double *y = new double[NDET];
	double *sigma_y = new double[NDET];
	int *Z = new int[NDET];
	double *phi = new double[NDET];
	double *sigma_phi = new double[NDET];



	int num_file = 0;
	int num_file2 = 0;
	while (getline (file, str)){
		if (str.substr(0,2) == "//"){
			continue;
		}
		int index = 0;
		
		time = nextDouble(str,index); //cout <<"time - " <<time <<endl;

		for(int i=0; i<NDET; i++){
			x[i] = nextDouble(str,index); //cout <<"\t x[" <<i <<"] = " <<x[i] <<endl;
			sigma_x[i] = nextDouble(str,index); //cout <<"\t sigma_x[" <<i <<"] = " <<sigma_x[i] <<endl;
			y[i] = nextDouble(str,index); //cout <<"\t y[" <<i <<"] = " <<y[i] <<endl;
			sigma_y[i] = nextDouble(str,index); //cout <<"\t sigma_y[" <<i <<"] = " <<sigma_y[i] <<endl;
			Z[i] = nextDouble(str,index); //cout <<"\t Z[" <<i <<"] = " <<Z[i] <<endl;
			phi[i] = nextDouble(str,index); //cout <<"\t phi[" <<i <<"] = " <<phi[i] <<endl;
			sigma_phi[i] = nextDouble(str,index); //cout <<"\t sigma_phi[" <<i <<"] = " <<sigma_phi[i] <<endl;
		}
/*
		cout <<endl;
		for(int i=0; i<NDET; i++){
			cout <<"\t Z[" <<i <<"] = " <<Z[i] <<endl;
		}
*/
		int leftMark = 0;
		int rightMark = NDET - 1;
		while (leftMark < rightMark){
		
			for (int i=leftMark; i<rightMark; i++){
				if (Z[i] > Z[i+1]){
					swap(x[i], x[i+1]);
					swap(sigma_x[i], sigma_x[i+1]);
					swap(y[i], y[i+1]);
					swap(sigma_y[i], sigma_y[i+1]);
					swap(Z[i], Z[i+1]);
					swap(phi[i], phi[i+1]);
					swap(sigma_phi[i], sigma_phi[i+1]);
				}
			}
			rightMark--;
		
			for (int i=rightMark; i>leftMark; i--){
				if (Z[i] < Z[i-1]){
					swap(x[i], x[i-1]);
					swap(sigma_x[i], sigma_x[i-1]);
					swap(y[i], y[i-1]);
					swap(sigma_y[i], sigma_y[i-1]);
					swap(Z[i], Z[i-1]);
					swap(phi[i], phi[i-1]);
					swap(sigma_phi[i], sigma_phi[i-1]);
				}
			}
			leftMark++;
		
		}
/*
		cout <<endl;
		for(int i=0; i<NDET; i++){
			cout <<"\t Z[" <<i <<"] = " <<Z[i] <<endl;
		}
*/
		if(num_file<N){
			for(int i=0; i<NDET; i++){
				gr_x[i]->SetPoint(num_file, time, x[i]);
				gr_x[i]->SetPointError(num_file, 0, sigma_x[i]);

				gr_y[i]->SetPoint(num_file, time, y[i]);	
				gr_y[i]->SetPointError(num_file, 0, sigma_y[i]);

				gr_phi[i]->SetPoint(num_file, time, phi[i]);
				gr_phi[i]->SetPointError(num_file, 0, sigma_phi[i]);
			}
		}
		
		if(num_file>=N){
			for(int i=0; i<NDET; i++){
				gr_x2[i]->SetPoint(num_file, time, x[i]);
				gr_x2[i]->SetPointError(num_file, 0, sigma_x[i]);

				gr_y2[i]->SetPoint(num_file, time, y[i]);	
				gr_y2[i]->SetPointError(num_file, 0, sigma_y[i]);

				gr_phi2[i]->SetPoint(num_file, time, phi[i]);
				gr_phi2[i]->SetPointError(num_file, 0, sigma_phi[i]);
			}
			num_file2++;
		}
		
		
		num_file++;
	}

	file.close();

//=============================================================================================================================================================
	TString title;
	for(int i=0; i<NDET; i++) {

		can->cd(i+1);
		hist=(TH1F*)gr_x[i]->GetHistogram();
		hist->GetXaxis()->SetTimeDisplay(1);
		hist->GetXaxis()->SetTimeOffset(0,"gmt");
		hist->GetXaxis()->SetTimeFormat("#splitline{%d.%m}{%H:%M}");
		hist->GetXaxis()->SetLabelOffset(0.03);
		hist->SetAxisRange(-50,50,"Y");
		title.Form("GEM detector with Z = %i;time;X, mm",Z[i]);
		gr_x[i]->SetTitle(title.Data());
		gr_x[i]->Draw("alp");
		gr_x2[i]->Draw("lp");

		can->cd(i+1+NDET);
		hist=(TH1F*)gr_y[i]->GetHistogram();
		hist->GetXaxis()->SetTimeDisplay(1);
		hist->GetXaxis()->SetTimeOffset(0,"gmt");
		hist->GetXaxis()->SetTimeFormat("#splitline{%d.%m}{%H:%M}");
		hist->GetXaxis()->SetLabelOffset(0.03);
		hist->SetAxisRange(-50,50,"Y");
		title.Form("GEM detector with Z = %i;time;Y, mm",Z[i]);
		gr_y[i]->SetTitle(title.Data());
		gr_y[i]->Draw("alp");
		gr_y2[i]->Draw("lp");
		
		can->cd(i+1+2*NDET);
		hist=(TH1F*)gr_phi[i]->GetHistogram();
		hist->GetXaxis()->SetTimeDisplay(1);
		hist->GetXaxis()->SetTimeOffset(0,"gmt");
		hist->GetXaxis()->SetTimeFormat("#splitline{%d.%m}{%H:%M}");
		hist->GetXaxis()->SetLabelOffset(0.03);
		hist->SetAxisRange(-5,5,"Y");
		title.Form("GEM detector with Z = %i;time;phi, rad",Z[i]);
		gr_phi[i]->SetTitle(title.Data());
		gr_phi[i]->Draw("alp");
		gr_phi2[i]->Draw("lp");
	}
	
	//can->SaveAs("result_graph.root");

   	delete [] gr_x;
   	delete [] gr_y;
   	delete [] gr_phi;
   	
   	delete [] gr_x2;
   	delete [] gr_y2;
   	delete [] gr_phi2;
   	
	delete [] x;
	delete [] sigma_x;
	delete [] y;
	delete [] sigma_y;
	delete [] Z;
	delete [] phi;
	delete [] sigma_phi;
	
}
