//function form fit
TF1* Init_Aprox(Int_t num, const char* name) {
	TF1* f = new TF1(name ,"[0] + [1] * x", 0, 200);
     	f -> SetParName(0, "Const");
     	f -> SetParName(1, "Angle");
     	f -> SetParLimits(1, num, 0);
	f -> SetTitle(name);
	return f;
}

void plotxy_new(TString name, Int_t nev, Int_t nev1, Int_t nev2, Int_t y1, Int_t y2) {
    
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
 
    exbeamdata -> Print();    
    //Friend tree exbeamdata from MCP with gem, daq, sipm
    exbeamdata -> AddFriend("gem");
    exbeamdata -> AddFriend("daq");
    exbeamdata -> AddFriend(sipm,"sipm");
 
    //boundaries along y-coordinate of LYSO bar
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
    TH2F* xy = new TH2F("xy", "GEM hits", 140, 0, 140, 40, 0, 40);
    TH2F* xys = new TH2F("xys", "GEM hits with SiPM", 140, 0, 140, 40, 0, 40); //Need??//
    TH2F* xyev = new TH2F("xyev", "GEM hits per Event", 140, 0, 140, 40, 0, 40);
    TH1F* htrg1 = new TH1F("htrg1", "MCP PMT spectra", 4096, 0, 4096);
    TH1F* htrg2 = new TH1F("htrg2", "MCP PMT spectra", 4096, 0, 4096);
    TH1F* ht = new TH1F("ht","#Delta t", 2001, -10.005, 10.005);
    TH1F* htsi = new TH1F("htsi","#Delta t for SiPM", 2001, -10.005, 10.005);
    TH1F* Time = new TH1F("Time", "Time of registration", 2001, -10.005, 10.005);
    TH1F* TimeDistributionTriggers  =  new TH1F("TimeDistributionTriggers", "Time distribution between triggers", 2001, -10.005, 10.005);
    TH2F* TimeVSx = new TH2F("TimeVSx", "", 100, 0, 100, 100, 0, 10);
    TH2F* TimeVSy = new TH2F("TimeVSy", "", 100, 0, 100, 100, 0, 10);
    TH2F* TimeVSAmplitude = new TH2F("TimeVSAmplitude", "Time VS Amplitude", 10000, 1700, 2400, 20, 3, 4);
    TH2F* xyf = new TH2F("xyf", "pixels", 40, 40, 80, 40, 0, 40);

    //======================
   //selection conditions for trgMCP and DAQ
    TString Conditions_1 = "(exbeamdata.chtrg2r.ped - exbeamdata.chtrg2r.min) > 300 && (exbeamdata.chtrg2r.ped - exbeamdata.chtrg2r.min) < 900 && (exbeamdata.chtrg1r.ped - exbeamdata.chtrg1r.min) > 300 && (exbeamdata.chtrg1r.ped - exbeamdata.chtrg1r.min) < 900 && daq.lecroy2249.ch0 > 800 && daq.lecroy2249.ch0 < 1600";
    //selection conditions for GEM
    TString Conditions_coor = "detClusters.x[2] > 40 &&  detClusters.x[2] < 49 && detClusters.y[2] >" + y_boundLeft + " &&  detClusters.y[2] <" + y_boundRight;
    TString Conditions_all = Conditions_1 + " && " + Conditions_coor;

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
    exbeamdata -> Draw("detClusters.y[2]:detClusters.x[2] >> xy", Conditions_1,"");
    xy -> Draw("colz");
    CanvGeom -> Update();

    TProfile* amp = new TProfile("amp", "a(t)", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* ampf = new TProfile("ampf", "a(t)", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* ampavg = new TProfile("ampavg", "a(t)", 1024, 1. / 1024, 0.2 * 1024);

    TProfile* ampsi = new TProfile("ampsi", "a(t) for SiPM", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* ampfsi = new TProfile("ampfsi", "a(t) for SiPM", 1024, 1. / 1024, 0.2 * 1024);

    TProfile* amptrg = new TProfile("amptrg", "a(t)", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* amptrg2 = new TProfile("amptrg2", "a2(t)", 1024, 1. / 1024, 0.2 * 1024);

    TProfile2D* dtvsxy = new TProfile2D("dtvsxy", "#Delta T vs XY", 140, 0, 140, 40, 0, 40);

    //Aproximations for signal
    TF1* fun = Init_Aprox(200000, "fun");
    TF1* funsi = Init_Aprox(200000, "funsi");
    TF1* funtrg = Init_Aprox(-200000, "funtrg");
    TF1* funtrg2 = Init_Aprox(-200000, "funtrg2");
    TF1* GaussBKG = new TF1("GaussBKG", "[0] * exp(-((x - [1]) / [2]) * ((x - [1]) / [2]) / 2) + [3]", -10, 10);

    Int_t nentries = 0;
    if (exbeamdata -> GetEntries() < nev) nev = exbeamdata -> GetEntries();
    cout << nev << endl;
    //MAIN LOOP
    for (int i = 0; i < nev; i++){
    	if (!(i > nev1 && i < nev2)) {
        	exbeamdata -> Draw("(exbeamdata.ch1r.ped - exbeamdata.ch1.amp):exbeamdata.ch1.ti / 5. >> ampf", Conditions_all , "goff", 1, i);
 		ampf -> SetAxisRange(2, 20, "X");
        	Float_t pedmax = ampf -> GetMaximum();
        	Float_t pedmin = ampf -> GetMinimum();
        	ampf -> SetAxisRange(2, 200, "X");
        	Float_t MaximumSignalBin = ampf -> GetMaximumBin();
        	Float_t MaximumSignal = ampf -> GetMaximum();
		Float_t FirstSignalBin = ampf -> FindFirstBinAbove(pedmax + MaximumSignal * 0.2, 1);
		Float_t HalfHeightBinLeft = ampf -> FindFirstBinAbove(MaximumSignal / 2.0, 1);
		Float_t HalfHeightBinRight = ampf -> FindLastBinAbove(MaximumSignal / 2.0, 1); //MaximumSignalBin);
		
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
			if (FirstSignalBin - MaximumSignalBin < 0 && MaximumSignalBin > 100 && MaximumSignal > 2 * (pedmax - pedmin) && (HalfHeightBinRight - HalfHeightBinLeft) > 10 && MaximumSignalBin < 350){
	    		 exbeamdata -> Draw("exbeamdata.ch1.amp:exbeamdata.ch1.ti / 5. >> amp", Conditions_all, "goff", 1, i);
	    		 exbeamdata -> Draw("sipm.ch1.amp:sipm.ch1.ti / 5. >> ampsi", Conditions_all, "goff", 1, i);
           		 exbeamdata -> Draw("exbeamdata.chtrg1.amp:exbeamdata.chtrg1.ti / 5. >> amptrg", Conditions_all, "goff", 1, i);
            		 exbeamdata -> Draw("exbeamdata.chtrg2.amp:exbeamdata.chtrg2.ti / 5. >> amptrg2", Conditions_all, "goff", 1, i);

            		 amp -> SetAxisRange(2, 20, "X");
            		 Float_t ped = amp -> GetMean(2);
            		 amp -> SetAxisRange(2, 200, "X");
            		 Float_t min = amp -> GetMinimum();

			 ampsi -> SetAxisRange(2, 20, "X");
            		 Float_t ped_Si = ampsi -> GetMean(2);
            		 ampsi -> SetAxisRange(2, 200, "X");
            		 Float_t min_Si = ampsi -> GetMinimum();
           
	   		 amptrg -> SetAxisRange(2, 20, "X");
            		 Float_t pedtrg = amptrg -> GetMean(2);
            		 amptrg -> SetAxisRange(2, 200, "X");
            		 Float_t minbintrg = amptrg -> GetMinimumBin();
            		 Float_t mintrg = amptrg -> GetMinimum();

   		         amptrg2 -> SetAxisRange(2, 20, "X");
	            	 Float_t pedtrg2 = amptrg2 -> GetMean(2);
            		 amptrg2 -> SetAxisRange(2, 200, "X");
            		 Float_t minbintrg2 = amptrg2 -> GetMinimumBin();
            		 Float_t mintrg2 = amptrg2 -> GetMinimum();

            		 CanvGeom -> cd(2);
            		 exbeamdata -> Draw("detClusters.y[2]:detClusters.x[2] >> +xys", Conditions_all, "goff", 1, i);
            		 exbeamdata -> Draw("detClusters.y[2]:detClusters.x[2] >> xyev", Conditions_all, "goff", 1, i);
            		 exbeamdata -> Draw("detClusters.y[2]:detClusters.x[2] >> +xyf", Conditions_all, "goff", 1, i);

            		 funtrg -> SetParameter(0, pedtrg);
            		 funtrg -> SetParameter(1, -2000);
 	    		 funtrg2 -> SetParameter(0, pedtrg2);
            		 funtrg2 -> SetParameter(1, -2000);
            		 amptrg -> Fit(funtrg, "W", "S", (minbintrg - 5.5) /5., (minbintrg - 1.5) / 5.);
			 
            		 amptrg2 -> Fit(funtrg2,"W", "S", (minbintrg2 - 5.5) / 5., (minbintrg2 - 1.5) / 5.);
	    		 Float_t tcrtrg = (-1 / 2 * (pedtrg - mintrg) + pedtrg - funtrg -> GetParameter(0)) / (funtrg -> GetParameter(1));
	    		 Float_t tcrtrg2 = (-1 / 2 * (pedtrg2 - mintrg2) + pedtrg2 - funtrg2 -> GetParameter(0)) / (funtrg2 -> GetParameter(1));
            		 
			 fun -> SetParameter(0, ped);
            		 fun -> SetParameter(1, -200);
            		 amp -> Fit(fun, "W", "S", (FirstSignalBin - 0.5) / 5., (FirstSignalBin + 4) / 5.);
			 
			 CanvAmp -> cd(5);
			 funsi -> SetParameter(0, pedtrg);
            		 funsi -> SetParameter(1, -2000);
			 ampsi -> Fit(funsi, "W", "S", (FirstSignalBin_Si - 10) / 5., (FirstSignalBin_Si + 25) / 5.);

            //======================

            		 Float_t tcr = (-0.5 * (ped - min) + ped - fun -> GetParameter(0)) / (fun -> GetParameter(1));
            		 Float_t tcr_Si = (-0.5 * (ped_Si - min_Si) + ped_Si - funsi -> GetParameter(0)) / (funsi-> GetParameter(1));

            //======================

            	//	 cout << "Max_bin:" << MaximumSignalBin << "\tAmp:" << (ped - min) << "\tHalfbin:" << halfbin << "\tTcr:" << tcr << endl;
            	//	 cout << "Min_bin:" << minbintrg << "\tAmp(TRG):" << (pedtrg - mintrg) << "\tMin(TRG):" << mintrg << "\tTcr(TRG):" << tcrtrg << endl;
	    	//	 cout << "tcr = "<<  tcr << "tcrtrg = " << tcrtrg << endl;

            //======================

            	 	 if ( (xyf -> GetEntries() - nentries) == 1 ) nentries = xyf -> GetEntries();
            		 for (int i = 0; i < amp -> GetNbinsX(); i++) {
                		 ampavg -> Fill(amp -> GetBinCenter(i) - tcrtrg + 60, amp -> GetBinContent(i));
            		 }
            		 
            //======================

            		 ht -> Fill(tcr - tcrtrg);          		 
			 htsi -> Fill(tcr_Si - tcrtrg);

            	//	 cout << "dT=" << (tcr - tcrtrg) << endl;
            		 if ((tcr - tcrtrg) < 15 && (tcr - tcrtrg) > -2) {
                	 	dtvsxy -> Fill(xyev -> GetMean(1), xyev -> GetMean(2), tcr - tcrtrg);
            	  	 }

	    		 TimeVSx -> Fill(xyev -> GetMean(1), tcr - tcrtrg);
	    		 TimeVSy -> Fill(xyev -> GetMean(2), tcr - tcrtrg);
	    		 TimeDistributionTriggers -> Fill(tcrtrg2 - tcrtrg);
	    		 TimeVSAmplitude -> Fill(mintrg, tcrtrg - tcrtrg2);
			 Time -> Fill(tcr - tcr_Si);
       		 }
         }
    	 printf("%d\r",i); fflush(stdout);
         gSystem -> ProcessEvents();
    }
    CanvTimeDistribution -> cd(1);  
    ht -> Draw("");
    CanvGeom -> cd(2);
    xys -> Draw("text, colz");
    xyf -> Draw("box, same");
    CanvAmp -> cd(2);
    amptrg -> Draw("");
    CanvAmp -> cd(3);
    amptrg2 -> Draw("");
    CanvAmp -> cd(4);
    amp -> Draw("");
    CanvAmp -> cd(5);
    ampsi -> Draw("");
    CanvTimeDistribution -> cd(2);
    htsi -> Draw("");
    CanvAmp -> cd(6);
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
    Time -> Draw("");
    CanvAmp -> Update();
    CanvGeom -> Update();
    CanvTimeDistribution -> Update(); 
    
    CanvTimeDistribution -> SaveAs(outDir + name + "(" + y_boundLeft + "-" + y_boundRight  + ")_CanvTimeDistribution.root");
    CanvAmp -> SaveAs(outDir + name + "(" + y_boundLeft + "-" + y_boundRight  + ")_CanvAmp.root");
    CanvGeom -> SaveAs(outDir + name + "(" + y_boundLeft + "-" + y_boundRight  + ")_CanvGeom.root");
}
