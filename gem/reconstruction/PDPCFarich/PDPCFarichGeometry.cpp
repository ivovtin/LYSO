#include "PDPCFarichGeometry.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>
#include <cassert>
#include <algorithm>
#include "TString.h"
#include "TRegexp.h"

ClassImp(PDPCFarichGeometry);

using namespace std;


PDPCFarichGeometry::Geometry::Geometry(UInt_t n) :
    ntotal(n), nx(0), ny(0),
    positionX(ntotal), positionY(ntotal), rotationPhi(ntotal),
    indexX(ntotal), indexY(ntotal)
{
}

PDPCFarichGeometry::Geometry::Geometry(UInt_t _nx, Float_t px, Float_t xsize, UInt_t _ny, Float_t py, Float_t ysize, Bool_t xminor, Bool_t snake)
{
    Set(_nx, px, xsize, _ny, py, ysize, xminor, snake);
}

PDPCFarichGeometry::Geometry::~Geometry()
{
    Reset();
}

void PDPCFarichGeometry::Geometry::Set(UInt_t n)
{
    ntotal = n;
    positionX.resize(ntotal);
    positionY.resize(ntotal);
    rotationPhi.resize(ntotal);
    indexX.resize(ntotal);
    indexY.resize(ntotal);
}

void PDPCFarichGeometry::Geometry::Set(UInt_t _nx, UInt_t _ny)
{
    nx = _nx;
    ny = _ny;
    Set(nx*ny);
}

void PDPCFarichGeometry::Geometry::
Set(UInt_t _nx, Float_t px, Float_t xsize, UInt_t _ny, Float_t py, Float_t ysize, Bool_t xminor, Bool_t snake)
{
    nx = _nx;
    ny = _ny;
    ntotal = nx*ny;
#ifdef DEBUG
    cout<<"Nx="<<nx<<" Ny="<<ny<<" Ntotal="<<ntotal<<endl;
#endif
    positionX.resize(ntotal);
    positionY.resize(ntotal);
    rotationPhi.resize(ntotal);
    indexX.resize(ntotal);
    indexY.resize(ntotal);
    elmXsize=xsize;
    elmYsize=ysize;

    Float_t y = -py*0.5*(ny-1);
    for(UInt_t j=0; j<ny; j++, y+=py) {
        Float_t x = -px*0.5*(nx-1);
	for(UInt_t i=0; i<nx; i++, x+=px) {
            UInt_t k = xminor?(j*nx+i):(j+i*ny); //if xminor, first increment x index, then y index
            if( k>=ntotal ) throw;
            Bool_t turnx = xminor&&snake&&(j%2==1);
            Bool_t turny = !xminor&&snake&&(i%2==1);
            positionX[k] = turnx?-x:x;
	    positionY[k] = turny?-y:y;
            rotationPhi[k] = 0.;
            if( px>0 && !turnx || px<0 && turnx )
                indexX[k] = i;
            else
                indexX[k] = nx-1-i;
            if( py>0 && !turny || py<0 && turny )
                indexY[k] = j;
            else
                indexY[k] = ny-1-j;
	}
    }
}

void PDPCFarichGeometry::Geometry::Reset()
{
    ntotal = 0;
    nx = ny = 0;
    positionX.clear();
    positionY.clear();
    rotationPhi.clear();
    indexX.clear();
    indexY.clear();
}

void PDPCFarichGeometry::Geometry::Include(const Geometry& subGeom)
{
    Geometry old(*this);

    Set(old.ntotal*subGeom.ntotal); //increase capacity
    nx = old.nx*subGeom.nx;
    ny = old.ny*subGeom.ny;
    elmXsize = subGeom.elmXsize;
    elmYsize = subGeom.elmYsize;

    UInt_t i=0;
    for(UInt_t iold=0; iold<old.ntotal; iold++) {
        Geometry sub(subGeom);
        sub.Rotate(old.rotationPhi[iold]); //rotation must go before translation
        sub.Translate(old.positionX[iold],old.positionY[iold]);
        for(UInt_t isub=0; isub<sub.ntotal; isub++) {
             positionX[i] = sub.positionX[isub];
             positionY[i] = sub.positionY[isub];
             rotationPhi[i] = sub.rotationPhi[isub];
             indexX[i] = old.indexX[iold]*sub.nx+sub.indexX[isub];
             indexY[i] = old.indexY[iold]*sub.ny+sub.indexY[isub];
             i++;
        }
    }
}

void PDPCFarichGeometry::Geometry::Translate(Float_t dx, Float_t dy)
{
    for(UInt_t k=0; k<ntotal; k++) {
	positionX[k] += dx;
	positionY[k] += dy;
    }
}

