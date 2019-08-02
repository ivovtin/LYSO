#ifndef PDPCFarichRootReader_H
# define PDPCFarichRootReader_H 1

# include "PDPCFarichVDataReader.h"

class PDPCFarichGeometry;
class PDPCFarichHitData;
class PDPCFarichFrameData;
class TFile;
class TTree;
class TClonesArray;

class PDPCFarichRootReader : public PDPCFarichVDataReader
{
public:
    PDPCFarichRootReader(const char* fn="");
    ~PDPCFarichRootReader();

public:
    void Open();
    void Close();
    ULong_t SkipFrames(ULong_t nframes);
    void ProcessFrame(PDPCFarichFrameData*);

public:
    void SetGeometry(PDPCFarichGeometry* f)
    {
        fGeometry = f;
    }

    void SetTreeName(const char* name) { treeName = name; }

private:
    const char* treeName;

    static const UInt_t nHitsToPrint = 1000;

    TFile* file;
    TTree *tree;
    PDPCFarichGeometry *fGeometry;
    TClonesArray *pdpcRawData;
    UInt_t triggerCount;

    UInt_t nevents_read;
};
#endif
