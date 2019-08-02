#ifndef PDPCFarichHitData_H
# define PDPCFarichHitData_H 1

# include "TObject.h"

class PDPCFarichHitData : public TObject
{
public:
    PDPCFarichHitData();
    ~PDPCFarichHitData()
    {
    }

public:
    void SetFirstHit(Bool_t b)
    {
	firstHit = b;
    }
    void SetDieSerialID(Int_t i)
    {
        dieSID = i;
    }
    void SetDie(Int_t i)
    {
        die = i;
    }
    void SetTile(Int_t i)
    {
        tile = i;
    }
    void SetModule(Int_t i)
    {
        module = i;
    }
    void SetBusboard(Int_t i)
    {
        busboard = i;
    }
    void SetTarget(Int_t i)
    {
        target = i;
    }
    void SetTileUID(Int_t i)
    {
        tileUID = i;
    }
    void SetTimestamp(Int_t i)
    {
        timestamp = i;
    }
    void SetFrame(Int_t i)
    {
        frame = i;
    }
    void SetFrameSequence(UInt_t i)
    {
        frameSequence = i;
    }
    void SetSync(Int_t i)
    {
        sync = i;
    }
    void SetCellCount(Int_t j,Int_t i)
    {
        cellCount[j] = i;
    }
    void SetXYindex(Int_t i,Int_t j)
    {
        xindex = i;
        yindex = j;
    }

public:
    void Init();

    void Print(Option_t *opt="");

    Int_t Compare(const TObject* right) const;

    Bool_t IsSortable() const { return kTRUE; }

public:

    Bool_t IsFirstHit() const
    {
        return firstHit;
    }
    Int_t GetDieSerialID() const
    {
        return dieSID;
    }
    Int_t GetDie() const
    {
        return die;
    }
    Int_t GetTile() const
    {
        return tile;
    }
    Int_t GetModule() const
    {
        return module;
    }
    Int_t GetBusboard() const
    {
        return busboard;
    }
    Int_t GetTarget() const
    {
        return target;
    }
    Int_t GetTileUID() const
    {
        return tileUID;
    }
    Int_t GetTimestamp() const
    {
        return timestamp;
    }
    Int_t GetFrame() const
    {
        return frame;
    }
    UInt_t GetFrameSequence() const
    {
        return frameSequence;
    }
    Int_t GetSync() const
    {
        return sync;
    }
    Int_t GetCellCount(Int_t j=-1) const
    {
        if( j<0 ) 
            return cellCount[0]+cellCount[1]+cellCount[2]+cellCount[3];
        if( j>3 ) 
            return 0;
        return cellCount[j];
    }
    Int_t GetXindex() const
    {
        return xindex;
    }
    Int_t GetYindex() const
    {
        return yindex;
    }

private:
    Bool_t firstHit;
    Int_t dieSID, die, tile, module, busboard, target, tileUID;
    Int_t timestamp;
    Int_t frame;
    UInt_t frameSequence;
    Int_t sync;
    Int_t cellCount[4]; //pixel cell counts
    Int_t xindex; //die x-index
    Int_t yindex; //die y-index

private:
    ClassDef(PDPCFarichHitData, 2)
};
#endif
