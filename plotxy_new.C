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

void plotxy_new(TString name, Int_t nev, Int_t nev1, Int_t nev2, Int_t y1, Int_t y2, Float_t threshold, bool verbose) {
    
    gStyle->SetOptStat(1);
    //gStyle->SetOptFit(1);
    //gROOT->SetStyle("Plain");
    gStyle -> SetPalette(1);

    TString thr;
    thr.Form("%.2f", threshold);
 
    TString outDir="/home/ovtin/development/LYSO/out/out1_trh" + thr + "/";
 
    TChain *exbeamdata=new TChain("exbeamdata");
    TChain *daq=new TChain("daq");
    TChain *gem=new TChain("gem");
    TChain *sipm=new TChain("exbeamdata");
   
    if(name=="lyso30minus")
    { 
     //data from MCP
     exbeamdata->Add("/home/ovtin/development/LYSO/2019-06-07/2019-06-07_18-37-52/mcplyso2019-06-07_18-37.root");
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_18-53-23/mcplyso2019-06-07_18-53.root");
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-00-52/mcplyso2019-06-07_19-00.root");
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-05-12/mcplyso2019-06-07_19-05.root");
     exbeamdata->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-10-16/mcplyso2019-06-07_19-10.root");
 
     daq->Add("/home/ovtin/development/LYSO/2019-06-07/2019-06-07_18-37-52/2019-06-07_18-37.root");
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_18-53-23/2019-06-07_18-53.root");
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-00-52/2019-06-07_19-00.root");
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-05-12/2019-06-07_19-05.root");
     daq->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-10-16/2019-06-07_19-10.root");
 
     gem->Add("/home/ovtin/development/LYSO/2019-06-07/2019-06-07_18-37-52/gem_2019-06-07_18-37.root");
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_18-53-23/gem_2019-06-07_18-53.root");
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-00-52/gem_2019-06-07_19-00.root");
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-05-12/gem_2019-06-07_19-05.root");
     gem->Add("/home/chizhik/root/2019-06-07_LYSO/2019-06-07_19-10-16/gem_2019-06-07_19-10.root");

     //data from SIPM
     sipm->Add("/home/ovtin/development/LYSO/2019-06-07/2019-06-07_18-37-52/sipm2019-06-07_18-37.root");
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


   if(name=="quartzzero")
    { 
     //data from MCP
     exbeamdata->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-31-49/mcpquartz2019-07-02_17-31.root");
     exbeamdata->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-39-53/mcpquartz2019-07-02_17-39.root");
     exbeamdata->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-46-46/mcpquartz2019-07-02_17-46.root");
     exbeamdata->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-52-12/mcpquartz2019-07-02_17-52.root");
     exbeamdata->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-56-31/mcpquartz2019-07-02_17-56.root");
 
     daq->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-31-49/2019-07-02_17-31.root");
     daq->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-39-53/2019-07-02_17-39.root");
     daq->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-46-46/2019-07-02_17-46.root");
     daq->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-52-12/2019-07-02_17-52.root");
     daq->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-56-31/2019-07-02_17-56.root");
 
     gem->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-31-49/gem_2019-07-02_17-31.root");
     gem->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-39-53/gem_2019-07-02_17-39.root");
     gem->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-46-46/gem_2019-07-02_17-46.root");
     gem->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-52-12/gem_2019-07-02_17-52.root");
     gem->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-56-31/gem_2019-07-02_17-56.root");

     //data from SIPM
     sipm->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-31-49/sipm2019-07-02_17-31.root");
     sipm->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-39-53/sipm2019-07-02_17-39.root");
     sipm->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-46-46/sipm2019-07-02_17-46.root");
     sipm->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-52-12/sipm2019-07-02_17-52.root");
     sipm->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_17-56-31/sipm2019-07-02_17-56.root");
    }
 
    if(name=="quartz30plus")
    { 
     //data from MCP
     exbeamdata->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-33-02/mcpquartz2019-07-02_19-33.root");
     exbeamdata->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-40-30/mcpquartz2019-07-02_19-40.root");
     exbeamdata->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-46-16/mcpquartz2019-07-02_19-46.root");
     exbeamdata->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-51-22/mcpquartz2019-07-02_19-51.root");
     exbeamdata->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-57-06/mcpquartz2019-07-02_19-57.root");
 
     daq->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-33-02/2019-07-02_19-33.root");
     daq->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-40-30/2019-07-02_19-40.root");
     daq->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-46-16/2019-07-02_19-46.root");
     daq->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-51-22/2019-07-02_19-51.root");
     daq->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-57-06/2019-07-02_19-57.root");
 
     gem->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-33-02/gem_2019-07-02_19-33.root");
     gem->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-40-30/gem_2019-07-02_19-40.root");
     gem->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-46-16/gem_2019-07-02_19-46.root");
     gem->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-51-22/gem_2019-07-02_19-51.root");
     gem->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-57-06/gem_2019-07-02_19-57.root");

     //data from SIPM
     sipm->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-33-02/sipm2019-07-02_19-33.root");
     sipm->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-40-30/sipm2019-07-02_19-40.root");
     sipm->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-46-16/sipm2019-07-02_19-46.root");
     sipm->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-51-22/sipm2019-07-02_19-51.root");
     sipm->Add("/home/ovtin/development/LYSO/2019-07-02/2019-07-02_19-57-06/sipm2019-07-02_19-57.root");
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

    TString x_boundLeft, x_boundRight;

    //create Canvas
    TCanvas* CanvGeom = new TCanvas("CanvGeom", "Geometry(" + y_boundLeft + " - " + y_boundRight + ")", 1500, 600, 600, 600);
    CanvGeom -> Divide(2, 4);
    TCanvas* CanvAmp = new TCanvas("CanvAmp", "Amplitude distributions(" + y_boundLeft + " - " + y_boundRight + ")", 1000, 600, 600, 600);
    CanvAmp -> Divide(3, 3);
    TCanvas* CanvTimeDistribution = new TCanvas("CanvTimeDistribution", "Time distribution(" + y_boundLeft + " - " + y_boundRight + ")", 500, 600, 600, 600);
    CanvTimeDistribution -> Divide(3, 3);

    //create histograms for output
    TH2F* xy1 = new TH2F("xy1", "GEM1 hits", 140, 0, 140, 40, 0, 40);
    TH2F* xy2 = new TH2F("xy2", "GEM2 hits", 140, 0, 140, 40, 0, 40);
    TH2F* xys = new TH2F("xys", "GEM hits with SiPM", 30, 30, 60, 20, 10, 30); //Need??//
    TH2F* xyev = new TH2F("xyev", "GEM hits per Event", 140, 0, 140, 40, 0, 40);
    TH1F* htrg1 = new TH1F("htrg1", "MCP PMT spectra", 4096, 0, 4096);
    TH1F* htrg2 = new TH1F("htrg2", "MCP PMT spectra", 4096, 0, 4096);
    TH1F* ht = new TH1F("ht","#Delta t for MCP", 6000, -10.005, 10.005);
    TH1F* hNaI= new TH1F("hNaI", "NaI calorimeter", 500, 0, 2000);
    TH1F* htsi = new TH1F("htsi","#Delta t for SiPM", 6000, -10.005, 10.005);
    TH1F* Time = new TH1F("Time", "Time of registration (MCP-SiPM)", 6000, -10.005, 10.005);
    TH1F* TimeCor = new TH1F("Time", "Time of registration (MCP-SiPM)", 6000, -10.005, 10.005);
    //TH1F* Time = new TH1F("Time", "Time of registration (MCP-SiPM)", 500, -10.005, 10.005);
    TH1F* TimeDistributionTriggers  =  new TH1F("TimeDistributionTriggers", "Time distribution between triggers MCP", 2001, -10.005, 10.005);
    TH2F* TimeVSx = new TH2F("TimeVSx", "", 30, 30, 60, 70, 0, 7);
    TH2F* TimeVSy = new TH2F("TimeVSy", "", 20, 10, 30, 70, 0, 7);
    TH2F* TimeVSAmplitude = new TH2F("TimeVSAmplitude", "Time VS Amplitude", 2000, 0, 2000, 100, 0, 6);
    TH2F* xyf = new TH2F("xyf", "pixels", 40, 40, 80, 40, 0, 40);

    //======================
   //selection conditions for trgMCP and DAQ
    TString Conditions_1 = "(exbeamdata.chtrg2r.ped - exbeamdata.chtrg2r.min) > 300 && (exbeamdata.chtrg2r.ped - exbeamdata.chtrg2r.min) < 900 && (exbeamdata.chtrg1r.ped - exbeamdata.chtrg1r.min) > 300 && (exbeamdata.chtrg1r.ped - exbeamdata.chtrg1r.min) < 900 && daq.lecroy2249.ch0 > 800 && daq.lecroy2249.ch0 < 1700";
    //selection conditions for GEM
    TString Conditions_coor1;
    //Conditions_coor1 = "detClusters.x[2] > 36 &&  detClusters.x[2] < 48 && detClusters.y[2] >" + y_boundLeft + " &&  detClusters.y[2] <" + y_boundRight;
    //Conditions_coor1 = "detClusters.x[2] > 41 &&  detClusters.x[2] < 46 && detClusters.y[2] > 15 &&  detClusters.y[2] < 27";
    //if(name=="lysozero")Conditions_coor1 = "detClusters.x[2] > 41 &&  detClusters.x[2] < 44 && detClusters.y[2] > 15 &&  detClusters.y[2] < 27";
    if(name=="lysozero")Conditions_coor1 = "detClusters.x[2] > 41 &&  detClusters.x[2] < 44 && detClusters.y[2] > 15 &&  detClusters.y[2] < 22";
    if(name=="lyso30plus")Conditions_coor1 = "detClusters.x[2] > 42 &&  detClusters.x[2] < 45 && detClusters.y[2] > 15  &&  detClusters.y[2] < 20";
    if(name=="lyso30minus")Conditions_coor1 = "detClusters.x[2] > 43 &&  detClusters.x[2] < 46 && detClusters.y[2] > 15 && detClusters.y[2] < 21";
    //TString Conditions_coor2 = "detClusters.x[3] > 64 &&  detClusters.x[3] < 76 && detClusters.y[3] > 10 && detClusters.y[3] < 28";
    //TString Conditions_2 = "(exbeamdata.ch1r.ped-exbeamdata.ch1r.min)>300 && (exbeamdata.ch1r.ped-exbeamdata.ch1r.min)<900 && (sipm.ch1r.max-sipm.ch1r.ped)>500 && (sipm.ch1r.max-sipm.ch1r.ped)<1150 && (exbeamdata.ch1r.tmin-exbeamdata.chtrg2r.tmin)/5<10 && (sipm.ch1r.tmax-sipm.chtrg2r.tmin)/5<64"; 
    TString Conditions_2 = "(exbeamdata.ch1r.ped-exbeamdata.ch1r.min)>300 && (exbeamdata.ch1r.ped-exbeamdata.ch1r.min)<900 && (sipm.ch1r.max-sipm.ch1r.ped)>200 && (sipm.ch1r.max-sipm.ch1r.ped)<1200 && (exbeamdata.ch1r.tmin-exbeamdata.chtrg2r.tmin)/5<12 && (sipm.ch1r.tmax-sipm.chtrg2r.tmin)/5<67"; 
    //TString Conditions_all = Conditions_1 + " && " + Conditions_2 + " && "+ Conditions_coor1 + "&&" + Conditions_coor2;
    TString Conditions_all = Conditions_1 + " && " + Conditions_2 + " && "+ Conditions_coor1;
    //TString Conditions_all = Conditions_1 + " && " + Conditions_coor1;
    TString Conditions_noampl = Conditions_1 + " && " + Conditions_coor1 + " && " + Conditions_2;

    TProfile* amp = new TProfile("amp", "a(t) for MCP ", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* ampf = new TProfile("ampf", "a(t)f for MCP", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* ampavg = new TProfile("ampavg", "Average a(t) for MCP", 1024, 1. / 1024, 0.2 * 1024);

    TProfile* ampsi = new TProfile("ampsi", "a(t) for SiPM", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* ampfsi = new TProfile("ampfsi", "a(t)f for SiPM", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* ampsiavg = new TProfile("ampsiavg", "Average a(t) for SiPM", 1024, 1. / 1024, 0.2 * 1024);

    TProfile* amptrg1 = new TProfile("amptrg1", "a1(t) for trgmcp1", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* amptrg2 = new TProfile("amptrg2", "a2(t) for trgmcp2", 1024, 1. / 1024, 0.2 * 1024);

    TProfile2D* dtvsxy = new TProfile2D("dtvsxy", "#Delta T vs XY", 140, 0, 140, 40, 0, 40);

    TH2F* avstmcp=new TH2F("avstmcp","A(t) for MCP",110,-60,170,4096,0,4096);
    TH2F* avstsipm=new TH2F("avstsipm","A(t) for SiPM",110,-60,170,4096,0,4096);

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
		exbeamdata -> Draw("(exbeamdata.ch1r.ped-exbeamdata.ch1r.min):(exbeamdata.ch1r.tmin-exbeamdata.chtrg2r.tmin)/5>>+avstmcp",Conditions_noampl, "goff", 1, i); 
		exbeamdata -> Draw("(sipm.ch1r.max-sipm.ch1r.ped):(sipm.ch1r.tmax-sipm.chtrg2r.tmin)/5>>+avstsipm",Conditions_noampl, "goff", 1, i); 
       
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
            		 Float_t max_Si = ampsi -> GetMaximum();
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

			 Int_t Bin2trg1=220.;
                         for(Bin2trg1; Bin2trg1<1024.; Bin2trg1++)
                         { 
			   if(  (amptrg1 -> GetBinContent(Bin2trg1) / pedtrg1 ) <= 0.9890 )
			   {
     			     break;
			   } 
			 }
                         Int_t Bin3trg1=Bin2trg1;
                         for(Bin3trg1; Bin3trg1<1024.; Bin3trg1++)
			 {  
                           Bin3trg1++;
                            if( amptrg1->GetBinContent(Bin3trg1) > amptrg1->GetBinContent(Bin3trg1-1.) )
                           {
                             break;
                           }  			    
			 }
                         Int_t FitBin1trg1=Bin2trg1+0.5;
                         Int_t FitBin2trg1=Bin3trg1-3.0;         
            		 amptrg1 -> Fit(funtrg, "W", "S", FitBin1trg1/5., FitBin2trg1/5.);     //fit in range  

			 Int_t Bin2trg2=220.;
                         for(Bin2trg2; Bin2trg2<500.; Bin2trg2++)
                         { 
			   if(  (amptrg2 -> GetBinContent(Bin2trg2) / pedtrg2 ) <= 0.9890 )
			   {
     			     break;
			   } 
			 }
                         Int_t Bin3trg2=Bin2trg2;
                         for(Bin3trg2; Bin3trg2<500.; Bin3trg2++)
			 {  
                           Bin3trg2++;
                            if( amptrg2->GetBinContent(Bin3trg2) > amptrg2->GetBinContent(Bin3trg2-1.) )
                           {
                             break;
                           }  			    
			 }
                         Int_t FitBin1trg2=Bin2trg2+0.5;
                         Int_t FitBin2trg2=Bin3trg2-3.0; 
            		 amptrg2 -> Fit(funtrg2, "W", "S", FitBin1trg2/5., FitBin2trg2/5.);     //fit in range  
                         //we find the solution to the intersection of lines: y=[0]+[1]*x and y=const
             		 //Float_t tcrtrg1 = (-threshold * (pedtrg1 - mintrg) + pedtrg1 - funtrg -> GetParameter(0)) / (funtrg -> GetParameter(1));
	    		 //Float_t tcrtrg2 = (-threshold * (pedtrg2 - mintrg2) + pedtrg2 - funtrg2 -> GetParameter(0)) / (funtrg2 -> GetParameter(1));
            		 Float_t tcrtrg1 = (-threshold * (pedtrg1 - amptrg1->GetBinContent(FitBin2trg1)) + pedtrg1 - funtrg -> GetParameter(0)) / (funtrg -> GetParameter(1));
	    		 Float_t tcrtrg2 = (-threshold * (pedtrg2 - amptrg2->GetBinContent(FitBin2trg2)) + pedtrg2 - funtrg2 -> GetParameter(0)) / (funtrg2 -> GetParameter(1));
            		 //cout<<"-threshold * (pedtrg2 - mintrg2) + pedtrg2="<<-threshold * (pedtrg2 - mintrg2) + pedtrg2<<"\t"<<"Event="<<i<<endl;
            		 if(verbose)cout<<"tcrtrg1="<<tcrtrg1<<"\t"<<"Event="<<i<<endl;
            		 if(verbose)cout<<"tcrtrg2="<<tcrtrg2<<"\t"<<"Event="<<i<<endl;

 	                 //time of MCP	
			 fun -> SetParameter(0, ped);
            		 fun -> SetParameter(1, -200);
                         //
			 Int_t Bin2mcp=250;
                         for(Bin2mcp; Bin2mcp<500.; Bin2mcp++)
                        { 
			   if(  (amp -> GetBinContent(Bin2mcp) / ped ) <= 0.9895 )
			   {
     			     break;
			   } 
			 }
                         Int_t Bin3mcp=Bin2mcp;
                         for(Bin3mcp; Bin3mcp<1024.; Bin3mcp++)
			 {  
                           Bin3mcp++;
                            if( amp->GetBinContent(Bin3mcp) > amp->GetBinContent(Bin3mcp-1) || (amp->GetBinContent(Bin3mcp-1)-amp->GetBinContent(Bin3mcp))<5 )
                           {
                             break;
                           }  			    
			 }
                         Int_t FitBin1mcp=Bin2mcp+1.0;
                         Int_t FitBin2mcp=Bin3mcp-3.5; 
            		 amp -> Fit(fun, "W", "S", FitBin1mcp / 5., FitBin2mcp / 5.);
                         //cout<<"amp -> GetBinContent(Bin1mcp)="<<amp -> GetBinContent(Bin2mcp-8)<<"\t"<<"amp -> GetBinContent(Bin2mcp)="<<amp -> GetBinContent(Bin2mcp)<<endl; 	
           		 //cout<<"FitBin1mcp="<<FitBin1mcp/5.<<"\t"<<"FitBin2mcp="<<FitBin2mcp/5.<<endl;
                           
                         //Float_t tcr = (-threshold * (ped - min) + ped - fun -> GetParameter(0)) / (fun -> GetParameter(1));
                         Float_t tcr = (-threshold * (ped - amp -> GetBinContent(Bin3mcp)) + ped - fun -> GetParameter(0)) / (fun -> GetParameter(1));
            	         //cout<<"-threshold * (ped - min) + ped="<<-threshold * (ped - amp -> GetBinContent(Bin3mcp)) + ped<<"\t"<<"Event="<<i<<endl;
            	         //cout<<"-threshold * (ped - min) + ped="<<-threshold * (ped - min) + ped<<"\t"<<"Event="<<i<<endl;
            		 if(verbose)cout<<"tcr="<<tcr<<"\t"<<"Event="<<i<<endl;
 
           		 //time of SIPM 		 
			 CanvAmp -> cd(5);
			 funsi -> SetParameter(0, pedtrg1);
            		 funsi -> SetParameter(1, -2000);
                         
	                 Int_t FitBin1sipm=0;
                         Int_t j4=0;
          	         Int_t maxbinsipm = ampsi -> GetMaximumBin();
                         Float_t binContent = ampsi -> GetBinContent(maxbinsipm);
		         //while ( (ped_Si + 4) < binContent )
		         while ( (ped_Si + 6) < binContent )
			 {
			  j4++;
                          FitBin1sipm = maxbinsipm-j4;
                          binContent = ampsi -> GetBinContent(FitBin1sipm);                                                  
                         }
                         
                         Int_t Bin3sipm=FitBin1sipm; 
                         Int_t FitBin2sipm;
                         FitBin2sipm=FitBin1sipm+12;
        		 ampsi -> Fit(funsi, "W", "S", (FitBin1sipm + 3.5) / 5., FitBin2sipm/5.);
			 //Float_t tcr_Si = (-threshold * (ped_Si - 2720) + ped_Si - funsi -> GetParameter(0)) / (funsi-> GetParameter(1));
			 Float_t tcr_Si = (-0.70 * (ped_Si - ampsi -> GetBinContent(FitBin2sipm)) + ped_Si - funsi -> GetParameter(0)) / (funsi-> GetParameter(1));
            		 if(verbose)cout<<"tcr_Si="<<tcr_Si<<"\t"<<"Event="<<i<<endl;
            		 //cout<<"-threshold * (ped_Si - ampsi -> GetBinContent(FitBin2sipm)) + ped_Si="<<-threshold * (ped_Si - ampsi -> GetBinContent(FitBin2sipm)) + ped_Si<<"\t"<<"Event="<<i<<endl;
            		 //cout<<"-threshold * (ped_Si - max_Si) + ped_Si="<<-threshold * (ped_Si - max_Si) + ped_Si<<"\t"<<"Event="<<i<<endl;
            		 if(verbose)cout<<"dt="<<tcr-tcr_Si<<"\t"<<"Event="<<i<<endl;

                         //======================

            	 	 if ( (xyf -> GetEntries() - nentries) == 1 ) nentries = xyf -> GetEntries();
             		 for (int i = 0; i < amp -> GetNbinsX(); i++) {
                		 ampavg -> Fill(amp -> GetBinCenter(i) - tcrtrg1+60., amp -> GetBinContent(i));
            		 }
            		 for (int i = 0; i < ampsi -> GetNbinsX(); i++) {
                		 ampsiavg -> Fill(ampsi -> GetBinCenter(i) - tcrtrg1+60., ampsi -> GetBinContent(i));
            		 }
            		 
                         //======================
 
                        //time between MCP and first Trg MCP
            		 ht -> Fill(tcr - tcrtrg1);          		 
                         //time between SIPM and first Trg MCP
			 htsi -> Fill(tcr_Si - tcrtrg1);

            	         //cout << "dT=" << (tcr - tcrtrg1) << endl;
            		 if ((tcr - tcrtrg1) < 15 && (tcr - tcrtrg1) > -2) {
                	 	dtvsxy -> Fill(xyev -> GetMean(1), xyev -> GetMean(2), tcr - tcrtrg1);
            	  	 }

                         //time between Trgs MCP
        		 TimeDistributionTriggers -> Fill(tcrtrg1 - tcrtrg2);
		         //time between MCP and SIPM
			 Time -> Fill(tcr - tcr_Si);
	                 if(name=="lysozero"){TimeCor -> Fill((tcr-tcr_Si)-(max_Si-ped_Si)*0.002001);}
	                 if(name=="lyso30plus"){TimeCor -> Fill((tcr-tcr_Si)-(max_Si-ped_Si)*0.001682);}
	                 if(name=="lyso30minus"){TimeCor -> Fill((tcr-tcr_Si)-(max_Si-ped_Si)*0.001057);}
       		   	 //in CanvGeom
   	    		 //TimeVSx -> Fill(xyev -> GetMean(1), tcr_Si - tcrtrg1);
	    		 //TimeVSy -> Fill(xyev -> GetMean(2), tcr_Si - tcrtrg1);
	 	         TimeVSx -> Fill(xyev -> GetMean(1), tcr-tcr_Si);
	    		 TimeVSy -> Fill(xyev -> GetMean(2), tcr-tcr_Si);
	                 //TimeVSAmplitude -> Fill(mintrg, tcrtrg1 - tcrtrg2);
	                 //TimeVSAmplitude -> Fill(max_Si-ped_Si, tcr_Si - tcrtrg1);
	                 //TimeVSAmplitude -> Fill(max_Si-ped_Si, tcr-tcr_Si);
	                 if(name=="lysozero"){TimeVSAmplitude -> Fill(max_Si-ped_Si, (tcr-tcr_Si)-(max_Si-ped_Si)*0.002001);}
	                 if(name=="lyso30plus"){TimeVSAmplitude -> Fill(max_Si-ped_Si, (tcr-tcr_Si)-(max_Si-ped_Si)*0.001682);}
	                 if(name=="lyso30minus"){TimeVSAmplitude -> Fill(max_Si-ped_Si, (tcr-tcr_Si)-(max_Si-ped_Si)*0.001057);}
                         //cout<<"max_Si="<<max_Si<<"\t"<<"tcr_Si - tcrtrg2="<<tcr_Si-tcrtrg2<<endl;
		 }
         }
    	 printf("%d\r",i); fflush(stdout);
         gSystem -> ProcessEvents();

         //system("read");
    }
      //Fit average for SiPM
      Int_t FitBin1sipm=0;
      Int_t j4=0;
      ampsiavg -> SetAxisRange(30, 45, "X");
      Float_t pedavg_Si = ampsiavg -> GetMean(2);
      ampsiavg -> SetAxisRange(30, 200, "X");
      Int_t maxbinsipm = ampsiavg -> GetMaximumBin();
      Float_t binContent = ampsiavg -> GetBinContent(maxbinsipm);
      while ( (pedavg_Si + 6) < binContent )
      {
        j4++;
        FitBin1sipm = maxbinsipm-j4;
        binContent = ampsiavg -> GetBinContent(FitBin1sipm);                                                  
      }                   
      Int_t Bin3sipm=FitBin1sipm; 
      Int_t FitBin2sipm;
      FitBin2sipm=FitBin1sipm+20;
      ampsiavg -> Fit(funsi, "W", "S", (FitBin1sipm - 0.5) / 5., FitBin2sipm/5.);

      //Fit average for MCP
      Int_t Bin2mcp=154.;
      for(Int_t Bin1mcp=150.; Bin1mcp<1024.; Bin1mcp++)
      { 
        Bin2mcp++;
        if(  (ampavg -> GetBinContent(Bin2mcp) / ampavg -> GetBinContent(Bin1mcp) ) <= 0.9890 )
	{
     	  break;
	} 
      }
      Int_t Bin3mcp=Bin2mcp;
      for(Bin3mcp; Bin3mcp<1024.; Bin3mcp++)
      {  
        Bin3mcp++;
        if( ampavg->GetBinContent(Bin3mcp) > ampavg->GetBinContent(Bin3mcp-1) || (ampavg->GetBinContent(Bin3mcp-1)-ampavg->GetBinContent(Bin3mcp))<5 )
        {
          break;
        }  			    
     }
     Int_t FitBin1mcp=Bin2mcp+1;
     Int_t FitBin2mcp=Bin3mcp-3.5; 
     ampavg-> Fit(fun, "W", "S", FitBin1mcp / 5., FitBin2mcp / 5.);

    //==========
 
    CanvAmp -> cd(2);
    amptrg1->GetXaxis()->SetTitle("Time, ns");
    amptrg1 -> Draw("");
    CanvAmp -> cd(3);
    amptrg2->GetXaxis()->SetTitle("Time, ns");
    amptrg2 -> Draw("");
    CanvAmp -> cd(4);
    amp->GetXaxis()->SetTitle("Time, ns");
    amp -> Draw("");
    //amp->GetXaxis()->SetRangeUser(225,350);
    amptrg2 -> SetLineColor(kBlue);
    amptrg2 -> Draw("same");
    CanvAmp -> cd(5);
    ampsi->GetXaxis()->SetTitle("Time, ns");
    ampsi -> Draw("");
    //ampsi->GetXaxis()->SetRangeUser(225,350);
    amptrg2 -> SetLineColor(kBlue);
    amptrg2 -> Draw("same");

    CanvAmp -> Update(); 
    //}
   //}
  //}

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
    mcpb1->SetLineWidth(2);
    mcpb2->SetLineWidth(2);
    mcpb1 -> Draw("same");
    mcpb2 -> Draw("same");
    CanvAmp -> Update();

    //Map of hits GEM
    CanvGeom -> cd(1);
    exbeamdata -> Draw("detClusters.y[2]:detClusters.x[2] >> xy1", Conditions_1,"");
    xy1 -> Draw("colz");  
    TLine* gem1b1 = new TLine(36, 14, 36, 28);
    TLine* gem1b2 = new TLine(48, 14, 48, 28);
    TLine* gem1b3 = new TLine(36, 14, 48, 14);
    TLine* gem1b4 = new TLine(36, 28, 48, 28);
    gem1b1->SetLineColor(kRed);
    gem1b2->SetLineColor(kRed);
    gem1b3->SetLineColor(kRed);
    gem1b4->SetLineColor(kRed);
    gem1b1->SetLineWidth(2);
    gem1b2->SetLineWidth(2);
    gem1b3->SetLineWidth(2);
    gem1b4->SetLineWidth(2);
    gem1b1 -> Draw("same");
    gem1b2 -> Draw("same");
    gem1b3 -> Draw("same");
    gem1b4 -> Draw("same");
    CanvGeom -> cd(2);

    exbeamdata -> Draw("detClusters.y[3]:detClusters.x[3] >> xy2", Conditions_1,"");
    xy2 -> Draw("colz");
    TLine* gem2b1 = new TLine(64, 10, 64, 28);
    TLine* gem2b2 = new TLine(76, 10, 76, 28);
    TLine* gem2b3 = new TLine(64, 10, 76, 10);
    TLine* gem2b4 = new TLine(64, 28, 76, 28);
    gem2b1->SetLineColor(kRed);
    gem2b2->SetLineColor(kRed);
    gem2b3->SetLineColor(kRed);
    gem2b4->SetLineColor(kRed);
    gem2b1->SetLineWidth(2);
    gem2b2->SetLineWidth(2);
    gem2b3->SetLineWidth(2);
    gem2b4->SetLineWidth(2);
    gem2b1 -> Draw("same");
    gem2b2 -> Draw("same");
    gem2b3 -> Draw("same");
    gem2b4 -> Draw("same");
    CanvGeom -> Update();

    CanvGeom -> cd(3);
    xys -> Draw("text, colz");
    xyf -> Draw("box, same");
    TLine* xysb1 = new TLine(41, 15, 41, 27);
    TLine* xysb2 = new TLine(44, 15, 44, 27);
    TLine* xysb3 = new TLine(41, 15, 44, 15);
    TLine* xysb4 = new TLine(41, 27, 44, 27);
    xysb1->SetLineColor(kRed);
    xysb2->SetLineColor(kRed);
    xysb3->SetLineColor(kRed);
    xysb4->SetLineColor(kRed);
    xysb1->SetLineWidth(2);
    xysb2->SetLineWidth(2);
    xysb3->SetLineWidth(2);
    xysb4->SetLineWidth(2);
    xysb1 -> Draw("same");
    xysb2 -> Draw("same");
    xysb3 -> Draw("same");
    xysb4 -> Draw("same");
 
    CanvAmp -> cd(6);
    ampavg->GetXaxis()->SetTitle("Time, ns");
    ampavg -> Draw("");
    CanvAmp -> cd(7);
    ampsiavg->GetXaxis()->SetTitle("Time, ns");
    ampsiavg -> Draw("");
    CanvAmp -> cd(8);
    exbeamdata -> Draw("daq.lecroy2249.ch0 >> hNaI");
    hNaI -> SetLineColor(2);
    hNaI -> Draw();
    TLine* NaIb1 = new TLine(800, 0, 800, 400);
    TLine* NaIb2 = new TLine(1700, 0, 1700, 400);
    NaIb1->SetLineWidth(2);
    NaIb2->SetLineWidth(2);
    NaIb1 -> Draw("same");
    NaIb2 -> Draw("same");
    CanvAmp -> Update();

    CanvGeom -> cd(4);    
    TimeVSx -> Draw("colz");
    CanvGeom -> cd(5);
    TimeVSy -> Draw("colz");
    CanvTimeDistribution -> cd(3);
    TimeDistributionTriggers -> Draw("");
    CanvGeom -> cd(6);
    TimeVSAmplitude -> Draw("colz");
    //TimeVSAmplitude -> Draw("prof");
    CanvGeom -> cd(7);
    TProfile *TimeVSAmplitudeX = TimeVSAmplitude->ProfileX("TimeVSAmplitudeX", 0, 2000);
    TimeVSAmplitudeX->Draw();

    CanvTimeDistribution -> cd(4);
    TF1* Gaus = new TF1("Gaus", "[0] * exp(-0.5*pow((x-[1])/[2],2))", -10, 10);
    Gaus -> SetParLimits(0, 1, 100);
    //Gaus -> SetParLimits(1, 5, 10);
    Gaus -> SetParLimits(1, -5, 5);
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

    CanvTimeDistribution -> cd(1);  
    ht -> Draw("");
    ht -> Fit(Gaus, "W", "S", -10, 10);

    CanvTimeDistribution -> cd(2);
    htsi -> Draw("");
    htsi -> Fit(Gaus, "W", "S", -10, 10);

    CanvTimeDistribution -> cd(5);
    avstmcp -> Draw("colz");
    CanvTimeDistribution -> cd(6);
    avstsipm -> Draw("colz");

    CanvTimeDistribution -> cd(7);
    TF1* Gaus2 = new TF1("Gaus2", "[0] * exp(-0.5*pow((x-[1])/[2],2))", -10, 10);
    Gaus2 -> SetParLimits(0, 1, 500);
    Gaus2 -> SetParLimits(1, -5, 5);
    Gaus2 -> SetParLimits(2, 0.1, 1);
    Gaus2 -> SetParameter(0, 1);
    Gaus2 -> SetParameter(1, TimeCor -> GetMean());
    Gaus2 -> SetParameter(2, 0.1);
    TimeCor -> Draw("");
    Gaus2 -> SetLineColor(kRed);
    TimeCor -> Fit(Gaus2, "W", "S", -10, 10); 

    CanvAmp -> Update();
    CanvGeom -> Update();
    CanvTimeDistribution -> Update(); 
    
    CanvTimeDistribution -> SaveAs(outDir + name + "(" + y_boundLeft + "-" + y_boundRight  + ")_CanvTimeDistribution.root");
    CanvAmp -> SaveAs(outDir + name + "(" + y_boundLeft + "-" + y_boundRight  + ")_CanvAmp.root");
    CanvGeom -> SaveAs(outDir + name + "(" + y_boundLeft + "-" + y_boundRight  + ")_CanvGeom.root");
}
