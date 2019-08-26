#include <fstream>
void TimeDistribution() {
	TCanvas* c = new TCanvas("c", "Canvas",1200,1000);
	//std::ifstream infile("lyso0.50.list");
	std::ifstream infile("lyso0.30_single.list");
	TString T;
        Int_t n=15;
	Double_t Mean[n];
	Double_t Mean_Error[n];
	Double_t Sigma[n];
	Double_t Sigma_Error[n];
	Double_t ary[n];
	Double_t arx[n];
	Int_t i = 0;
        Double_t chi2, ndf;
	while (infile >> T ) {
		cout << T << endl;
         	TFile* FileToRead = new TFile(T, "READ");
		FileToRead -> ls();
		TCanvas* CanvTimeDistribution = (TCanvas*)FileToRead -> Get("CanvTimeDistribution");
		//TPad* Pad = (TPad*)CanvTimeDistribution -> GetPad(4);
		//TPad* Pad = (TPad*)CanvTimeDistribution -> GetPad(1);
		TPad* Pad = (TPad*)CanvTimeDistribution -> GetPad(2);
		//TH1F* Time = (TH1F*)Pad -> GetPrimitive("Time");
		//TH1F* Time = (TH1F*)Pad -> GetPrimitive("ht");
		TH1F* Time = (TH1F*)Pad -> GetPrimitive("htsi");
		c->cd();
                gStyle->SetOptStat(1);
                gStyle->SetOptFit(1);
                Time->Rebin(2);
                Time->GetXaxis()->SetRangeUser(-5.,10.);
                Time->GetXaxis()->SetTitle("Time, ns");
	        Time -> Draw("");
		
                TF1* Gaus = new TF1("Gaus", "[0] * exp(-0.5*pow((x-[1])/[2],2))", 0, 3);
    	        Gaus -> SetParLimits(0, 0, 1000);
                Gaus -> SetParLimits(1, 0, 3);
    		Gaus -> SetParLimits(2, 0.001, 1);
    		Gaus -> SetParameter(0, 1);
    		Gaus -> SetParameter(1, Time -> GetMean());
    		Gaus -> SetParameter(2, 0.01);
                // give the parameters meaningful names
                Gaus->SetParNames ("Constant","Mean","Sigma");               
 
		Time-> Fit(Gaus, "W", "S", 0, 3);
		Mean[i] = Gaus -> GetParameter(1);
		Mean_Error[i] = Gaus -> GetParError(1);
		Sigma[i] = Gaus -> GetParameter(2);
		Sigma_Error[i] = Gaus -> GetParError(2);
		arx[i] = i + 1;
		ary[i] = 0;
                chi2 = Gaus->GetChisquare();   
                ndf = Gaus->GetNDF();   
                         
                cout<<"chi2="<<chi2<<"\t"<<"ndf="<<ndf<<endl;                  

		FileToRead -> Close();
		
		ofstream of(T + "_res_" + ".dat",ios_base::out);   
   		of<<Mean[i]<<"\t"<<Mean_Error[i]<<"\t"<<Sigma[i]<<"\t"<<Sigma_Error[i]<<endl;
    		of.close();

		i++;
                system("read");
                c->Update(); 
	}

}
