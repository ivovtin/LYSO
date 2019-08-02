#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <TVector3.h>
#include "GemGeometry.h"

using namespace std;

namespace gem {

const float Geometry::HALF_X_SIZE_THICK = 64., Geometry::HALF_Y_SIZE_THICK = 50.; //GEM detector half dimensions, mm
const float Geometry::HALF_X_SIZE_THIN = 80., Geometry::HALF_Y_SIZE_THIN = 20.; //GEM detector half dimensions, mm

int Geometry::readConfiguration(const char* cfgfn){
    ifstream in(cfgfn);
    if( !in.is_open() ) {
        cerr << "Can not open GEM geometry data file " << cfgfn << "\n";
        return -1;
    }

    for(size_t i=0; i<NDET; i++) {
        _defined[i] = false;
        _x[i] = 0.;
        _y[i] = 0.;
        _z[i] = 0.;
        _phi[i] = 0.;
    }
    _ndet = 0;

    in.exceptions ( ios::failbit | ios::badbit );
    try {
        //Skip first two lines as it's comments
        for(int l=0; l<2; l++) {
            in.ignore(numeric_limits<streamsize>::max(),'\n');
        }

        size_t i;
        float x, y, z, phi;
        for(size_t k=0; k<NDET; k++) {
            in >> i >> x >> y >> z >> phi;
            if(i < NDET) {
                _defined[i] = true;
                _x[i] = x;
                _y[i] = y;
                _z[i] = z;
                _phi[i] = phi;
                _ndet++;
            } else {
                cerr << "Skip illegal detector index " << i << endl;
            }
        }
    } catch( ifstream::failure& e ) {
        if( !in.eof() ) {
        cerr << "Error reading " << cfgfn << ": " << e.what() << "\n";
            in.close();
            return -1;
        }
    } catch( string s ) {
        cerr << cfgfn << ": " << s << "\n";
        in.close();
        return -1;
    }
    in.close();

    cout << "GEM geometry successfully read from " << cfgfn << "\n";

    return _ndet;
}

void Geometry::transformToLabFrame(size_t i, ::TVector3 &pos)
{
    if(i>=NDET) {
        cerr << "Geometry::transformToLabFrame(): Illegal detector index " << i << endl;
        return;
    }
    if(!_defined[i]) {
        cerr << "Geometry::transformToLabFrame(): Undefined detector index " << i << endl;
        return;
    }

    if(i<10){
        pos.SetXYZ(pos.X()-HALF_X_SIZE_THICK, pos.Y()-HALF_Y_SIZE_THICK, 0.);
        pos.RotateZ(_phi[i]);
        pos.SetXYZ(pos.X()+_x[i], pos.Y()+_y[i], _z[i]);
    }

    if(i>=10){
        pos.SetXYZ(pos.X()-HALF_X_SIZE_THIN, pos.Y()-HALF_Y_SIZE_THIN, 0.);
        pos.RotateZ(_phi[i]);
        pos.SetXYZ(pos.X()+_x[i], pos.Y()+_y[i], _z[i]);
    }
}

ostream & operator << (ostream & ostr, const Geometry & g)
{
    streamsize w = ostr.width();
    streamsize p = ostr.precision();
    ostr.setf(ios::fixed,ios::floatfield);
    ostr << "GEM geometry configuration\n"
         << "det  x[mm]  y[mm]  z[mm]  phi[rad]\n";
    for(size_t i=0; i<Geometry::NDET; i++) {
    if( g.defined(i) ) {
            ostr << " " << setw(2) << i
                 << "  " << setprecision(2) << setw(5) << g.x(i) << "   " << setw(5) << g.y(i)
                 << "   " << setprecision(0) << setw(5) << g.z(i)
                 << "   " << setprecision(4) << setw(7) << g.phi(i) << "\n";
        }
    }
    ostr.unsetf(ios::floatfield);
    ostr.width(w);
    ostr.precision(p);
}

} //namespace gem