void PDPCFarichGeometry::Geometry::Rotate(Float_t dphi)
{
    if( dphi==0. ) return; //nothing to do
    for(UInt_t k=0; k<ntotal; k++) {
        Float_t x = positionX[k], y = positionY[k];
        Float_t c = cos(dphi), s = sin(dphi);
	positionX[k] = x*c-y*s;
        positionY[k] = x*s+y*c;
        rotationPhi[k] += dphi;
        if( nx>0 && ny>0 ) {
            if( s>c && s>=-c ) {
                indexX[k] = ny-1-indexY[k];
                indexY[k] = indexX[k];
                swap(nx,ny);
            } else if( c<=s && c<-s ) {
                indexX[k] = nx-1-indexX[k];
                indexY[k] = ny-1-indexY[k];
            } else if( s<c && s<=-c ) {
                indexX[k] = indexY[k];
                indexY[k] = nx-1-indexX[k];
                swap(nx,ny);
            }
        }
    }
}

void PDPCFarichGeometry::Geometry::Rotate(Int_t nq)
{
    if( nq<0 ) nq+=4*(-nq/4+1);
    if( nq%4==0 ) return; //nothing to do
    for(UInt_t k=0; k<ntotal; k++) {
	Float_t x = positionX[k], y = positionY[k];
	Int_t c = (1-2*((nq/2)%2))*(1-nq%2), s = (1-2*((nq/2)%2))*(nq%2);
	positionX[k] = x*c+y*s;
	positionY[k] = x*c+y*s;
        rotationPhi[k] += nq*M_PI_2;
        if( nx>0 && ny>0 ) {
            if( nq%4==1 ) {
                indexX[k] = ny-1-indexY[k];
                indexY[k] = indexX[k];
                swap(nx,ny);
            } else if( nq%4==2 ) {
                indexX[k] = nx-1-indexX[k];
                indexY[k] = ny-1-indexY[k];
            } else if( nq%4==3 ) {
                indexX[k] = indexY[k];
                indexY[k] = nx-1-indexX[k];
                swap(nx,ny);
            }
        }
    }
}

Int_t PDPCFarichGeometry::Geometry::Locate(Float_t x, Float_t y, UInt_t start, UInt_t len) const
{
    UInt_t end = len==0 ? ntotal : start+len;
    for(UInt_t k=start; k<end; k++) {
        Float_t dx = x-positionX[k], dy = y-positionY[k], phi = rotationPhi[k];
        Float_t lx = dx*cos(phi)+dy*sin(phi);
        Float_t ly = dy*cos(phi)-dx*sin(phi);
        if( fabs(lx)<=0.5*elmXsize && fabs(ly)<=0.5*elmYsize ) {
            return k;
        }
    }
    return -1;
}

Int_t PDPCFarichGeometry::Geometry::Locate(UInt_t ix,UInt_t iy) const
{
    for(UInt_t k=0; k<ntotal; k++) {
        if( indexX[k]==ix && indexY[k]==iy ) return k;
    }
    return -1;
}

Bool_t PDPCFarichGeometry::Geometry::EqualTo(const Geometry& g) const
{
    if( ntotal!=g.ntotal || nx!=g.nx || ny!=g.ny ) return kFALSE;

    if( elmXsize!=g.elmXsize || elmYsize!=g.elmYsize ) return kFALSE;

    if(!(positionX==g.positionX) || !(positionY==g.positionY) ||
       !(rotationPhi==g.rotationPhi) ||
       !(indexX==g.indexX) || !(indexY==g.indexY)) return kFALSE;

    return kTRUE;
}

