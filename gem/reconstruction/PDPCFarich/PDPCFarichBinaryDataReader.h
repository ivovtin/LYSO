#ifndef PDPCFarichBinaryDataReader_H
# define PDPCFarichBinaryDataReader_H 1

# include "PDPCFarichVDataReader.h"
# include "Rtypes.h"
# include <stdint.h>

class PDPCFarichGeometry;
class PDPCFarichHitData;
class PDPCFarichFrameData;

class PDPCFarichBinaryDataReader : public PDPCFarichVDataReader
{
private:
    enum {
        CoincidenceMode=0,
	AllHitsMode=1
    };
    enum {
	FirstCoincidenceHeader = 0xFF,
	NextCoincidenceHeader = 0xFE
    };

public:
    PDPCFarichBinaryDataReader(const char* fn="");
    ~PDPCFarichBinaryDataReader();

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

public:
    Int_t IsCoincidenceMode() const
    {
	return binary_file_mode==CoincidenceMode;
    }
    UInt_t GetHitSize() const
    {
	return hit_size;
    }
    UInt_t GetNhitsInFile() const
    {
	return file_size/hit_size;
    }

private:
    ULong_t ReadChunk();

    void SetCoincidenceMode(Bool_t b=kTRUE)
    {
	if( b ) {
	    binary_file_mode = CoincidenceMode;
	    hit_size = 23;
	} else {
	    binary_file_mode = AllHitsMode;
	    hit_size = 22;
	}
    }

private:
    static const UInt_t nHitsToPrint = 100000;

    FILE *file;
    PDPCFarichGeometry *fGeometry;

    Int_t binary_file_mode;
    size_t hit_size;
    UInt_t nhits_chunk;
    UInt_t nhits_total_read;
    UInt_t last_frame_seq_read;
    UInt_t last_frame_read;
    Int_t  last_sync_read;
    Int_t sync0;
    Int_t nsync_rollovers;

    unsigned char *buffer1;
    size_t position;
    size_t buffer_size;
    size_t file_position;
    size_t file_size;
};
#endif
