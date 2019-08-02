#ifndef PDPCFarichGeometry_H
# define PDPCFarichGeometry_H 1

# include <cmath>
# include <map>
# include <vector>
# include "TObject.h"
# include "TString.h"

class PDPCFarichGeometry : public TObject
{
public:
    class Geometry
    {
    public:
        //Default constructor
        Geometry() : ntotal(0), nx(0), ny(0) {}

        //General constructor
        Geometry(UInt_t n);

        //Constructor for rectangular geometry
        Geometry(UInt_t _nx, Float_t px, Float_t xsize, UInt_t _ny, Float_t py, Float_t ysize, Bool_t xminor=kTRUE, Bool_t snake=kFALSE);

        ~Geometry();

        void Set(UInt_t n); //allocate space
        void Set(UInt_t _nx, UInt_t _ny); //allocate space for rectangular geometry
        void Set(UInt_t _nx, Float_t px, Float_t xsize, UInt_t _ny, Float_t py, Float_t ysize, Bool_t xminor=kTRUE, Bool_t snake=kFALSE);
        void Reset(); //free space

        void SetNxy(UInt_t _nx,UInt_t _ny) { nx=_nx; ny=_ny;}
        void SetPositionX(UInt_t i, Float_t x) { if( i<positionX.size() ) positionX[i] = x; }
        void SetPositionY(UInt_t i, Float_t y) { if( i<positionY.size() ) positionY[i] = y; }
        void SetPosition(UInt_t i, Float_t x, Float_t y) { if( i<ntotal ) { positionX[i] = x; positionY[i] = y; } }
        void SetRotation(UInt_t i, Float_t phi) { if( i<rotationPhi.size() ) rotationPhi[i] = phi; }
        void SetIndexX(UInt_t i, UInt_t ix) { if( i<indexX.size() ) indexX[i] = ix; }
        void SetIndexY(UInt_t i, UInt_t iy) { if( i<indexY.size() ) indexY[i] = iy; }
        void SetIndexXY(UInt_t i, UInt_t ix, UInt_t iy) { if( i<ntotal ) { indexX[i] = ix; indexY[i] = iy; } }

        //Redefine the geometry with inclusion of a sub geometry
        void Include(const Geometry& subGeom);

        //Translate coordinates by (dx,dy)
        void Translate(Float_t dx, Float_t dy);
        //Rotate coordinates w.r.t. (0,0) counter-clockwise by dphi [rad]
        void Rotate(Float_t dphi);
        //Rotate coordinates w.r.t. (0,0) counter-clockwise by nq quarters of 2*PI
        void Rotate(Int_t nq);

        //Locate the element that contains (x,y).
        //Return a found element index or -1 if the point is outside of any element.
        //Start search from start element and go len elements. If len==0 check up to the end.
        Int_t Locate(Float_t x,Float_t y,UInt_t start=0,UInt_t len=0) const;

        //Locate the element with indices (ix,iy). Return -1 if not found.
        Int_t Locate(UInt_t ix,UInt_t iy) const;

        //Compare with another geometry and return true if equal
        Bool_t EqualTo(const Geometry& g) const;

        UInt_t  GetN() const { return ntotal; }
        Float_t GetPositionX(UInt_t i) const { return i<positionX.size()?positionX[i]:NAN; }
        Float_t GetPositionY(UInt_t i) const { return i<positionY.size()?positionY[i]:NAN; }
        Float_t GetRotation(UInt_t i) const { return i<rotationPhi.size()?rotationPhi[i]:NAN; }
        Int_t   GetIndexX(UInt_t i) const { return i<indexX.size()?indexX[i]:-1; }
        Int_t   GetIndexY(UInt_t i) const { return i<indexY.size()?indexY[i]:-1; }

    private:
        UInt_t ntotal; //total number of elements
        UInt_t nx, ny; //number of elements on global X and Y, can change with rotation
        std::vector<Float_t> positionX, positionY, rotationPhi; //position and rotation of every element
        std::vector<UInt_t> indexX, indexY; //indices along global X and Y, can change with rotation
        Float_t elmXsize, elmYsize; //rectangular element dimensions (never rotated)

        ClassDef(PDPCFarichGeometry::Geometry,1)
    };

    class Tile
    {
    public:
        Tile() : uid(0), pos(0), x(0.), y(0.), phi(0.) {}
        Tile(Int_t u, Int_t p, Float_t _x, Float_t _y, Float_t _phi) :
            uid(u), pos(p),
            x(_x), y(_y), phi(_phi) {}
        ~Tile() {}

        Int_t UID() const     { return uid; }
        Int_t PosCode() const { return pos; }
        Float_t X() const     { return x; }
        Float_t Y() const     { return y; }
        Float_t Phi() const   { return phi; }

        void UID(Int_t u)      { uid=u; }
        void PosCode(Int_t p)  { pos=p; }
        void X(Float_t _x)     { x=_x; }
        void Y(Float_t _y)     { y=_y; }
        void Phi(Float_t _phi) { phi=_phi; }

    private:
        Int_t uid; //tile's unique (serial) ID
        Int_t pos; //yindex*10+xindex. Possible values: [xy]index=1..6
        Float_t x, y, phi; //position (mm), counter-clockwise rotation (rad)