const UInt_t PDPCFarichGeometry::nCellsPerPixel = 3200;
const UInt_t PDPCFarichGeometry::nxCellsPerPixel = 50;
const UInt_t PDPCFarichGeometry::nyCellsPerPixel = 64;
const UInt_t PDPCFarichGeometry::nPixelsPerDie = 4;
const UInt_t PDPCFarichGeometry::nxPixelsPerDie = 2;
const UInt_t PDPCFarichGeometry::nDiesPerTile = 16;
const UInt_t PDPCFarichGeometry::nxDiesPerTile = 4;
const UInt_t PDPCFarichGeometry::nPixelsPerTile = 64;
const UInt_t PDPCFarichGeometry::nxPixelsPerTile = 8;
const UInt_t PDPCFarichGeometry::nxTiles = 6;
const UInt_t PDPCFarichGeometry::nxTotalDies = 24;
const UInt_t PDPCFarichGeometry::nxTotalPixels = 48;
const UInt_t PDPCFarichGeometry::nxTotalCells = 2400;
const UInt_t PDPCFarichGeometry::nyTotalCells = 3072;
const Float_t PDPCFarichGeometry::cellSizeX = 0.05378; //cell size from the mean fill factor
const Float_t PDPCFarichGeometry::cellSizeY = 0.05378; //cell size from the mean fill factor
const Float_t PDPCFarichGeometry::pixelSizeX = 3.2;
//const Float_t PDPCFarichGeometry::pixelSizeY = 3.8775; //TEK manual v0.21
const Float_t PDPCFarichGeometry::pixelSizeY = 3.8016; //T.Frach's data
const Float_t PDPCFarichGeometry::cellPitchX = 0.064;
const Float_t PDPCFarichGeometry::cellPitchY = 0.0594;
//const Float_t PDPCFarichGeometry::pixelPitchX = 3.35; //TEK manual v0.21
//const Float_t PDPCFarichGeometry::pixelPitchY = 3.9975; //TEK manual v0.21
const Float_t PDPCFarichGeometry::pixelPitchX = 3.3408; //T.Frach's data
const Float_t PDPCFarichGeometry::pixelPitchY = 3.91856; //T.Frach's data
const Float_t PDPCFarichGeometry::dieSizeX = 7.15;
const Float_t PDPCFarichGeometry::dieSizeY = 7.875;
const Float_t PDPCFarichGeometry::diePitchX = 7.95;
const Float_t PDPCFarichGeometry::diePitchY = 7.975;
const Float_t PDPCFarichGeometry::tileSizeX = 32.6;
const Float_t PDPCFarichGeometry::tileSizeY = 32.6;

PDPCFarichGeometry::PDPCFarichGeometry(const char* tilesgeomfn, Bool_t v) :
    verbose(v), initialized(kFALSE),
    tileGeometryFilename(tilesgeomfn),
    nTotalTiles(0), nTotalDies(0), nTotalPixels(0),
    //Tile's geometry (dies' positions) is very accurate and regular so it can be determined by die pitch only.
    // Die numbering is increasing from left to right and from top to bottom
    tileOfDies(nxDiesPerTile,diePitchX,dieSizeX,nxDiesPerTile,-diePitchY,dieSizeY),
    //Die's geometry is also very accurate and regular.
    //Pixels are numbered in a snake order starting from right to left and from top to bottom.
    dieOfPixels(nxPixelsPerDie,-pixelPitchX,pixelSizeX,nxPixelsPerDie,-pixelPitchY,pixelSizeY,kTRUE,kTRUE),
    //Exact cell layout in a pixel is unknown by the moment.
    //Distribute them evenly from left to right and from top to bottom.
    pixelOfCells(nxCellsPerPixel,cellPitchX,cellSizeX,nyCellsPerPixel,-cellPitchY,cellSizeY)
{
    if( tilesgeomfn && *tilesgeomfn )
        Initialize();
}

