#ifndef GEMGEOMETRY_H_
#define GEMGEOMETRY_H_

#include <iosfwd>

namespace gem {

class Geometry {
public:
    enum { NDET=12 }; //Number of detectors
    static const float HALF_X_SIZE_THICK, HALF_Y_SIZE_THICK; //thick GEM detector half dimensions, mm
    static const float HALF_X_SIZE_THIN, HALF_Y_SIZE_THIN; //thin GEM detector half dimensions, mm

private:
    size_t _ndet;   //0..NDET == number of detector in file
    bool _defined[NDET]; //if detector is defined
    float _x[NDET], _y[NDET], _z[NDET]; //GEM detectors center positions, mm
    float _phi[NDET]; //GEM detectors phi rotation angles, rad

public:
    Geometry() :
        _ndet(0)
    {
    }

    Int_t readConfiguration(const char* cfgfn);
    void transformToLabFrame(size_t index, TVector3& pos);

    size_t numdet() const {
        return _ndet;
    }
    bool defined(size_t i) const {
        return i<NDET?_defined[i]:false;
    }
    int index(size_t k) const {
        if(k>=NDET) return -1;
        size_t _k = 0;
        for(size_t i=0; i<NDET; i++){
            if(_defined[i]) {
                if(k==_k) return i;
                _k++;
            }
        }
        return -1;
    }
    float x(size_t i) const {
        return i<NDET?_x[i]:0.;
    }
    float y(size_t i) const {
        return i<NDET?_y[i]:0.;
    }
    float z(size_t i) const {
        return i<NDET?_z[i]:0.;
    }
    float phi(size_t i) const {
        return i<NDET?_phi[i]:0.;
    }

    void setnumdet(size_t ndet) {
        _ndet = ndet;
    }
    void setdefined(size_t i,bool d) {
        if(i < NDET){
            _defined[i] = d;
        }
    }
    void setx(size_t i,float x) {
        if(i < NDET){
            _x[i] = x;
        }
    }
    void sety(size_t i,float y) {
        if(i < NDET){
            _y[i] = y;
        }
    }
    void setz(size_t i,float z) {
        if(i < NDET){
            _z[i] = z;
        }
    }
    void setphi(size_t i,float phi) {
        if(i < NDET){
            _phi[i] = phi;
        }
    }

};

std::ostream & operator << (std::ostream & ostr, const Geometry & geom);

}

#endif
