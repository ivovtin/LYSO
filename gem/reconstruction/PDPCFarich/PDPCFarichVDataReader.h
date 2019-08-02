
#ifndef PDPCFarichVDataReader_H
#define PDPCFarichVDataReader_H 1

# include "Rtypes.h"

class PDPCFarichHitData;
class PDPCFarichFrameData;

class PDPCFarichVDataReader
{

public:
    PDPCFarichVDataReader(const char* s="") : filename(s), verbose(kFALSE),
	flag_eof(kFALSE), flag_error(kFALSE) {}
    virtual ~PDPCFarichVDataReader();

public:
    virtual void Open() = 0;
    virtual void Close() = 0;
    virtual ULong_t SkipFrames(ULong_t) = 0;
    virtual void ProcessFrame(PDPCFarichFrameData*) = 0;

public:
    Bool_t IsError() const
    {
	return flag_error;
    }
    Bool_t IsComplete() const
    {
	return flag_eof || flag_error;
    }
    const char* GetFilename()
    {
	return filename;
    }

    void SetFilename(const char* s)
    {
	filename = s;
    }
    void SetVerbose(Bool_t b)
    {
	verbose = b;
    }

protected:
    const char *filename;
    Bool_t verbose;
    Bool_t flag_eof, flag_error;
};
#endif