void PDPCFarichGeometry::Initialize()
{
    cout << "Defining detector's geometry..." << endl;

    tileMap.clear();

    nTotalTiles = 0;
    nTotalDies = 0;
    nTotalPixels = 0;
    nTotalCells = 0;

    initialized = kFALSE;

    cout << "Reading targets and tile positions from " << tileGeometryFilename << "..." << endl;

    //Read tile geometry from a data file
    ifstream in(tileGeometryFilename);
    if( !in.is_open() ) {
        cerr << "PDPCFarichGeometry::Initialize(): Can not open geometry file "
             << tileGeometryFilename << endl;
	return;
    }

    Int_t target, tileUID, pos;
    Float_t x, y, phi;
    Int_t line = 0;

    ostringstream msgs;
    in.exceptions ( ios::failbit | ios::badbit );
    try {
        //Skip first two lines as it's comments
        for(Int_t l=0; l<2; l++) {
            in.ignore(numeric_limits<streamsize>::max(),'\n');
        }

        while( 1 ) {
            in >> pos >> hex >> target >> dec >> tileUID >> x >> y >> phi;
            line++;
            if( verbose )
                cout << " " << pos << "\t" << hex << target << dec << "\t"
                     << tileUID << "\t" << x << "\t" << y << "\t" << phi << endl;

            if( pos%10<1 || pos%10>nxTiles || pos/10<1 || pos/10>nxTiles ) {
                msgs.str(string());
                msgs << "Wrong positional code " << pos << ". Must be yindex*10+xindex, [xy]index is from 1 to 6.";
                throw ios::failure(msgs.str());
            }
            //map is sorted in ascending order by target
            tileMap[target] = Tile(tileUID,pos,x,y,phi);
        }
    } //try
    catch(ios::failure& e) {
        if( !in.eof() ) {
            cerr << tileGeometryFilename << ": error reading line " << line << ": " << e.what() << endl;
            in.close();
            return;
        }
    }

    in.close();

    nTotalTiles = tileMap.size();
    nTotalDies = nTotalTiles*nDiesPerTile;
    nTotalPixels = nTotalDies*nPixelsPerDie;
    nTotalCells = nTotalPixels*nCellsPerPixel;

    //Detector geometry may be not square
    detectorOfTiles.Set(nTotalTiles);
    detectorOfTiles.SetNxy(nxTiles,nxTiles);

    map<Int_t,Tile>::const_iterator first=tileMap.begin(), last=tileMap.end();
    for(UInt_t itile=0; first!=last; first++, itile++) {
	Int_t target = first->first;
	const Tile &aTile = first->second;

        detectorOfTiles.SetPosition(itile,aTile.X(),aTile.Y());
        detectorOfTiles.SetRotation(itile,aTile.Phi()+(target%2==0?M_PI:0)); //add orientation of the tile in a module
        detectorOfTiles.SetIndexXY(itile,aTile.PosCode()%10-1,aTile.PosCode()/10-1);
    }

    //Get die positions in the detector
    detectorOfDies=detectorOfTiles;
    detectorOfDies.Include(tileOfDies);

    //Get pixel positions in the detector
    detectorOfPixels=detectorOfDies;
    detectorOfPixels.Include(dieOfPixels);

    //Get cell positions in the detector
    detectorOfCells=detectorOfPixels;
    detectorOfCells.Include(pixelOfCells);

    //Checks for consistency
    if( detectorOfDies.GetN()!=nTotalDies ) throw;
    if( detectorOfPixels.GetN()!=nTotalPixels ) throw;
    if( detectorOfCells.GetN()!=nTotalCells ) throw;

#ifdef DEBUG
    Print();

    cerr << "DEBUG: Storing pixel geometry in pixel_real_geometry.dat, pixel_int_geometry.dat..." << endl;
    Print("pixel file:pixel_real_geometry.dat");
    Print("pixel integer file:pixel_int_geometry.dat");

    cerr << "DEBUG: Storing die geometry in die_real_geometry.dat, die_int_geometry.dat..." << endl;
    Print("die file:die_real_geometry.dat");
    Print("die integer file:die_int_geometry.dat");
#endif

    initialized = kTRUE;
}

Int_t PDPCFarichGeometry::GetTileUID(UInt_t target) const
{
    map<Int_t,Tile>::const_iterator where = tileMap.find(target);
    if( where==tileMap.end() ) {
	cerr << "PDPCFarichGeometry::GetTileUID(): Can not find specified target " << hex << target << dec << endl;
	return -1;
    }
    return where->second.UID();
}

Int_t PDPCFarichGeometry::GetTileUID(Int_t bb, Int_t m, Int_t t) const
{
    return GetTileUID(TranslateTarget(bb,m,t));
}

Int_t PDPCFarichGeometry::FindTarget(UInt_t uid) const
{
    map<Int_t,Tile>::const_iterator first=tileMap.begin(), last=tileMap.end();
    for( ; first!=last; first++)
        if( first->second.UID()==uid ) return first->first;
    return -1;
}

Int_t PDPCFarichGeometry::GetDieSerialID(Int_t target, Int_t d) const
{
    map<Int_t,Tile>::const_iterator where = tileMap.find(target);
    if( where==tileMap.end() ) {
        cerr << hex << target << dec << ": Target not found" << endl;
	return -1;
    }

    if( d<0 || d>15 ) {
        cerr << d << ": Illegal die index in tile" << endl;
	return -1;
    }

    Int_t dieSID = distance(tileMap.begin(),where)*nDiesPerTile + d;

    if( dieSID>=nTotalDies ) throw;

    return dieSID;
}

Int_t PDPCFarichGeometry::GetPixelSerialID(Int_t target, Int_t d, Int_t p) const
{
    if( p<0 || p>3 ) {
        cerr << p << ": Illegal pixel index in die" << endl;
	return -1;
    }

    Int_t dieSID = GetDieSerialID(target,d);
    if( dieSID<0 ) return -1;

    Int_t pixelSID = dieSID*nPixelsPerDie + p;

    if( pixelSID>=nTotalPixels ) throw;

    return pixelSID;
}

