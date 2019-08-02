#ifndef PDPCFarichMonitoring_H
# define PDPCFarichMonitoring_H 1

# include "Rtypes.h"
# include "TString.h"
# include "PDPCFarichVFrameProcessing.h"

class PDPCFarichFrameData;
class PDPCFarichRecoEvent;
class PDPCFarichHitData;
class PDPCFarichGeometry;
class TApplication;
class TFile;
class TH1F;
class TH2F;
class TTree;
class TBranch;
class TCanvas;
class TPad;
class TGraph;
class TEllipse;
class TMarker;
class TClonesArray;

class PDPCFarichMonitoring : public PDPCFarichVFrameProcessing
{
public:
    PDPCFarichMonitoring(PDPCFarichGeometry* f);
    ~PDPCFarichMonitoring()
    {
    }

public:
    void Initialize();
    void Terminate();

    void ProcessFrame(PDPCFarichFrameData* frame);

    void SetOutputFilename(const char *name)
    {
        filename = name;
    }
    void StoreEmptyEvents(Bool_t flag)
    {
        storeEmptyEvents = flag;
    }
    void StoreRawHits(Bool_t flag)
    {
        storeRawHits = flag;
    }
    void EnableDrawing(Bool_t flag,Float_t interval=1.)
    {
	enableDrawing = flag;
	drawingInterval = interval;
    }
    void EnablePrintingEventDisplay(Bool_t flag)
    {
	enablePrintingEventDisplay = flag;
    }
    void SetEventDisplayPrintDirectory(const char* dirname)
    {
        edPrintDirectory = dirname;
    }
    void SetGeometry(PDPCFarichGeometry* f)
    {
        fGeometry = f;
    }
    void SetTimingLimit(Int_t i)
    {
        timingLimit = i<0?-i:i;
    }

protected:
    void BuildTree();
    void BuildHistograms();

private:
    static const Double_t tdcBin_ns;

    PDPCFarichGeometry *fGeometry;

    TString filename;
    TFile *file;

    Bool_t storeEmptyEvents;
    Bool_t storeRawHits;
    Bool_t enableDrawing;
    Bool_t enablePrintingEventDisplay;
    Float_t drawingInterval; //sec
    Int_t timingLimit; //TDC ticks

    PDPCFarichRecoEvent* event;
    TClonesArray* rawHits;
    TTree* outTree;
    TBranch* rawHitsBranch;
    TBranch* eventBranch;
    TH2F* hPixelOccupancy;
    TH2F* hDieOccupancy;
    TH1F* hDieAmplitude1;
    TH2F* hDieAmplitude2;
    TH1F* hPixelAmplitude1;
    TH2F* hPixelAmplitude2;
    TH1F* hDiePixelCount;
    TH1F* hPixelCount;
    TH1F* hCellCount;
    TH1F* hAllTimes;
    TH2F* hRingCenter;
    TH1F* hRadius;
    TH1F* hTime;
    TH1F* hNlogLH;
    TCanvas* canvas;
    TGraph* gSelHits;
    TGraph* gUnselHits;
    TH1F* hEventTimes;
    TEllipse* ring;
    TMarker* ringCenter;
    TString edPrintDirectory;
    TApplication *app;

    ULong64_t nevents;
};
#endif
