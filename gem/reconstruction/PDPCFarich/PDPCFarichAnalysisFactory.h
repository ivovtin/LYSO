#ifndef PDPCFarichAnalysisFactory_H
# define PDPCFarichAnalysisFactory_H 1

# include "Rtypes.h"

class PDPCFarichVDataReader;
class PDPCFarichFrameData;
class PDPCFarichVFrameProcessing;
class PDPCFarichGeometry;

class PDPCFarichAnalysisFactory
{
public:
    PDPCFarichAnalysisFactory();
    ~PDPCFarichAnalysisFactory();

public:
    void Initialize();
    void Terminate();

    ULong64_t Process(ULong64_t start=0, ULong64_t nframes=0);

public:
    void SetInputDataReader(PDPCFarichVDataReader* f)
    {
        fReader = f;
    }
    void SetGeometry(PDPCFarichGeometry* f)
    {
        fGeometry = f;
    }
    void SetTDCLinearization(PDPCFarichVFrameProcessing* f)
    {
        fTDCLinearization = f;
    }
    void SetSkewCorrection(PDPCFarichVFrameProcessing* f)
    {
        fSkewCorrection = f;
    }
    void SetEventConstruction(PDPCFarichVFrameProcessing* f)
    {
        fEventConstruction = f;
    }
    void SetMonitor(PDPCFarichVFrameProcessing* f)
    {
        fMonitor = f;
    }

private:
    Bool_t initialized;
    PDPCFarichGeometry *fGeometry;
    PDPCFarichVDataReader *fReader;
    PDPCFarichVFrameProcessing *fTDCLinearization;
    PDPCFarichVFrameProcessing *fSkewCorrection;
    PDPCFarichVFrameProcessing *fEventConstruction;
    PDPCFarichVFrameProcessing *fMonitor;

    ULong64_t nframes_reconstructed;
    ULong64_t nhits_reconstructed;
};
#endif
