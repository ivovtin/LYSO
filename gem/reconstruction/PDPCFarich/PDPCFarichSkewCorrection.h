#ifndef PDPCFarichSkewCorrection
# define PDPCFarichSkewCorrection_H 1

# include "PDPCFarichVFrameProcessing.h"
# include <vector>

class PDPCFarichGeometry;
class PDPCFarichFrameData;

class PDPCFarichSkewCorrection : public PDPCFarichVFrameProcessing
{

public:
    PDPCFarichSkewCorrection(PDPCFarichGeometry* f, const char* fn="");
    ~PDPCFarichSkewCorrection();

public:
    void Initialize();

    void ProcessFrame(PDPCFarichFrameData* frame);

public:
    void SetGeometry(PDPCFarichGeometry* f)
    {
        fGeometry = f;
    }
    void SetFilename(const char* fn)
    {
	filename = fn;
    }

private:
    PDPCFarichGeometry* fGeometry;
    const char* filename;
    std::vector<Int_t> skewData;
};
#endif
