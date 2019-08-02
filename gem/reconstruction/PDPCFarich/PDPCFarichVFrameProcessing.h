
#ifndef PDPCFarichVFrameProcessing_H
# define PDPCFarichVFrameProcessing_H 1

# include "Rtypes.h"

class PDPCFarichFrameData;

class PDPCFarichVFrameProcessing
{

public:

    PDPCFarichVFrameProcessing() : verbose(kFALSE), initialized(kFALSE) {}
    virtual ~PDPCFarichVFrameProcessing();

public:
    void SetVerbose(Bool_t b) { verbose = b; }

    virtual void Initialize()
    {
	initialized = kTRUE;
    }
    virtual void Terminate()
    {
	initialized = kFALSE;
    }

    virtual void ProcessFrame(PDPCFarichFrameData*) = 0;

protected:
    Bool_t verbose;
    Bool_t initialized;
};
#endif
