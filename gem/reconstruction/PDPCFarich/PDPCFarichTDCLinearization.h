
#ifndef PDPCFarichTDCLinearization
# define PDPCFarichTDCLinearization_H 1

# include "PDPCFarichVFrameProcessing.h"

class PDPCFarichFrameData;

class PDPCFarichTDCLinearization : public PDPCFarichVFrameProcessing
{

public:
    PDPCFarichTDCLinearization() : PDPCFarichVFrameProcessing()
    {}
    ~PDPCFarichTDCLinearization()
    {}

public:
    void Initialize();
    void ProcessFrame(PDPCFarichFrameData* frame);

};
#endif
