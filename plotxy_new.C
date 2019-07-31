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

    TString inputDir="/home/chizhik/root/2019-06-07_LYSO/";
    TString outDir="/home/ovtin/development/LYSO/out/";

    char s[100];
    // printing current working directory 
    printf("%s\n", getcwd(s, 100));    
    chdir(inputDir); 
    printf("%s\n", getcwd(s, 100)); 

    TString fpixel = TString(inputDir + "mcp" + name + ".root");
    cout << fpixel << endl;

    TFile* f1 = new TFile(fpixel, "read");
    TTree* exbeamdata = (TTree*)f1 -> Get("exbeamdata");
    exbeamdata -> Print();

    TString fdaq = TString(inputDir + "daq" + name + ".root");
    cout << fdaq << endl;
    TString fgem = TString(inputDir + "gem" + name + ".root");
    cout << fgem << endl;
    TString fsipm = TString(inputDir + "sipm" + name + ".root");
    cout << fsipm << endl;
    TString y_boundLeft;
    y_boundLeft.Form("%d", y1);
    TString y_boundRight;
    y_boundRight.Form("%d", y2);

    //!!Creating Canvas!!//

    TCanvas* CanvGeom = new TCanvas("CanvGeom", "Geometry(" + y_boundLeft + " - " + y_boundRight + ")", 1500, 600, 600, 600);
    CanvGeom -> Divide(2, 4);
    CanvGeom -> Draw();
    TCanvas* CanvAmp = new TCanvas("CanvAmp", "Amplitude distributions(" + y_boundLeft + " - " + y_boundRight + ")", 1000, 600, 600, 600);
    CanvAmp -> Divide(2, 3);
    CanvAmp -> Draw();
    TCanvas* CanvTimeDistribution = new TCanvas("CanvTimeDistribution", "Time distribution(" + y_boundLeft + " - " + y_boundRight + ")", 500, 600, 600, 600);
    CanvTimeDistribution -> Divide(2, 2);
    CanvTimeDistribution -> Draw();

    //!!Creating histograms for output!!//

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

    exbeamdata -> AddFriend("gem", fgem);
    exbeamdata -> AddFriend("daq", fdaq);
    exbeamdata -> AddFriend("exb2 = exbeamdata", fsipm);
    TString Conditions_1 = "(exbeamdata.chtrg2r.ped - exbeamdata.chtrg2r.min) > 300 && (exbeamdata.chtrg2r.ped - exbeamdata.chtrg2r.min) < 900 && (exbeamdata.chtrg1r.ped - exbeamdata.chtrg1r.min) > 300 && (exbeamdata.chtrg1r.ped - exbeamdata.chtrg1r.min) < 900 && daq.lecroy2249.ch0 > 800 && daq.lecroy2249.ch0 < 1600";
    TString Conditions_coor = "detClusters.x[2] > 40 &&  detClusters.x[2] < 49 && detClusters.y[2] >" + y_boundLeft + " &&  detClusters.y[2] <" + y_boundRight;
    TString Conditions_all = Conditions_1 + " && " + Conditions_coor;

    //!!CanvCuts -> cd(1): Amplitude of min in triggers per event!!//

    CanvAmp -> cd(1);
    exbeamdata -> Draw("exbeamdata.chtrg1r.ped - exbeamdata.chtrg1r.min >> htrg1"); // Amplitude of min in trigger per event//
    exbeamdata -> Draw("exbeamdata.chtrg2r.ped - exbeamdata.chtrg2r.min >> htrg2"); //#//
    htrg1 -> SetLineColor(2);
    htrg2 -> SetLineColor(4);
    htrg1 -> Draw();
    htrg2 -> Draw("same");
    TLine* mcpb1 = new TLine(300, 0, 300, 100);
    TLine* mcpb2 = new TLine(900, 0, 900, 100);
    mcpb1 -> Draw("same");
    mcpb2 -> Draw("same");
    CanvAmp -> Update();

    //!!CanvCuts -> cd(2): Map of hits!!//

    CanvGeom -> cd(1);
    exbeamdata -> Draw("detClusters.y[2]:detClusters.x[2] >> xy", Conditions_1,"");
    xy -> Draw("colz");
    CanvGeom -> Update();

    //!!Errors!!//

    TProfile* amp = new TProfile("amp", "a(t)", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* ampf = new TProfile("ampf", "a(t)", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* ampavg = new TProfile("ampavg", "a(t)", 1024, 1. / 1024, 0.2 * 1024);

    TProfile* ampsi = new TProfile("ampsi", "a(t) for SiPM", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* ampfsi = new TProfile("ampfsi", "a(t) for SiPM", 1024, 1. / 1024, 0.2 * 1024);

    TProfile* amptrg = new TProfile("amptrg", "a(t)", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* amptrg2 = new TProfile("amptrg2", "a2(t)", 1024, 1. / 1024, 0.2 * 1024);

    TProfile2D* dtvsxy = new TProfile2D("dtvsxy", "#Delta T vs XY", 140, 0, 140, 40, 0, 40);

    //!!Aproximations for signal!!//

    TF1* fun = Init_Aprox(200000, "fun");
    TF1* funsi = Init_Aprox(200000, "funsi");
    TF1* funtrg = Init_Aprox(-200000, "funtrg");
    TF1* funtrg2 = Init_Aprox(-200000, "funtrg2");
    TF1* GaussBKG = new TF1("GaussBKG", "[0] * exp(-((x - [1]) / [2]) * ((x - [1]) / [2]) / 2) + [3]", -10, 10);

    //!!MAIN LOOP!!//

    Int_t nentries = 0;
    if (exbeamdata -> GetEntries() < nev) nev = exbeamdata -> GetEntries();
    cout << nev << endl;
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
		
		exbeamdata -> Draw("(exb2.ch1.amp - exb2.ch1r.ped):exb2.ch1.ti / 5. >> ampfsi", Conditions_all , "goff", 1, i);
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
	    		 exbeamdata -> Draw("exb2.ch1.amp:exb2.ch1.ti / 5. >> ampsi", Conditions_all, "goff", 1, i);
           		 exbeamdata -> Draw("exbeamdata.chtrg1.amp:exbeamdata.chtrg1.ti / 5. >> amptrg", Conditions_all, "goff", 1, i);
            		 exbeamdata -> Draw("exbeamdata.chtrg2.amp:exbeamdata.chtrg2.ti / 5. >> amptrg2", Conditions_all, "goff", 1, i);

            		 amp -> SetAxisRange(2, 20, "X");
            		 Float_t ped = amp -> GetMean(2);
            		 amp -> SetAxisRange(2, 200, "X");
            		 Float_t min = amp -> GetMinimum();
            	//	 Float_t halfbin = ampf -> FindFirstBinAbove((ped - min) / 2., 1);

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
	    	//	 xys -> Draw("text, colz");
            		 exbeamdata -> Draw("detClusters.y[2]:detClusters.x[2] >> +xyf", Conditions_all, "goff", 1, i);
            	//	 xyf -> Draw("box, same");
            	//	 CanvGeom -> Update();
           	//	 CanvAmp -> Update();

		//	 CanvAmp -> cd(2);
            		 funtrg -> SetParameter(0, pedtrg);
            		 funtrg -> SetParameter(1, -2000);
 	    		 funtrg2 -> SetParameter(0, pedtrg2);
            		 funtrg2 -> SetParameter(1, -2000);
            	//	 amptrg -> Draw("");
            		 amptrg -> Fit(funtrg, "W", "S", (minbintrg - 5.5) /5., (minbintrg - 1.5) / 5.);
			 
		//	 CanvAmp -> cd(3);
            		 amptrg2 -> Fit(funtrg2,"W", "S", (minbintrg2 - 5.5) / 5., (minbintrg2 - 1.5) / 5.);
	    		 Float_t tcrtrg = (-1 / 2 * (pedtrg - mintrg) + pedtrg - funtrg -> GetParameter(0)) / (funtrg -> GetParameter(1));
	    		 Float_t tcrtrg2 = (-1 / 2 * (pedtrg2 - mintrg2) + pedtrg2 - funtrg2 -> GetParameter(0)) / (funtrg2 -> GetParameter(1));
            		 
            	//	 CanvAmp -> cd(4); 
			 fun -> SetParameter(0, ped);
            		 fun -> SetParameter(1, -200);
            	//	 amp -> Draw();
            		 amp -> Fit(fun, "W", "S", (FirstSignalBin - 0.5) / 5., (FirstSignalBin + 4) / 5.);
			 
			 CanvAmp -> cd(5);
			 funsi -> SetParameter(0, pedtrg);
            		 funsi -> SetParameter(1, -2000);
		//	 ampsi -> Draw();
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

		//	 CanvTimeDistribution -> cd(1);
            		 ht -> Fill(tcr - tcrtrg);          		 
		//	 ht -> Draw("");

		//	 CanvTimeDistribution -> cd(2);
			 htsi -> Fill(tcr_Si - tcrtrg);
		//	 htsi -> Draw("");

           	//	 CanvAmp -> cd(6);
            	//	 ampavg -> Draw("");
            	//	 cout << "dT=" << (tcr - tcrtrg) << endl;
            	//	 CanvTimeDistribution -> Update();
            	//	 CanvGeom -> cd(3);
            		 if ((tcr - tcrtrg) < 15 && (tcr - tcrtrg) > -2) {
                	 	dtvsxy -> Fill(xyev -> GetMean(1), xyev -> GetMean(2), tcr - tcrtrg);
                //		dtvsxy -> Draw("lego2, 0");
            	  	 }
	    	//	 CanvGeom -> Update();

	    	//	 CanvGeom -> cd(4);
	    		 TimeVSx -> Fill(xyev -> GetMean(1), tcr - tcrtrg);
	    	//	 TimeVSx -> Draw("colz");
	    	
		//	 CanvGeom -> cd(5);
	    		 TimeVSy -> Fill(xyev -> GetMean(2), tcr - tcrtrg);
	    	//	 TimeVSy -> Draw("colz");

	    	//	 CanvTimeDistribution -> cd(3);
	    		 TimeDistributionTriggers -> Fill(tcrtrg2 - tcrtrg);
	    	//	 TimeDistributionTriggers -> Draw();

	    	//	 CanvGeom -> cd(6);
	    		 TimeVSAmplitude -> Fill(mintrg, tcrtrg - tcrtrg2);
	    	//	 TimeVSAmplitude -> Draw("colz");

		//	 CanvTimeDistribution -> cd(4);
			 Time -> Fill(tcr - tcr_Si);
		//	 CanvAmp -> Update();
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
    
    CanvTimeDistribution -> SaveAs(outDir + name + "(" + y_boundLeft + " - " + y_boundRight  + ")_CanvTimeDistribution.root");
    CanvAmp -> SaveAs(outDir + name + "(" + y_boundLeft + " - " + y_boundRight  + ")_CanvAmp.root");
    CanvGeom -> SaveAs(outDir + name + "(" + y_boundLeft + " - " + y_boundRight  + ")_CanvGeom.root");
/*
    GaussBKG -> SetParLimits(0, 1, 100);
    GaussBKG -> SetParLimits(1, 5, 10);
    GaussBKG -> SetParLimits(2, 0.1, 1);
    GaussBKG -> SetParLimits(3, 0, 10);
    GaussBKG -> SetParameter(0, 1);
    GaussBKG -> SetParameter(1, Time -> GetMean());
    GaussBKG -> SetParameter(2, 0.1);
    GaussBKG -> SetParameter(3, 0);
    Time -> Fit("GaussBKG", "w", "S");
*/
}