Int_t PDPCFarichGeometry::GetCellSerialID(Int_t target, Int_t d, Int_t col, Int_t row) const
{
    if( row<0 || row>=2*nxCellsPerPixel ) {
        cerr << row << ": Illegal cell row" << endl;
	return -1;
    }
    if( col<0 || col>=2*nyCellsPerPixel ) {
        cerr << col << ": Illegal cell column" << endl;
        return -1;
    }

    Int_t pxInDie = 3*(col/nyCellsPerPixel)+(1-2*(col/nyCellsPerPixel))*(1-row/nxCellsPerPixel);
    Int_t cellInPixel = (col%nyCellsPerPixel)*nxCellsPerPixel+row%nxCellsPerPixel;

    Int_t pxSID = GetPixelSerialID(target,d,pxInDie);
    if( pxSID<0 ) return -1;

    Int_t cellSID = pxSID*nCellsPerPixel+cellInPixel;

    if( cellSID>=nTotalCells ) throw;

    return cellSID;
}

Bool_t PDPCFarichGeometry::GetAddressesByPixelSID(Int_t pixelSID, Int_t &target, Int_t& d, Int_t& p) const
{
    if( pixelSID<0 || pixelSID>=nTotalPixels )
        return kFALSE;

    p = pixelSID%nPixelsPerDie;
    d = (pixelSID/nPixelsPerDie)%nDiesPerTile;
    Int_t it = pixelSID/nPixelsPerTile;
    map<Int_t,Tile>::const_iterator where = tileMap.begin();
    advance(where,it);
    target = where->first;
}

Int_t PDPCFarichGeometry::LocatePixel(Float_t x, Float_t y) const
{
    Int_t tileID;
    if( (tileID=detectorOfTiles.Locate(x,y))<0 ) return -1;
    Int_t dieSID;
    if( (dieSID=detectorOfDies.Locate(x,y,tileID*nDiesPerTile,nDiesPerTile))<0 ) return -1;
    Int_t pixelSID;
    if( (pixelSID=detectorOfPixels.Locate(x,y,dieSID*nPixelsPerDie,nPixelsPerDie))<0 ) return -1;

    return pixelSID;
}

void PDPCFarichGeometry::Print(Option_t* option) const
{
    TString sopt = option;

    std::streambuf *coutbuf = cout.rdbuf();
    ofstream fout;

    Ssiz_t p, l;
    if( (p=sopt.Index(TRegexp("file:[^ ]+"),&l,0))>=0 ) {
        TString outfn=sopt(p+5,l-5);
        if( !outfn.IsNull() ) {
            fout.open(outfn);
            if( fout.is_open() ) {
                cout << "Writing geometry data to " << outfn << " ..." << endl;
                cout.rdbuf(fout.rdbuf());
            }
        }
    }

    cout << "PDPCFarichGeometry: " << tileGeometryFilename << "\n";

    if( sopt.Contains("die",TString::kIgnoreCase) ) { //print dies
        cout << nTotalDies << " dies defined\n";
        if( sopt.Contains("integer",TString::kIgnoreCase) ) { //print integer geometry
            for(size_t i=0; i<nTotalDies; i++) {
                cout << GetDieIndexX(i) << " " << GetDieIndexY(i) << endl;
            }
        } else {
            for(size_t i=0; i<nTotalDies; i++) {
                cout << fixed << setprecision(2) << setw(7) << GetDiePositionX(i) << " " << GetDiePositionY(i) << endl;
            }
        }
    } else if( sopt.Contains("pixel",TString::kIgnoreCase) ) { //print pixels
        cout << nTotalPixels << " pixels defined\n";
        if( sopt.Contains("integer",TString::kIgnoreCase) ) { //print integer geometry
            for(size_t i=0; i<nTotalPixels; i++) {
                cout << GetPixelIndexX(i) << " " << GetPixelIndexY(i) << endl;
            }
        } else {
            for(size_t i=0; i<nTotalPixels; i++) {
                cout << fixed << setprecision(2) << setw(6) << GetPixelPositionX(i) << " " << GetPixelPositionY(i) << endl;
            }
        }
    } else {
        cout << nTotalTiles << " tiles defined\n";
        map<Int_t,Tile>::const_iterator first = tileMap.begin(), last = tileMap.end();
        for(Int_t itile=0; first!=last; first++, itile++) {
            Int_t target = first->first;
            const Tile& aTile = first->second;
            cout << " " << setw(2) << aTile.PosCode() << "  " << hex << setw(4) << target << dec << "  "
                << setw(4) << aTile.UID() << "  " << fixed << setprecision(2) << setw(7) << aTile.X() << " " << aTile.Y() << " "
                << setprecision(4) << aTile.Phi() << "\n";
        }
    }

    if( fout.is_open() ) {
        fout.close();
        cout.rdbuf(coutbuf);
    }
}