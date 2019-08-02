#ifndef PDPCFarichEventConstruction_H
# define PDPCFarichEventConstruction_H 1

# include "Rtypes.h"
# include "PDPCFarichVFrameProcessing.h"

class PDPCFarichFrameData;
class PDPCFarichGeometry;
namespace ROOT { namespace Math { class Minimizer; } }
class PDPCFarichLhFunc;
class PDPCFarichRecoEvent;

class PDPCFarichEventConstruction : public PDPCFarichVFrameProcessing
{
public:
    PDPCFarichEventConstruction(PDPCFarichGeometry *f, const char* cfgfn) :
	PDPCFarichVFrameProcessing(), fGeometry(f), fMinimizer(0), lhFunc(0),
        calFilename(""), timeWindow(200), nHitsFitThreshold(3), doFit(kTRUE), skewCorrected(kFALSE), 
        beamAerogel(kFALSE), useCentralArea(kFALSE), x0min(-50.), y0min(-50.), x0max(50.), y0max(50.), step(8.), radius_est(56.),
        centralTarget(0), coincidenceWindow(0)
    {
        ReadConfigFile(cfgfn);
    }
    ~PDPCFarichEventConstruction() {}

public:
    void Initialize();
    void Terminate();

    void ProcessFrame(PDPCFarichFrameData*);

    void SetGeometry(PDPCFarichGeometry* f)
    {
        fGeometry = f;
    }
    void SetCalFilename(const char* s)
    {
        calFilename = s;
    }
    void SetTimeWindow(Int_t tw)
    {
        timeWindow = tw;
    }
    void SetFitThreshold(UInt_t i)
    {
        nHitsFitThreshold = i;
    }
    void DoFit(Bool_t b=kTRUE)
    {
        doFit = b;
    }
    void SetSkewCorrected(Bool_t b=kTRUE)
    {
        skewCorrected = b;
    }
    void SetCenterWindow(Float_t x1,Float_t x2,Float_t y1,Float_t y2)
    {
    	x0min = x1;
    	x0max = x2;
    	y0min = y1;
    	y0max = y2;
    }
    void SetCenterStep(Float_t f)
    {
        step = f;
    }
    void SetCentralTarget(Int_t i)
    {
        centralTarget = i;
    }
    void SetRadiusEstimate(Float_t r)
    {
        radius_est = r;
    }
    void SetCoincidenceWindow(Int_t i)
    {
        coincidenceWindow = i;
    }
    void SetUseCentralArea(Bool_t i)
    {
        useCentralArea = i;
    }

private:
    void ReadConfigFile(const char *filename);

    void Reinitialize();

    void CoarseSearch(PDPCFarichRecoEvent*,Float_t,Float_t,Float_t,Float_t,Float_t,Float_t);

    Bool_t Fit(PDPCFarichRecoEvent*, Option_t* opt="");

private:
    PDPCFarichGeometry *fGeometry;
    ROOT::Math::Minimizer *fMinimizer;
    PDPCFarichLhFunc *lhFunc;
    const char* calFilename; //calibration file name with efficiency and DCR data per pixel
    Int_t timeWindow; //narrow time window for beam event search, TDC ticks
    UInt_t nHitsFitThreshold; //minimum number of hits per event to fit
    Bool_t doFit; //do ring fitting
    Bool_t skewCorrected; //skew is corrected (refit with fine timing resolution)
    Bool_t beamAerogel; //a small tile of aerogel installed in the central area in front of the detector
    Bool_t useCentralArea; //if set exclude hits in the rectangular area given by (x0min..x0max,y0min..y0max) from fit, use this area to determine timing
    Float_t x0min, y0min; //lower ranges of ring center scan, mm
    Float_t x0max, y0max; //upper ranges of ring center scan, mm
    Float_t step; //ring center scan step, mm
    Float_t radius_est; //estimated ring radius
    Int_t centralTarget; //central tile (one on beam line) target
    Int_t coincidenceWindow; //coincidence window for separating different syncs, TDC ticks

    ULong64_t event_seq_number, events_triedtofit, events_fitted, last_reinit_event;
    UInt_t nreinit;
};
#endif
