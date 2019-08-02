//
//  plotmag_negative.c
//
//  Created by Alexander Barnyakov on 09.05.15.
//
//
#include <Riostream.h>
#include <stdio.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TString.h>
#include <TF1.h>
#include <TApplication.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TFile.h>
#include <TLine.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TMath.h>
#include <TTree.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace std;
string progname;

struct data_t
{
    UInt_t numev;
    UInt_t number;
    Float_t amp[1024];
    UInt_t ti[1024];
    Float_t sum;
};
struct datar_t
{
    UInt_t numev;
    Float_t ped;
    UInt_t min;
    UInt_t tmin;
    UInt_t max;
    UInt_t tmax;
    UInt_t pedmax;
    UInt_t pedmin;
    UInt_t t1;
    UInt_t t2;
};

//void plotmag_negative23(TString filename="data.root", TString fname="datat.root")
//{
TApplication *theApp;

int Usage(string status)
{
        cout<<"Usage: "<<progname<<"\t"<<"test.root test_t.root 0 10000 1 0"<<endl;
        exit(0);
}

int main(int argc, char* argv[])
{
//    TTree* datree;
//    TTree* outree;
    TFile* fd=new TFile(argv[1],"READ");
    TFile* fout=new TFile(argv[2],"RECREATE","timing");
    Int_t thr=atoi(argv[3]); //70; //--- it depends on argument-parameter of procn2.cc ---//
	Int_t total=0;
	Int_t dropt=1;
    if(argc>=4)
    {
        total=atoi(argv[4]);
        if(argc>4)
        {
            dropt=atoi(argv[5]);
		}
    }
    if(argc < 7 || argc > 7 )
    {
      	Usage(progname);
    }
    theApp = new TApplication("app", &argc, argv);
    TTree* outree=new TTree("timedat","MCP magfield timereco");
    Float_t tt1,tt2,ttr1, ttr2, tt1flt, tt2flt;
    
    Float_t xx,yy;
    
    Float_t amptrg, ampch1, ampch1f, tcrtrg, tcrmcp, tcrflt, frtrg, frmcp, frflt, efrtrg, efrmcp, efrflt, chi2trg, chi2mcp, chi2flt, ndftrg, ndfmcp, ndfflt;
    Float_t amptrg2, tcrtrg2, frtrg2, efrtrg2, chi2trg2,ndftrg2;
    Float_t sum, ped, pedmax, pedmin,tot,chled1,trled,trled2,ledfr;
    Float_t sumflt, pedflt, pedmaxflt, pedminflt, totflt, fltled1, ledfrflt;
    // New variable 30/01/2016 //
//    Float_t sum, ped;
    /////////////////////////////	
	int num;
	int n=0;
	Float_t chan;
    outree->Branch("event",&num,"n/i");
    outree->Branch("x",&xx,"x/f");
    outree->Branch("y",&yy,"y/f");
    outree->Branch("trgamp",&amptrg,"trgamp/f");
    outree->Branch("trg2amp",&amptrg2,"trg2amp/f");
    outree->Branch("ch1amp",&ampch1,"ch1amp/f");
    outree->Branch("ch1famp",&ampch1f,"ch1famp/f");
    outree->Branch("trgtcr",&tcrtrg,"trgtcr/f");
    outree->Branch("trg2tcr",&tcrtrg2,"trg2tcr/f");
    outree->Branch("ch1tcr",&tcrmcp,"ch1tcr/f");
    outree->Branch("ch1ftcr",&tcrflt,"ch1ftcr/f");
    outree->Branch("trgfr",&frtrg,"trgfr/f");
    outree->Branch("trg2fr",&frtrg2,"trg2fr/f");
    outree->Branch("ch1fr",&frmcp,"ch1fr/f");
    outree->Branch("ch1ffr",&frflt,"ch1ffr/f");
    outree->Branch("trgefr",&efrtrg,"trgefr/f");
    outree->Branch("trg2efr",&efrtrg2,"trg2efr/f");
    outree->Branch("ch1efr",&efrmcp,"ch1efr/f");
    outree->Branch("ch1fefr",&efrflt,"ch1fefr/f");
    // New branches//
    outree->Branch("ch1sum",&sum,"ch1sum/f");
    outree->Branch("ch1ped",&ped,"ch1ped/f");
    outree->Branch("ch1fsum",&sumflt,"ch1fsum/f");
    outree->Branch("ch1fped",&pedflt,"ch1fped/f");
    /////////////////
    // Newest branches from 14/07/17//
    outree->Branch("ch1tot",&tot,"ch1tot/f");
    outree->Branch("ch1tled",&chled1,"ch1tled/f");
    outree->Branch("ch1ledfr",&ledfr,"ch1ledfr/f");
    outree->Branch("trgtled",&trled,"trgtled/f");
    outree->Branch("trg2tled",&trled2,"trg2tled/f");
    outree->Branch("ch1ftot",&totflt,"ch1ftot/f");
    outree->Branch("ch1ftled",&fltled1,"ch1ftled/f");
    outree->Branch("ch1fledfr",&ledfrflt,"ch1fledfr/f");
    /////////////////
    outree->Branch("trgchi2",&chi2trg,"trgchi2/f");
    outree->Branch("trg2chi2",&chi2trg2,"trg2chi2/f");
    outree->Branch("ch1chi2",&chi2mcp,"ch1chi2/f");
    outree->Branch("ch1fchi2",&chi2flt,"ch1fchi2/f");
    
    outree->Branch("trgndf",&ndftrg,"trgndf/f");
    outree->Branch("trg2ndf",&ndftrg2,"trg2ndf/f");
    outree->Branch("ch1ndf",&ndfmcp,"ch1ndf/f");
    outree->Branch("ch1fndf",&ndfflt,"ch1fndf/f");
    outree->Branch("ch1pedmax",&pedmax,"ch1pedmax/f");
    outree->Branch("ch1pedmin",&pedmin,"ch1pedmin/f");
    outree->Branch("ch1fpedmax",&pedmaxflt,"ch1fpedmax/f");
    outree->Branch("ch1fpedmin",&pedminflt,"ch1fpedmin/f");

    outree->Branch("ch",&chan,"ch/f");
    
//    TString plotfile=par+"plot.ps";
    
//    gStyle->SetOptFit(1);
//    gStyle->SetTextFont(62);
    
    TTree *datree;
    // TTree* datree=new TTree();
    // tree=new TTree();
    if (fd->FindObjectAny("exbeamdata")>0) {
        //        outree=(TChain*)fd->Get("outree");
        datree=(TTree*)fd->Get("exbeamdata");
    }
    else{
        cout << "Wrong name of TTree" << endl;
        exit(1);
    }
    //    outree->Print();
    cout<<"datree found!!!"<<endl;
    data_t ch1, ch1f, trg, trg2;
    datar_t ch1r, ch1fr, trgr, trg2r;
    datree->SetBranchStatus("*",1);
    datree->SetBranchAddress("ch1",&ch1.numev);
    datree->SetBranchAddress("ch1f",&ch1f.numev);
    datree->SetBranchAddress("chtrg1",&trg.numev);
    datree->SetBranchAddress("chtrg2",&trg2.numev);
    datree->SetBranchAddress("ch1r",&ch1r.numev);
    datree->SetBranchAddress("ch1rf",&ch1fr.numev);
    datree->SetBranchAddress("chtrg1r",&trgr.numev);
    datree->SetBranchAddress("chtrg2r",&trg2r.numev);
    datree->SetBranchAddress("x",&xx);
    datree->SetBranchAddress("y",&yy);
    datree->SetBranchAddress("ch",&chan);


    TF1* ftrg=new TF1("edgetrg","x*[1]+[0]",-70,100); //34,64
    TF1* ftrg2=new TF1("edg2trg","x*[1]+[0]",-70,100); //34,64
    TF1* fmcp=new TF1("edgemcp","x*[1]+[0]",-70,100); //,110,170
    TF1* fflt=new TF1("edgeflt","x*[1]+[0]",-70,100); //,110,170
//------------- Drawing ------------------//
//TCanvas *mcan,*mcant;
//if(dropt>0){
    TCanvas* mcan=new TCanvas("mcan","Amp(t)", 0, 0, 800, 600);
    
//}
//------------- END ----------------------//
    TH1F* ht=new TH1F("ht","TTS",2000,-35,35);//80,100
    TH1F* htrg=new TH1F("htrg","TTS",2000,-35,35);//80,100
    TH1F* ha=new TH1F("ha","AMP",4000,0,4000);
//    int n=0;
	if (total<0) total=datree->GetEntries();
    for (int i=0; i<total; i++)
    {
//		cout << "i=" << i << endl;
        tt1=0; tt2=0; ttr1=0; ttr2=0; tt1flt=0; tt2flt=0;
    
        xx=0; yy=0;
    
        amptrg=0; ampch1=0; ampch1f=0; tcrtrg=0; tcrmcp=0; tcrflt=0; frtrg=0; frmcp=0; frflt=0; efrtrg=0; efrmcp=0; efrflt=0; chi2trg=0;
        chi2mcp=0; chi2flt=0; ndftrg=0; ndfmcp=0; ndfflt=0;
        sum=0; ped=0; pedmax=0; pedmin=0; tot=0; chled1=0; trled=0;ledfr=0;
        sumflt=0; pedflt=0; pedmaxflt=0; pedminflt=0; totflt=0; fltled1=0; ledfrflt=0;
        chan=0; num=0;
        amptrg2=0; tcrtrg2=0; frtrg2=0; efrtrg2=0; chi2trg2=0; ndftrg2=0; trled2=0;

        datree->GetEntry(i);
        if (thr>=0)
        {
            ha->Fill(ch1r.ped-ch1r.min); // for negative signals
        }
        else
        {
            ha->Fill(ch1r.max-ch1r.ped);  // for positive signals
        }
//         cout << "ch1r.tmin = " << ch1r.tmin <<endl;
        // New from 30/01/2016 //
        num=ch1.number;
        Int_t n1=ch1r.t1;
        Int_t n2=ch1r.t2;
        if (n1>0 && n2<1000)
        {
            if (thr>=0)
            {
                Float_t a12=ch1r.ped-ch1.amp[n1];
                Float_t a11=ch1r.ped-ch1.amp[n1-1];
                if(a11<a12){	tt1=ch1r.t1-(thr-a11)/(a12-a11);}
                else { tt1=ch1r.t1;}
                Float_t a21=ch1r.ped-ch1.amp[n2-1];
                Float_t a22=ch1r.ped-ch1.amp[n2];
                if(a22<a21){	tt2=ch1r.t2-(thr-a22)/(a21-a22);}
                else {tt2=ch1r.t2;}
            }
            else
            {
                Float_t a12=(-1)*ch1r.ped+ch1.amp[n1];
                Float_t a11=(-1)*ch1r.ped+ch1.amp[n1-1];
                if(a11<a12){	tt1=ch1r.t1-((-1)*thr-a11)/(a12-a11);}
                else { tt1=ch1r.t1;}
                Float_t a21=(-1)*ch1r.ped+ch1.amp[n2-1];
                Float_t a22=(-1)*ch1r.ped+ch1.amp[n2];
                if(a22<a21){	tt2=ch1r.t2-((-1)*thr-a22)/(a21-a22);}
                else {tt2=ch1r.t2;}
            }
        }

        Int_t n1f=ch1fr.t1;
        Int_t n2f=ch1fr.t2;
        if (n1f>0 && n2f<1000)
        {
            if (thr>=0)
            {
                Float_t a12=ch1fr.ped-ch1f.amp[n1];
                Float_t a11=ch1fr.ped-ch1f.amp[n1-1];
                if(a11<a12){	tt1flt=ch1fr.t1-(thr-a11)/(a12-a11);}
                else { tt1flt=ch1r.t1;}
                Float_t a21=ch1fr.ped-ch1f.amp[n2-1];
                Float_t a22=ch1fr.ped-ch1f.amp[n2];
                if(a22<a21){	tt2flt=ch1fr.t2-(thr-a22)/(a21-a22);}
                else {tt2flt=ch1fr.t2;}
            }
            else
            {
                Float_t a12=(-1)*ch1fr.ped+ch1f.amp[n1];
                Float_t a11=(-1)*ch1fr.ped+ch1f.amp[n1-1];
                if(a11<a12){	tt1flt=ch1fr.t1-((-1)*thr-a11)/(a12-a11);}
                else { tt1flt=ch1fr.t1;}
                Float_t a21=(-1)*ch1fr.ped+ch1f.amp[n2-1];
                Float_t a22=(-1)*ch1fr.ped+ch1f.amp[n2];
                if(a22<a21){	tt2flt=ch1fr.t2-((-1)*thr-a22)/(a21-a22);}
                else {tt2flt=ch1fr.t2;}
            }
        }

        Int_t k1=trgr.t1;
        Int_t k2=trgr.t2;
        if (k1>0 && k2<1000)
        {
            if (thr>=0)
            {
                Float_t a12=trgr.ped-trg.amp[n1];
                Float_t a11=trgr.ped-trg.amp[n1-1];
                if(a11<a12){	ttr1=trgr.t1-(thr-a11)/(a12-a11);}
                else { ttr1=trgr.t1;}
            }
            else
            {
                Float_t a12=(-1)*trgr.ped+trg.amp[n1];
                Float_t a11=(-1)*trgr.ped+trg.amp[n1-1];
                if(a11<a12){	ttr1=trgr.t1-((-1)*thr-a11)/(a12-a11);}
                else { ttr1=trgr.t1;}
            }
        }
        Int_t kt1=trg2r.t1;
        Int_t kt2=trg2r.t2;
        if (kt1>0 && kt2<1000)
        {
            if (thr>=0)
            {
                Float_t a12=trgr.ped-trg.amp[n1];
                Float_t a11=trgr.ped-trg.amp[n1-1];
                if(a11<a12){	ttr2=trg2r.t1-(thr-a11)/(a12-a11);}
                else { ttr2=trg2r.t1;}
            }
            else
            {
                Float_t a12=(-1)*trg2r.ped+trg2.amp[n1];
                Float_t a11=(-1)*trg2r.ped+trg2.amp[n1-1];
                if(a11<a12){	ttr2=trg2r.t1-((-1)*thr-a11)/(a12-a11);}
                else { ttr2=trg2r.t1;}
            }

//            Float_t a21=ch1r.ped-ch1.amp[n2-1];
//            Float_t a22=ch1r.ped-ch1.amp[n2];
//            if(a22<a21){	tt2=ch1r.t2-(thr-a22)/(a21-a22);}
//            else {tt2=ch1r.t2;}
        }
        tot=tt2-tt1;
        ledfr=thr/(ch1r.ped-ch1r.min);
        chled1=tt1;
        trled=ttr1;
        trled2=ttr2;
        sum=ch1.sum;
        ped=ch1r.ped;
        pedmax=ch1r.pedmax;
        pedmin=ch1r.pedmin;
//----21/07/2017 -filter ----//
        totflt=tt2flt-tt1flt;
        ledfrflt=thr/(ch1fr.ped-ch1fr.min);
        fltled1=tt1flt;
        sumflt=ch1f.sum;
        pedflt=ch1fr.ped;
        pedmaxflt=ch1fr.pedmax;
        pedminflt=ch1fr.pedmin;
//----21/07/2017 -filter ----//
        /////////////////////////
//	if (ch1r.tmin<730 &&  ch1r.tmin>0 &&  (ch1r.ped-ch1r.min)>0.1                           &&                              (ch1r.ped-ch1r.min)<3000)
//    if (ch1r.tmin<300 &&  ch1r.tmin>0 &&  (ch1r.ped-ch1r.min)>0.5*(pedmax-pedmin) &&  (ch1r.ped-ch1r.min)>1.5 &&  (ch1r.ped-ch1r.min)<3000 /*&& puts("if4")*/)
//	if (ch1r.t1<800 &&  ch1r.t1>100 && ch1r.t2>100 &&ch1r.t2<1020 &&  (ch1r.ped-ch1r.min)>0.5*(pedmax-pedmin) &&  (ch1r.ped-ch1r.min)>1.5 &&  (ch1r.ped-ch1r.min)<3500 /*&& puts("if4")*/)
        //if (ch1r.t1<800 &&  ch1r.t1>100 && ch1r.t2>100 &&ch1r.t2<1020 &&  (ch1r.ped-ch1r.min)>0.5*(pedmax-pedmin) &&  (ch1r.max-ch1r.ped)>1.5 &&  (ch1r.max-ch1r.ped)<3900)
            if (ch1r.t1<800 &&  ch1r.t1>100 && ch1r.t2>100 &&ch1r.t2<1020 &&  (ch1r.ped-ch1r.min)>0.5*(pedmax-pedmin) &&  (ch1r.ped-ch1r.min)>1.5 &&  (ch1r.ped-ch1r.min)<3900 && trg2r.t1<800 &&  trg2r.t1>100 && trg2r.t2>100 &&trg2r.t2<1020 &&  (trg2r.ped-trg2r.min)>0.5*(trg2r.pedmax-trg2r.pedmin) &&  (trg2r.ped-trg2r.min)>50 && (trg2r.ped-trg2r.min)<3900 && (float)sqrt(pow((trgr.tmin-trg2r.tmin),2))/5.<1.5) //for negative signals with TWO TRG signals//
        /*&& puts("if4")*/ // for positive signals
//	if (ch1r.tmin<730 &&  ch1r.tmin>600 && (ch1r.ped-ch1r.min)<3000 && (ch1r.ped-ch1r.min)>0.5 && puts("if1 = true")) // for negative signals SiPM
//       if (ch1r.tmax<950&&ch1r.tmax>600&&(ch1r.max-ch1r.ped)>1.1*(ch1r.pedmax-ch1r.pedmin))   // for positive signals
        {
//			cout << "n=" << n << endl;
            n++;
            if (thr>=0)
            {
                amptrg=trgr.ped-trgr.min; //for negative trg
                amptrg2=trg2r.ped-trg2r.min; //for negative trg
                ampch1=ch1r.ped-ch1r.min; // for negative signals
                ampch1f=ch1fr.ped-ch1fr.min; // for negative signals
            }
            else
            {
                amptrg=trgr.max-trgr.ped; //for positive trg
                amptrg2=trg2r.max-trg2r.ped; //for positive trg
                ampch1=ch1r.max-ch1r.ped; // for positive signals
                ampch1f=ch1fr.max-ch1fr.ped; // for positive signals
            }
            TGraphErrors* grmcp=new TGraphErrors(1024);
            TGraphErrors* grflt=new TGraphErrors(1024);
            TGraphErrors* grtrg=new TGraphErrors(1024);
            TGraphErrors* grtrg2=new TGraphErrors(1024);
            grtrg->SetTitle("");
//        TGraphErrors* grtrg=new TGraphErrors(1024);
            for (int k=0; k<1024; k++)
            {
                grmcp->SetPoint(k,(float)k/5,ch1.amp[k]);
                grmcp->SetPointError(k,0.025,(ch1r.pedmax-ch1r.pedmin)/2);
                grflt->SetPoint(k,(float)k/5,ch1f.amp[k]);
                grflt->SetPointError(k,0.025,(ch1fr.pedmax-ch1fr.pedmin)/2);
                grtrg->SetPoint(k,(float)k/5,trg.amp[k]);
                grtrg->SetPointError(k,0.025,(trgr.pedmax-trgr.pedmin)/2);
                grtrg2->SetPoint(k,(float)k/5,trg2.amp[k]);
                grtrg2->SetPointError(k,0.025,(trg2r.pedmax-trg2r.pedmin)/2);
            }
            ftrg->SetParameter(0,(trgr.ped*(-1)));  // for negative TRG signals
            ftrg->SetParameter(1,(trgr.ped-trgr.min)/0.9*(-1)); // for negative TRG signals
            ftrg2->SetParameter(0,(trg2r.ped*(-1))); // for negative TRG signals
            ftrg2->SetParameter(1,(trg2r.ped-trg2r.min)/0.9*(-1)); // for negative TRG signals

            fmcp->SetParameter(0,(2*ch1r.ped));
            fflt->SetParameter(0,(2*ch1fr.ped));
            if (thr>=0)
            {
                fmcp->SetParameter(1,(ch1r.ped-ch1r.min)/(-1.1)); //for negative signals
                fflt->SetParameter(1,(ch1fr.ped-ch1fr.min)/(-1.1)); //for negative signals
            }
            else
            {
                fmcp->SetParameter(1,(ch1r.max-ch1r.ped)/6.1); // for positive signals
                fflt->SetParameter(1,(ch1fr.max-ch1fr.ped)/6.1); // for positive signals
            }
            grtrg->SetMaximum(3900);
            grtrg->SetMinimum(50);
//------------- Drawing ------------------//
            if(dropt>0){
                grtrg->Draw("AWLP");
                //mcan->RangeAxis(50,1500,100,3000);
                grmcp->Draw("LP");
                grtrg2->Draw("LP");
            }
//------------- Drawing ------------------//
           
            float thrscale=0.5, thrtrg, thrtrg2, thrmcp, thrflt;
            if (thr>=0)
            {
                thrtrg=trgr.ped-amptrg*thrscale; //for negative trg
                thrtrg2=trg2r.ped-amptrg2*thrscale; //for negative trg
                thrmcp=ch1r.ped-ampch1*thrscale; // for negative signals
                thrflt=ch1fr.ped-ampch1f*thrscale; // for negative signals
            }
            else
            {
                thrtrg=trgr.ped+amptrg*thrscale; //for positive trg
                thrtrg2=trg2r.ped+amptrg2*thrscale; //for positive trg
                thrmcp=ch1r.ped+ampch1*thrscale; // for positive signals
                thrflt=ch1fr.ped+ampch1f*thrscale; // for positive signals
            }
//	        gStyle->SetStatY(1.002);
//            gStyle->SetStatX(0.4);
//            cout<<"TRG.Tmax="<<trgr.tmin<<endl;
            if (thr>=0)
            {
                grtrg->Fit(ftrg,"E,Q0","",(float)(trgr.tmin-5)/5,(float)(trgr.tmin-1.)/5); //For fast negative TRG from MCP PMT
              //  cout<<"TRG1 is fitted"<<endl;
                grtrg2->Fit(ftrg2,"E,Q0","",((float)(trg2r.tmin)-4)/5,((float)(trg2r.tmin)-0.5)/5); //For fast negative TRG from MCP PMT
              //  cout<<"TRG2 is fitted"<<endl;
            }
            else
            {
                grtrg->Fit(ftrg,"E,Q0","",(trgr.tmax-17)/5,(trgr.tmax-4)/5); //For positive logical TRG from PiLas sync. signal
                grtrg2->Fit(ftrg2,"E,Q0","",(trg2r.tmax-17)/5,(trg2r.tmax-5)/5); //For positive logical TRG from PiLas sync. signal
            }
            if(dropt>0){
                ftrg->Draw("same");
                ftrg2->SetLineColor(3);
                ftrg2->Draw("same");
//            cout<<"After TRG fit"<<endl;
                TLine* ltrg=new TLine(10,thrtrg,54,thrtrg);
                TLine* ltrg2=new TLine(10,thrtrg2,54,thrtrg2);
                ltrg->SetLineColor(6);
                ltrg2->SetLineColor(3);
//------------- Drawing ------------------//
                ltrg->Draw("same");
                ltrg2->Draw("same");
                mcan->Update();
            }
//------------- Drawing ------------------//
            frtrg=ftrg->GetParameter(1);
            efrtrg=ftrg->GetParError(1);
            chi2trg=ftrg->GetChisquare();
            ndftrg=ftrg->GetNDF();
//------------- TRG2 params -------------//
            frtrg2=ftrg2->GetParameter(1);
            efrtrg2=ftrg2->GetParError(1);
            chi2trg2=ftrg2->GetChisquare();
            ndftrg2=ftrg2->GetNDF();
//--------------------------------------//
            //cout<<"ftrg1,2 params have got"<<endl;
            tcrtrg=(thrtrg-ftrg->GetParameter(0))/frtrg;
            tcrtrg2=(thrtrg2-ftrg2->GetParameter(0))/frtrg2;
//            gStyle->SetStatX(1.0);
            if (thr>=0)
            {
	        grmcp->Fit(fmcp,"E,Q0","",(float)(ch1r.tmin-5)/5,(float)(ch1r.tmin-1.)/5); // for negative signals
            grflt->Fit(fflt,"E,Q0","",(float)(ch1fr.tmin-5)/5,(float)(ch1fr.tmin-1.)/5); // for negative signals
            }
            else
            {
//            cout<<"After MCP fit"<<endl;
		Float_t det=ch1r.tmax-ch1r.t1;
//		Float_t det=ch1.ti[ch1r.t1+1]-ch1.ti[ch1r.t1-1];
		Float_t ampl=ch1r.max-ch1r.ped+thr;
		Float_t thrampl=ampl/2+ch1r.ped;
//		Float_t ampl=ch1r.max-ch1r.ped;
		Float_t da=(float)ch1r.max/2.-ch1r.ped/2.+thr;
//		Float_t da=ch1.amp[ch1r.t1+1]-ch1.amp[ch1r.t1-1];
		Float_t tin=ch1.ti[ch1r.t1+1]-((ch1.amp[ch1r.t1+1]-ch1r.pedmax)/da)*det+1;
		Float_t tfi=ch1.ti[ch1r.t1-1]+((ampl*0.6-ch1.amp[ch1r.t1-1])/da)*det-1;
		Float_t t1_2; //=ch1r.t1+(da/ampl)*(det);
		for (int i=0; i<60; i++) {
			if ( ch1.amp[ch1r.tmax-i]<thrampl ) {
				t1_2=ch1r.tmax-i;
				break;
			}
		}
		//cout<<"ampl:"<<ampl<<" da:"<<da<<" det:"<<det<<" t(1/2):"<<t1_2<<endl;
                grmcp->Fit(fmcp,"E,Q0","",(t1_2-4.)/5,(t1_2+6.2)/5); // for positive signals from SiPM
//                grmcp->Fit(fmcp,"E,Q0","",(tin)/5,(tfi-0)/5); // for positive signals from SiPM
                grflt->Fit(fflt,"E,Q0","",(float)(ch1fr.tmax-19.2)/5,(float)(ch1fr.tmax-9.)/5); // for positive signals from SiPM
            }
            if(dropt>0){
                TLine* lmcp=new TLine(0,thrmcp,136,thrmcp);
                lmcp->SetLineColor(4);
//------------- Drawing ------------------//
                fmcp->SetLineColor(4);
                fmcp->Draw("same");
                lmcp->Draw("same");
                mcan->Update();
            }
//------------- END ----------------------//
            frmcp=fmcp->GetParameter(1);
            efrmcp=fmcp->GetParError(1);
            chi2mcp=fmcp->GetChisquare();
            ndfmcp=fmcp->GetNDF();
            //float
            tcrmcp=(thrmcp-fmcp->GetParameter(0))/frmcp;
// ----------21/07/2017 ------//
            frflt=fflt->GetParameter(1);
            efrflt=fflt->GetParError(1);
            chi2flt=fflt->GetChisquare();
            ndfflt=fflt->GetNDF();
            //float
            tcrflt=(thrflt-fflt->GetParameter(0))/frflt;
//----------------------------//
//            getchar();
            printf("%d\tTcr(trg1)=%6.3f \t Tcr(trg2)=%6.3f \t Tcr(mcp)=%6.3f \t\t Dt=%6.3f\r",n,tcrtrg,tcrtrg2,tcrmcp,tcrmcp-tcrtrg); fflush(stdout);
//            cout<<n<<"\tTcr(trg)="<<tcrtrg<<"\tTcr(mcp)="<<tcrmcp<<"\tDt="<<tcrmcp-tcrtrg<<endl;
            ht->Fill(tcrmcp-tcrtrg);
            htrg->Fill(tcrtrg2-tcrtrg);
            grtrg->Delete();
            grtrg2->Delete();
            grmcp->Delete();
            grflt->Delete();
        }
        outree->Fill();
        //cout<<"Tree filled"<<endl;
    }
    cout<<"For loop is finished"<<endl;
    fd->Close();
    cout<<"FD is closed"<<endl;
//    if(dropt>0)
//    {
//        TCanvas* mcant=new TCanvas("mcant","TTS", 400, 0, 1200, 600);
//        ht->Draw();
//    }
//    mcant->cd();
//    mcant->Update();
   // fout->cd();
    outree->Write();
    cout<<"OutTree is written"<<endl;
    ht->Write();
    htrg->Write();
    ha->Write();
//    mcant->Write();
  //  mcan->Write();
    fout->cd();
    fout->Close();
    cout<<"Fout is closed"<<endl;
    if(dropt>0){
        theApp->Run();
    }
    cout<<"After theApp->Run()"<<endl;

}

