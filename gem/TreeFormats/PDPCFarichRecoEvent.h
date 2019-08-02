#ifndef PDPCFarichRecoEvent_H
# define PDPCFarichRecoEvent_H 1

# include "TObject.h"
# include "TClonesArray.h"
# include <vector>

class PDPCFarichRecoHit : public TObject
{
public:
    PDPCFarichRecoHit();
    ~PDPCFarichRecoHit()
    {
    }

public:
    void UseForFit(Bool_t b=kTRUE)
    {
        fitThis = b;
    }
    void SetCentral(Bool_t b=kTRUE)
    {
        central = b;
    }
    void SetTarget(Int_t i)
    {
        target = i;
    }
    void SetDie(Int_t i)
    {
        die = i;
    }
    void SetPixel(Int_t i)
    {
        pixel = i;
    }
    void SetPixelSerialID(Int_t i)
    {
        pixelSID = i;
    }
    void SetXYindex(Int_t i,Int_t j)
    {
        xindex = i;
        yindex = j;
    }
    void SetPosition(Float_t _x,Float_t _y)
    {
        x = _x;
        y = _y;
    }
    void SetCellCount(UInt_t i)
    {
        count = i;
    }
    void SetTime(UInt_t i)
    {
        time = i;
    }

public:
    void Print(Option_t *opt="") const;

    Int_t Compare(const TObject* right) const;

    Bool_t IsSortable() const { return kTRUE; }

public:
    Bool_t UsedForFit() const
    {
        return fitThis;
    }
    Bool_t IsCentral() const
    {
        return central;
    }
    Int_t GetTarget() const
    {
        return target;
    }
    Int_t GetDie() const
    {
        return die;
    }
    Int_t GetPixel() const
    {
        return pixel;
    }
    Int_t GetPixelSerialID() const
    {
        return pixelSID;
    }
    Int_t GetXindex() const
    {
        return xindex;
    }
    Int_t GetYindex() const
    {
        return yindex;
    }
    Float_t GetXposition() const
    {
        return x;
    }
    Float_t GetYposition() const
    {
        return y;
    }
    UInt_t GetCellCount() const
    {
	return count;
    }
    Int_t GetTime() const
    {
        return time;
    }
private:
    Bool_t fitThis;
    Bool_t central;
    Int_t target;
    Int_t die;
    Int_t pixel;
    Int_t pixelSID;
    Int_t xindex;
    Int_t yindex;
    Float_t x;
    Float_t y;
    UInt_t count;
    Int_t time;

private:
    ClassDef(PDPCFarichRecoHit, 4)

};

bool operator<(const PDPCFarichRecoHit& l, const PDPCFarichRecoHit& r);

class PDPCFarichRecoEvent : public TObject
{
public:
    PDPCFarichRecoEvent();
    ~PDPCFarichRecoEvent();

public:
    PDPCFarichRecoHit* AddHit();

    Bool_t Estimate();

    void Print(Option_t* opt="") const;

    void Clear(Option_t* opt="")
    {
        hits->Delete();
    }

    void Copy(PDPCFarichRecoEvent&) const;

    void ResetFit();

public:
    void SetNumber(UInt_t i)
    {
        number = i;
    }
    void SetSync(Int_t i)
    {
        sync = i;
    }
    void SetFrame(UInt_t i)
    {
        frame = i;
    }
    void SetRadius(Float_t r)
    {
        radius = r;
    }
    void SetCenterPosition(Float_t x, Float_t y)
    {
        x0=x;
        y0=y;
    }
    void SetTime(Double_t d)
    {
        time = d;
    }
    void SetCentralTime(Double_t d)
    {
        ctime = d;
    }
    void SetSelectedHits(UInt_t i)
    {
        nsel = i;
    }
    void SetFitted(Bool_t b)
    {
        fitted = b;
    }
    void SetNlogLH(Float_t f)
    {
        nLogLH = f;
    }
    void SetConsistency(Float_t f)
    {
        consistency = f;
    }
    void SetNpe(Float_t f)
    {
        npe = f;
    }
    void SetSigmaRadius(Float_t f)
    {
        sigmaRadius = f;
    }
    void SetSigmaTime(Float_t f)
    {
        sigmaTime = f;
    }

public:
    Int_t GetNumber() const { return number; }
    Int_t GetSync() const { return sync; }
    UInt_t GetFrame() const { return frame; }
    Int_t GetNhits() const { return nhits; }
    Int_t GetNselectedHits() const { return nsel; }
    Bool_t IsFitted() const { return fitted; }
    Float_t GetRadius() const { return radius; }
    Float_t GetXcenter() const { return x0; }
    Float_t GetYcenter() const { return y0; }
    Float_t GetTime() const { return time; }
    Float_t GetCentralTime() const { return ctime; }
    Float_t GetNlogLH() const { return nLogLH; }
    Float_t GetConsistency() const { return consistency; }
    Float_t GetNpe() const { return npe; }
    Float_t GetSigmaRadius() const { return sigmaRadius; }
    Float_t GetSigmaTime() const { return sigmaTime; }
    PDPCFarichRecoHit* GetHit(UInt_t i) const { return (PDPCFarichRecoHit*)(i<nhits?hits->At(i):0); }
    TClonesArray* GetHitArray() const { return hits; }

public:
    static int GetNactiveEvents() { return nActiveEvents; }

    static void Reset();

private:
    UInt_t number;
    Int_t sync;
    UInt_t frame;        //32-bit frame number (PDPCFarichHitData::frameSequence)
    UInt_t nhits;
    UInt_t nsel;
    TClonesArray *hits; //->

    Bool_t fitted;       //is ring fittes
    Float_t radius;      //ring radius
    Float_t x0, y0;      //center of ring position
    Double_t time;       //mean time of selected hits
    Double_t ctime;      //mean time of central hits (if central area is specified)
    Float_t nLogLH;      //-log(LH) at minimum
    Float_t consistency; //log(LH)-log(LH_bkg) at minimum
    Float_t npe;         //mean number of photoelectrons per event (externally set)
    Float_t sigmaRadius; //sigma of radius per photon (externally set)
    Float_t sigmaTime;   //sigma of time distribution per photon (externally set)

    static std::vector<TClonesArray*> vgHits;
    static int nActiveEvents;

private:
    ClassDef(PDPCFarichRecoEvent, 8)
};

#endif
