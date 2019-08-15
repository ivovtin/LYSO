//function form fit
TF1* Init_Aprox(Int_t num, const char* name) {
	TF1* f = new TF1(name ,"[0] + [1] * x", 0, 200);
     	f -> SetParName(0, "Const");
     	f -> SetParName(1, "Angle");
     	f -> SetParLimits(1, num, 0);
	f -> SetTitle(name);
        f -> SetLineColor(kRed);
	return f;
}

void plotxy_new(TString name, Int_t nev, Int_t nev1, Int_t nev2, Int_t y1, Int_t y2) {
    
    gStyle->SetOptStat(1);
    gStyle->SetOptFit(1);
    gROOT->SetStyle("Plain");
    gStyle -> SetPalette(1);

    TString outDir="/home/ovtin/development/LYSO/out/";

    TChain *exbeamdata=new TChain("exbeamdata");
    TChain *daq=new TChain("daq");
    TChain *gem=new TChain("gem");
    TChain *sipm=new TChain("exbeamdata");
   
    if(name=="lyso30minus")
    { 
     //data from MCP
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_18-53-23/mcplyso2019-06-07_18-53.root");
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-00-52/mcplyso2019-06-07_19-00.root");
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-05-12/mcplyso2019-06-07_19-05.root");
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-10-16/mcplyso2019-06-07_19-10.root");
 
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_18-53-23/2019-06-07_18-53.root");
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-00-52/2019-06-07_19-00.root");
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-05-12/2019-06-07_19-05.root");
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-10-16/2019-06-07_19-10.root");
 
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_18-53-23/gem_2019-06-07_18-53.root");
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-00-52/gem_2019-06-07_19-00.root");
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-05-12/gem_2019-06-07_19-05.root");
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-10-16/gem_2019-06-07_19-10.root");

     //data from SIPM
     sipm->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_18-53-23/sipm2019-06-07_18-53.root");
     sipm->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-00-52/sipm2019-06-07_19-00.root");
     sipm->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-05-12/sipm2019-06-07_19-05.root");
     sipm->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-10-16/sipm2019-06-07_19-10.root");
    }

    if(name=="lyso30plus")
    { 
     //data from MCP
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-24-54/mcplyso2019-06-07_19-24.root");
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-32-34/mcplyso2019-06-07_19-32.root");
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-38-34/mcplyso2019-06-07_19-38.root");
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-43-43/mcplyso2019-06-07_19-43.root");
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-48-51/mcplyso2019-06-07_19-48.root");
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-53-41/mcplyso2019-06-07_19-53.root");
 
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-24-54/2019-06-07_19-24.root");
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-32-34/2019-06-07_19-32.root");
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-38-34/2019-06-07_19-38.root");
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-43-43/2019-06-07_19-43.root");
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-48-51/2019-06-07_19-48.root");
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-53-41/2019-06-07_19-53.root");
 
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-24-54/gem_2019-06-07_19-24.root");
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-32-34/gem_2019-06-07_19-32.root");
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-38-34/gem_2019-06-07_19-38.root");
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-43-43/gem_2019-06-07_19-43.root");
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-48-51/gem_2019-06-07_19-48.root");
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-53-41/gem_2019-06-07_19-53.root");

     //data from SIPM
     sipm->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-24-54/sipm2019-06-07_19-24.root");
     sipm->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-32-34/sipm2019-06-07_19-32.root");
     sipm->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-38-34/sipm2019-06-07_19-38.root");
     sipm->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-43-43/sipm2019-06-07_19-43.root");
     sipm->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-48-51/sipm2019-06-07_19-48.root");
     sipm->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-53-41/sipm2019-06-07_19-53.root");
    }
 
     if(name=="lysozero")
    { 
     //data from MCP
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-05-00/mcpquartz2019-06-07_20-05.root");
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-11-25/mcplyso2019-06-07_20-11.root");
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-16-47/mcplyso2019-06-07_20-16.root");
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-21-30/mcplyso2019-06-07_20-21.root");
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-25-13/mcplyso2019-06-07_20-25.root");
     
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-05-00/2019-06-07_20-05.root");
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-11-25/2019-06-07_20-11.root");
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-16-47/2019-06-07_20-16.root");
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-21-30/2019-06-07_20-21.root");
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-25-13/2019-06-07_20-25.root");
     
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-05-00/gem_2019-06-07_20-05.root");
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-11-25/gem_2019-06-07_20-11.root");
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-16-47/gem_2019-06-07_20-16.root");
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-21-30/gem_2019-06-07_20-21.root");
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-25-13/gem_2019-06-07_20-25.root");
     

     //data from SIPM
     sipm->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-05-00/sipm2019-06-07_20-05.root");
     sipm->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-11-25/sipm2019-06-07_20-11.root");
     sipm->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-16-47/sipm2019-06-07_20-16.root");
     sipm->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-21-30/sipm2019-06-07_20-21.root");
     sipm->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-25-13/sipm2019-06-07_20-25.root");
    }

     if(name=="test")
    {
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-05-00/mcpquartz2019-06-07_20-05.root");
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-05-00/2019-06-07_20-05.root");
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-05-00/gem_2019-06-07_20-05.root");
     sipm->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_20-05-00/sipm2019-06-07_20-05.root");
    }
 
    exbeamdata -> Print();    
    //Friend tree exbeamdata from MCP with gem, daq, sipm
    exbeamdata -> AddFriend("gem");
    exbeamdata -> AddFriend("daq");
    exbeamdata -> AddFriend(sipm,"sipm");
 
    //boundaries along y-coordinate of LYSO bar - used GEM for selection this events
    TString y_boundLeft;
    y_boundLeft.Form("%d", y1);
    TString y_boundRight;
    y_boundRight.Form("%d", y2);

    //create Canvas
    TCanvas* CanvGeom = new TCanvas("CanvGeom", "Geometry(" + y_boundLeft + " - " + y_boundRight + ")", 1500, 600, 600, 600);
    CanvGeom -> Divide(2, 4);
    TCanvas* CanvAmp = new TCanvas("CanvAmp", "Amplitude distributions(" + y_boundLeft + " - " + y_boundRight + ")", 1000, 600, 600, 600);
    CanvAmp -> Divide(2, 3);
    TCanvas* CanvTimeDistribution = new TCanvas("CanvTimeDistribution", "Time distribution(" + y_boundLeft + " - " + y_boundRight + ")", 500, 600, 600, 600);
    CanvTimeDistribution -> Divide(2, 2);

    //create histograms for output
    TH2F* xy1 = new TH2F("xy1", "GEM1 hits", 140, 0, 140, 40, 0, 40);
    TH2F* xy2 = new TH2F("xy2", "GEM2 hits", 140, 0, 140, 40, 0, 40);
    TH2F* xys = new TH2F("xys", "GEM hits with SiPM", 140, 0, 140, 40, 0, 40); //Need??//
    TH2F* xyev = new TH2F("xyev", "GEM hits per Event", 140, 0, 140, 40, 0, 40);
    TH1F* htrg1 = new TH1F("htrg1", "MCP PMT spectra", 4096, 0, 4096);
    TH1F* htrg2 = new TH1F("htrg2", "MCP PMT spectra", 4096, 0, 4096);
    TH1F* ht = new TH1F("ht","#Delta t for MCP", 2001, -10.005, 10.005);
    TH1F* htsi = new TH1F("htsi","#Delta t for SiPM", 2001, -10.005, 10.005);
    TH1F* Time = new TH1F("Time", "Time of registration (MCP-SiPM)", 2001, -10.005, 10.005);
    //TH1F* Time = new TH1F("Time", "Time of registration (MCP-SiPM)", 500, -10.005, 10.005);
    TH1F* TimeDistributionTriggers  =  new TH1F("TimeDistributionTriggers", "Time distribution between triggers MCP", 2001, -10.005, 10.005);
    TH2F* TimeVSx = new TH2F("TimeVSx", "", 100, 0, 100, 100, 0, 10);
    TH2F* TimeVSy = new TH2F("TimeVSy", "", 100, 0, 100, 100, 0, 10);
    TH2F* TimeVSAmplitude = new TH2F("TimeVSAmplitude", "Time VS Amplitude", 10000, 1700, 2400, 20, 3, 4);
    TH2F* xyf = new TH2F("xyf", "pixels", 40, 40, 80, 40, 0, 40);

    //======================
   //selection conditions for trgMCP and DAQ
    TString Conditions_1 = "(exbeamdata.chtrg2r.ped - exbeamdata.chtrg2r.min) > 300 && (exbeamdata.chtrg2r.ped - exbeamdata.chtrg2r.min) < 900 && (exbeamdata.chtrg1r.ped - exbeamdata.chtrg1r.min) > 300 && (exbeamdata.chtrg1r.ped - exbeamdata.chtrg1r.min) < 900 && daq.lecroy2249.ch0 > 800 && daq.lecroy2249.ch0 < 1600";
    //selection conditions for GEM
    TString Conditions_coor1 = "detClusters.x[2] > 36 &&  detClusters.x[2] < 48 && detClusters.y[2] >" + y_boundLeft + " &&  detClusters.y[2] <" + y_boundRight;
    TString Conditions_coor2 = "detClusters.x[3] > 65 &&  detClusters.x[3] < 74 && detClusters.y[3] >" + y_boundLeft + " &&  detClusters.y[3] <" + y_boundRight;
    //TString Conditions_all = Conditions_1 + " && " + Conditions_coor1 + "&&" + Conditions_coor2;
    TString Conditions_all = Conditions_1 + " && " + Conditions_coor1;

    CanvAmp -> cd(1);
    //Amplitude of min in trigger per event
    exbeamdata -> Draw("exbeamdata.chtrg1r.ped - exbeamdata.chtrg1r.min >> htrg1");
    exbeamdata -> Draw("exbeamdata.chtrg2r.ped - exbeamdata.chtrg2r.min >> htrg2"); 
    htrg1 -> SetLineColor(2);
    htrg2 -> SetLineColor(4);
    htrg1 -> Draw();
    htrg2 -> Draw("same");
    TLine* mcpb1 = new TLine(300, 0, 300, 100);
    TLine* mcpb2 = new TLine(900, 0, 900, 100);
    mcpb1 -> Draw("same");
    mcpb2 -> Draw("same");
    CanvAmp -> Update();

    //Map of hits GEM
    CanvGeom -> cd(1);
    exbeamdata -> Draw("detClusters.y[2]:detClusters.x[2] >> xy1", Conditions_1,"");
    xy1 -> Draw("colz");  
    CanvGeom -> cd(2);
    exbeamdata -> Draw("detClusters.y[3]:detClusters.x[3] >> xy2", Conditions_1,"");
    xy2 -> Draw("colz");
    CanvGeom -> Update();

    TProfile* amp = new TProfile("amp", "a(t) for MCP ", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* ampf = new TProfile("ampf", "a(t)f for MCP", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* ampavg = new TProfile("ampavg", "Average a(t) for MCP", 1024, 1. / 1024, 0.2 * 1024);

    TProfile* ampsi = new TProfile("ampsi", "a(t) for SiPM", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* ampfsi = new TProfile("ampfsi", "a(t)f for SiPM", 1024, 1. / 1024, 0.2 * 1024);

    TProfile* amptrg1 = new TProfile("amptrg1", "a1(t) for trgmcp1", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* amptrg2 = new TProfile("amptrg2", "a2(t) for trgmcp2", 1024, 1. / 1024, 0.2 * 1024);

    TProfile2D* dtvsxy = new TProfile2D("dtvsxy", "#Delta T vs XY", 140, 0, 140, 40, 0, 40);

    //Aproximations for signal
    TF1* fun = Init_Aprox(200000, "fun");             //mcp
    TF1* funsi = Init_Aprox(200000, "funsi");         //sipm
    TF1* funtrg = Init_Aprox(-200000, "funtrg");      //trgmcp1
    TF1* funtrg2 = Init_Aprox(-200000, "funtrg2");    //trgmcp2

    Int_t nentries = 0;
    if (exbeamdata -> GetEntries() < nev) nev = exbeamdata -> GetEntries();
    cout << nev << endl;
    //MAIN LOOP
    //for (int i = 0; i < 1; i++){
    for (int i = 0; i < nev; i++){
    	if (!(i > nev1 && i < nev2)) {
        	exbeamdata -> Draw("(exbeamdata.ch1r.ped - exbeamdata.ch1.amp):exbeamdata.ch1.ti / 5. >> ampf", Conditions_all , "goff", 1, i);
 		ampf -> SetAxisRange(2, 20, "X");     //pedestal from first 20 ns
        	Float_t pedmax = ampf -> GetMaximum();
        	Float_t pedmin = ampf -> GetMinimum();
        	ampf -> SetAxisRange(2, 200, "X");    //range with signal
        	Float_t MaximumSignalBin = ampf -> GetMaximumBin();
     	        Float_t MaximumSignal = ampf -> GetMaximum();
		//cout<<"MaximumSignalBin="<<MaximumSignalBin<<"\t"<<"MaximumSignal="<<MaximumSignal<<"\t"<<"Event="<<i<<endl;
        	Float_t FirstSignalBin = ampf -> FindFirstBinAbove(pedmax + MaximumSignal * 0.2, 1); //20% from Maximum signal plus maximum of pedestal
		Float_t HalfHeightBinLeft = ampf -> FindFirstBinAbove(MaximumSignal / 2.0, 1);  //50% from from Maximum signal
		Float_t HalfHeightBinRight = ampf -> FindLastBinAbove(MaximumSignal / 2.0, 1); 
		
		exbeamdata -> Draw("(sipm.ch1.amp - sipm.ch1r.ped):sipm.ch1.ti / 5. >> ampfsi", Conditions_all , "goff", 1, i);
 		ampfsi -> SetAxisRange(2, 20, "X");
        	Float_t pedmax_Si = ampfsi -> GetMaximum();
        	Float_t pedmin_Si = ampfsi -> GetMinimum();
       		ampfsi -> SetAxisRange(2, 200, "X");
        	Float_t MaximumSignalBin_Si = ampfsi -> GetMaximumBin();
	      	Float_t MaximumSignal_Si = ampfsi -> GetMaximum();
		Float_t FirstSignalBin_Si = ampfsi -> FindFirstBinAbove(pedmax_Si + MaximumSignal_Si * 0.1, 1);
		Float_t HalfHeightBinLeft_Si = ampfsi -> FindFirstBinAbove(MaximumSignal_Si / 2.0, 1);
		Float_t HalfHeightBinRight_Si = ampfsi -> FindLastBinAbove(MaximumSignal_Si / 2.0, 1);
                        //conditions from MCP waveform 
			if (FirstSignalBin - MaximumSignalBin < 0 && MaximumSignalBin > 100 && MaximumSignal > 2 * (pedmax - pedmin) && (HalfHeightBinRight - HalfHeightBinLeft) > 10 && MaximumSignalBin < 350){
	    		 exbeamdata -> Draw("exbeamdata.ch1.amp:exbeamdata.ch1.ti / 5. >> amp", Conditions_all, "goff", 1, i);
	    		 exbeamdata -> Draw("sipm.ch1.amp:sipm.ch1.ti / 5. >> ampsi", Conditions_all, "goff", 1, i);
           		 exbeamdata -> Draw("exbeamdata.chtrg1.amp:exbeamdata.chtrg1.ti / 5. >> amptrg1", Conditions_all, "goff", 1, i);
            		 exbeamdata -> Draw("exbeamdata.chtrg2.amp:exbeamdata.chtrg2.ti / 5. >> amptrg2", Conditions_all, "goff", 1, i);
                         
                         //mcp
            		 amp -> SetAxisRange(30, 45, "X");
            		 Float_t ped = amp -> GetMean(2);
            		 amp -> SetAxisRange(30, 200, "X");
            		 Float_t min = amp -> GetMinimum();
                         //sipm
			 ampsi -> SetAxisRange(30, 45, "X");
            		 Float_t ped_Si = ampsi -> GetMean(2);
            		 ampsi -> SetAxisRange(30, 200, "X");
            		 Float_t min_Si = ampsi -> GetMinimum();
                         //trgmcp1
	   		 amptrg1 -> SetAxisRange(30, 45, "X");
            		 Float_t pedtrg1 = amptrg1 -> GetMean(2);
            		 amptrg1 -> SetAxisRange(30, 200, "X");
            		 Int_t minbintrg = amptrg1 -> GetMinimumBin();
            		 Float_t mintrg = amptrg1 -> GetMinimum();
                         //trgmcp2
   		         amptrg2 -> SetAxisRange(30, 45, "X");
	            	 Float_t pedtrg2 = amptrg2 -> GetMean(2);
            		 amptrg2 -> SetAxisRange(30, 200, "X");
            		 Float_t minbintrg2 = amptrg2 -> GetMinimumBin();
            		 Float_t mintrg2 = amptrg2 -> GetMinimum();
                         //gem
            		 CanvGeom -> cd(3);
            		 exbeamdata -> Draw("detClusters.y[2]:detClusters.x[2] >> +xys", Conditions_all, "goff", 1, i);
            		 exbeamdata -> Draw("detClusters.y[2]:detClusters.x[2] >> xyev", Conditions_all, "goff", 1, i);
            		 exbeamdata -> Draw("detClusters.y[2]:detClusters.x[2] >> +xyf", Conditions_all, "goff", 1, i);

                         //the time calculation for trgmcp1 and trgmcp2	
            		 funtrg -> SetParameter(0, pedtrg1);
            		 funtrg -> SetParameter(1, -2000);
 	    		 funtrg2 -> SetParameter(0, pedtrg2);
            		 funtrg2 -> SetParameter(1, -2000);

			 Int_t Bin2trg1=155.;
                         for(Int_t Bin1trg1=150.; Bin1trg1<1024.; Bin1trg1++)
                         { 
                           Bin2trg1++;
			   if(  (amptrg1 -> GetBinContent(Bin2trg1) / amptrg1 -> GetBinContent(Bin1trg1) ) <= 0.9890 )
			   {
     			     break;
			   } 
			 }
                         Int_t Bin3trg1=Bin2trg1;
                         for(Bin3trg1; Bin3trg1<1024.; Bin3trg1++)
			 {  
                           Bin3trg1++;
                            if( amptrg1->GetBinContent(Bin3trg1) > amptrg1->GetBinContent(Bin3trg1-2.) )
                           {
                             break;
                           }  			    
			 }
                         Int_t FitBin1trg1=Bin2trg1;
                         Int_t FitBin2trg1=Bin3trg1-4.;         
            		 amptrg1 -> Fit(funtrg, "W", "S", FitBin1trg1/5., FitBin2trg1/5.);     //fit in range  

			 Int_t Bin2trg2=155.;
                         for(Int_t Bin1trg2=150.; Bin1trg2<1024.; Bin1trg2++)
                         { 
                           Bin2trg2++;
			   if(  (amptrg2 -> GetBinContent(Bin2trg2) / amptrg2 -> GetBinContent(Bin1trg2) ) <= 0.9890 )
			   {
     			     break;
			   } 
			 }
                         Int_t Bin3trg2=Bin2trg2;
                         for(Bin3trg2; Bin3trg2<1024.; Bin3trg2++)
			 {  
                           Bin3trg2++;
                            if( amptrg2->GetBinContent(Bin3trg2) > amptrg2->GetBinContent(Bin3trg2-2.) )
                           {
                             break;
                           }  			    
			 }
                         Int_t FitBin1trg2=Bin2trg2;
                         Int_t FitBin2trg2=Bin3trg2-4.; 
            		 amptrg2 -> Fit(funtrg2, "W", "S", FitBin1trg2/5., FitBin2trg2/5.);     //fit in range  
                         //we find the solution to the intersection of lines: y=[0]+[1]*x and y=const
            		 Float_t tcrtrg1 = (-0.35 * (pedtrg1 - mintrg) + pedtrg1 - funtrg -> GetParameter(0)) / (funtrg -> GetParameter(1));
	    		 Float_t tcrtrg2 = (-0.35 * (pedtrg2 - mintrg2) + pedtrg2 - funtrg2 -> GetParameter(0)) / (funtrg2 -> GetParameter(1));
            		 cout<<"tcrtrg1="<<tcrtrg1<<"\t"<<"Event="<<i<<endl;
            		 cout<<"tcrtrg2="<<tcrtrg2<<"\t"<<"Event="<<i<<endl;

 	                 //time of MCP	
			 fun -> SetParameter(0, ped);
            		 fun -> SetParameter(1, -200);
                         //
			 Int_t Bin2mcp=158.;
                         for(Int_t Bin1mcp=150.; Bin1mcp<1024.; Bin1mcp++)
                         { 
                           Bin2mcp++;
			   if(  (amp -> GetBinContent(Bin2mcp) / amp -> GetBinContent(Bin1mcp) ) <= 0.9890 )
			   {
     			     break;
			   } 
			 }
                         Int_t Bin3mcp=Bin2mcp;
                         for(Bin3mcp; Bin3mcp<1024.; Bin3mcp++)
			 {  
                           Bin3mcp++;
                            if( amp->GetBinContent(Bin3mcp) > amp->GetBinContent(Bin3mcp-1) )
                           {
                             break;
                           }  			    
			 }
                         Int_t FitBin1mcp=Bin2mcp;
                         Int_t FitBin2mcp=Bin3mcp-5.0; 
            		 amp -> Fit(fun, "W", "S", FitBin1mcp / 5., FitBin2mcp / 5.);
                         cout<<"amp -> GetBinContent(Bin1mcp)="<<amp -> GetBinContent(Bin2mcp-8)<<"\t"<<"amp -> GetBinContent(Bin2mcp)="<<amp -> GetBinContent(Bin2mcp)<<endl; 	
           		 cout<<"FitBin1mcp="<<FitBin1mcp/5.<<"\t"<<"FitBin2mcp="<<FitBin2mcp/5.<<endl;
                           
                         Float_t tcr = (-0.35 * (ped - min) + ped - fun -> GetParameter(0)) / (fun -> GetParameter(1));
            		 cout<<"tcr="<<tcr<<"\t"<<"Event="<<i<<endl;
 
           		 //time of SIPM 		 
			 CanvAmp -> cd(5);
			 funsi -> SetParameter(0, pedtrg1);
            		 funsi -> SetParameter(1, -2000);
                         
	                 Int_t FitBin1sipm=0;
                         Int_t j4=0;
          	         Int_t maxbinsipm = ampsi -> GetMaximumBin();
                         Float_t binContent = ampsi -> GetBinContent(maxbinsipm);
		         while ( (ped_Si + 4) < binContent )
			 {
			  j4++;
                          FitBin1sipm = maxbinsipm-j4;
                          binContent = ampsi -> GetBinContent(FitBin1sipm);                                                  
                         }
                         Int_t Bin3sipm=FitBin1sipm;
                         for(Bin3sipm; Bin3sipm<350.; Bin3sipm++)
			 {  
                           Bin3sipm++;
                           //cout<<"Bin3sipm="<<Bin3sipm/5.<<"\t"<<"ampsi->GetBinContent(Bin3sipm)/ampsi->GetBinContent(Bin3sipm-1)="<<ampsi->GetBinContent(Bin3sipm)/ampsi->GetBinContent(Bin3sipm-1)<<endl;
                           if( ampsi->GetBinContent(Bin3sipm)/ampsi->GetBinContent(Bin3sipm-1) < 1.00110 )
                           {
                            break;
                           }  			    
			 }
                         Int_t FitBin2sipm=Bin3sipm-7.0; 
        		 ampsi -> Fit(funsi, "W", "S", (FitBin1sipm - 0.5) / 5., FitBin2sipm/5.);
			 Float_t tcr_Si = (-0.35 * (ped_Si - ampsi -> GetBinContent(FitBin2sipm)) + ped_Si - funsi -> GetParameter(0)) / (funsi-> GetParameter(1));
            		 cout<<"tcr_Si="<<tcr_Si<<"\t"<<"Event="<<i<<endl;

                         //======================

            	 	 if ( (xyf -> GetEntries() - nentries) == 1 ) nentries = xyf -> GetEntries();
            		 for (int i = 0; i < amp -> GetNbinsX(); i++) {
                		 ampavg -> Fill(amp -> GetBinCenter(i) - tcrtrg1 + 60, amp -> GetBinContent(i));
            		 }
            		 
                         //======================
                         //time between MCP and first Trg MCP
            		 ht -> Fill(tcr - tcrtrg2);          		 
                         //time between SIPM and first Trg MCP
			 htsi -> Fill(tcr_Si - tcrtrg2);

            	         //cout << "dT=" << (tcr - tcrtrg1) << endl;
            		 if ((tcr - tcrtrg1) < 15 && (tcr - tcrtrg1) > -2) {
                	 	dtvsxy -> Fill(xyev -> GetMean(1), xyev -> GetMean(2), tcr - tcrtrg1);
            	  	 }

                         //time between Trgs MCP
        		 TimeDistributionTriggers -> Fill(tcrtrg2 - tcrtrg1);
		         //time between MCP and SIPM
			 Time -> Fill(tcr - tcr_Si);
       		   	 //in CanvGeom
  	    		 TimeVSx -> Fill(xyev -> GetMean(1), tcr - tcrtrg1);
	    		 TimeVSy -> Fill(xyev -> GetMean(2), tcr - tcrtrg1);
	                 TimeVSAmplitude -> Fill(mintrg, tcrtrg1 - tcrtrg2);
		 }
         }
    	 printf("%d\r",i); fflush(stdout);
         gSystem -> ProcessEvents();
    }
    CanvTimeDistribution -> cd(1);  
    ht -> Draw("");
    CanvGeom -> cd(3);
    xys -> Draw("text, colz");
    xyf -> Draw("box, same");
    CanvAmp -> cd(2);
    amptrg1->GetXaxis()->SetTitle("Time, ns");
    amptrg1 -> Draw("");
    CanvAmp -> cd(3);
    amptrg2->GetXaxis()->SetTitle("Time, ns");
    amptrg2 -> Draw("");
    CanvAmp -> cd(4);
    amp->GetXaxis()->SetTitle("Time, ns");
    amp -> Draw("");
    amptrg2 -> SetLineColor(kBlue);
    amptrg2 -> Draw("same");
    CanvAmp -> cd(5);
    ampsi->GetXaxis()->SetTitle("Time, ns");
    ampsi -> Draw("");
    amptrg2 -> SetLineColor(kBlue);
    amptrg2 -> Draw("same");
    CanvTimeDistribution -> cd(2);
    htsi -> Draw("");
    CanvAmp -> cd(6);
    ampavg->GetXaxis()->SetTitle("Time, ns");
    ampavg -> Draw("");
    CanvGeom -> cd(4);    
    TimeVSx -> Draw("");
    CanvGeom -> cd(5);
    TimeVSy -> Draw("");
    CanvTimeDistribution -> cd(3);
    TimeDistributionTriggers -> Draw("");
    CanvGeom -> cd(6);
    TimeVSAmplitude -> Draw("");
    
    CanvTimeDistribution -> cd(4);
    /*
    TF1* GaussBKG = new TF1("GaussBKG", "[0] * exp(-((x - [1]) / [2]) * ((x - [1]) / [2]) / 2) + [3]", -10, 10);
    GaussBKG -> SetParLimits(0, 1, 100);
    GaussBKG -> SetParLimits(1, 5, 10);
    GaussBKG -> SetParLimits(2, 0.1, 1);
    GaussBKG -> SetParLimits(3, 0, 10);
    GaussBKG -> SetParameter(0, 1);
    GaussBKG -> SetParameter(1, Time -> GetMean());
    GaussBKG -> SetParameter(2, 0.1);
    GaussBKG -> SetParameter(3, 0);
    Time -> Draw("");
    Time -> Fit(GaussBKG, "W", "S", -10, 10);
    Double_t Mean = GaussBKG -> GetParameter(1);
    Double_t Mean_Error = GaussBKG -> GetParError(1);
    Double_t Sigma = GaussBKG -> GetParameter(2);
    Double_t Sigma_Error = GaussBKG -> GetParError(2);
    */
    TF1* Gaus = new TF1("Gaus", "[0] * exp(-0.5*pow((x-[1])/[2],2))", -10, 10);
    Gaus -> SetParLimits(0, 1, 100);
    //Gaus -> SetParLimits(1, 5, 10);
    Gaus -> SetParLimits(1, 0, 10);
    Gaus -> SetParLimits(2, 0.1, 1);
    Gaus -> SetParameter(0, 1);
    Gaus -> SetParameter(1, Time -> GetMean());
    Gaus -> SetParameter(2, 0.1);
    Time -> Draw("");
    Gaus -> SetLineColor(kRed);
    Time -> Fit(Gaus, "W", "S", -10, 10);
    Double_t Mean = Gaus -> GetParameter(1);
    Double_t Mean_Error = Gaus -> GetParError(1);
    Double_t Sigma = Gaus -> GetParameter(2);
    Double_t Sigma_Error = Gaus -> GetParError(2);
 
    ofstream of(outDir + name + "_res_" + y_boundLeft + "-"+ y_boundRight + ".dat",ios_base::out);   
    of<<Mean<<"\t"<<Mean_Error<<"\t"<<Sigma<<"\t"<<Sigma_Error<<endl;
    of.close();

    CanvAmp -> Update();
    CanvGeom -> Update();
    CanvTimeDistribution -> Update(); 
    
    CanvTimeDistribution -> SaveAs(outDir + name + "(" + y_boundLeft + "-" + y_boundRight  + ")_CanvTimeDistribution.root");
    CanvAmp -> SaveAs(outDir + name + "(" + y_boundLeft + "-" + y_boundRight  + ")_CanvAmp.root");
    CanvGeom -> SaveAs(outDir + name + "(" + y_boundLeft + "-" + y_boundRight  + ")_CanvGeom.root");
}