        ClassDef(PDPCFarichGeometry::Tile,1)
    };

public:
    PDPCFarichGeometry(const char* tilesgeomfn=0, Bool_t v=kFALSE);

    ~PDPCFarichGeometry()
    {}

public:
    void Initialize();

    void SetGeomFilename(const char* tilesgeomfn) { tileGeometryFilename=tilesgeomfn; }
    void SetVerbose(Bool_t b) { verbose = b; }

    void Print(Option_t* option = "") const;

public:
    UInt_t GetNpixels() { return nTotalPixels; }
    UInt_t GetNdies() { return nTotalDies; }
    UInt_t GetNtiles() { return nTotalTiles; }
    UInt_t GetNcells() { return nTotalCells; }

    Bool_t IsInitialized() const { return initialized; }
    Int_t GetTileUID(UInt_t target) const;
    Int_t GetTileUID(Int_t bb, Int_t m, Int_t t) const;
    Int_t FindTarget(UInt_t uid) const;
    Int_t GetDieSerialID(Int_t target, Int_t d) const;
    Int_t GetPixelSerialID(Int_t target, Int_t d, Int_t p) const;
    Int_t GetCellSerialID(Int_t target, Int_t d, Int_t col, Int_t row) const;
    Float_t GetDiePositionX(UInt_t i) const { return detectorOfDies.GetPositionX(i); }
    Float_t GetDiePositionY(UInt_t i) const { return detectorOfDies.GetPositionY(i); }
    Int_t GetDieIndexX(UInt_t i) const { return detectorOfDies.GetIndexX(i); }
    Int_t GetDieIndexY(UInt_t i) const { return detectorOfDies.GetIndexY(i); }
    Float_t GetPixelPositionX(UInt_t i) const { return detectorOfPixels.GetPositionX(i); }
    Float_t GetPixelPositionY(UInt_t i) const { return detectorOfPixels.GetPositionY(i); }
    Int_t GetPixelIndexX(UInt_t i) const { return detectorOfPixels.GetIndexX(i); }
    Int_t GetPixelIndexY(UInt_t i) const { return detectorOfPixels.GetIndexY(i); }
    Float_t GetCellPositionX(UInt_t i) const { return detectorOfCells.GetPositionX(i); }
    Float_t GetCellPositionY(UInt_t i) const { return detectorOfCells.GetPositionY(i); }
    Int_t GetCellIndexX(UInt_t i) const { return detectorOfCells.GetIndexX(i); }
    Int_t GetCellIndexY(UInt_t i) const { return detectorOfCells.GetIndexY(i); }
    Int_t GetPixelByCell(UInt_t cellSID) const { return cellSID<nTotalCells?cellSID/nCellsPerPixel:-1; }
    Bool_t GetAddressesByPixelSID(Int_t pixelSID, Int_t &target, Int_t& d, Int_t& p) const;

    //Find the pixel by point and return its index. If point is out of any pixel active area -1 is returned.
    Int_t LocatePixel(Float_t x, Float_t y) const;

    static Int_t TranslateTarget(Int_t bb, Int_t m, Int_t t) { return ((bb&0xF)<<12)|((m&0xFF)<<4)|(t&0xF); }
    static Int_t TranslateBusboardID(Int_t target) { return (target&0xF000)>>12; }
    static Int_t TranslateModuleID(Int_t target) { return (target&0xFF0)>>4; }
    static Int_t TranslateTileID(Int_t target) { return (target&0xF); }

private:
    void DefineIntGeometry();

public:
    static const UInt_t nCellsPerPixel;
    static const UInt_t nxCellsPerPixel;
    static const UInt_t nyCellsPerPixel;
    static const UInt_t nxPixelsPerDie;
    static const UInt_t nxDiesPerTile;
    static const UInt_t nxPixelsPerTile;
    static const UInt_t nxTiles;
    static const UInt_t nxTotalDies;
    static const UInt_t nxTotalPixels;
    static const UInt_t nxTotalCells;
    static const UInt_t nyTotalCells;
    static const UInt_t nPixelsPerDie;
    static const UInt_t nDiesPerTile;
    static const UInt_t nPixelsPerTile;

    static const Float_t cellSizeX, cellSizeY;
    static const Float_t cellPitchX, cellPitchY;
    static const Float_t pixelSizeX, pixelSizeY;
    static const Float_t pixelPitchX, pixelPitchY;
    static const Float_t dieSizeX, dieSizeY;
    static const Float_t diePitchX, diePitchY;
    static const Float_t tileSizeX, tileSizeY;

private:
    Bool_t verbose;
    Bool_t initialized;
    TString tileGeometryFilename;
    std::map<Int_t,Tile> tileMap;

    //number of detector units configured
    UInt_t nTotalTiles;
    UInt_t nTotalDies;
    UInt_t nTotalPixels;
    UInt_t nTotalCells;

    Geometry tileOfDies;
    Geometry dieOfPixels;
    Geometry pixelOfCells;
    Geometry detectorOfTiles;
    Geometry detectorOfDies;
    Geometry detectorOfPixels;
    Geometry detectorOfCells;

    ClassDef(PDPCFarichGeometry,4)
};

#endif
