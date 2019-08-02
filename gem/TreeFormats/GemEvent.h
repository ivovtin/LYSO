#ifndef GemEvent_H__
#define GemEvent_H__
#include <vector>
#include <ostream>

#include <Rtypes.h>
#include <TObject.h>

struct GemEvent {
	enum {EVENT_SIZE = 512 * 10 + 4*16*640};
	Int_t hitCount;
	Short_t eventIndex;
	Short_t amplitudes[EVENT_SIZE];
	GemEvent() {
		reset();
	}
	void reset() {
		hitCount = -1;
		eventIndex = -1;
		for (int j = 0; j <  GemEvent::EVENT_SIZE; ++j	) {
			amplitudes[j] = 0;
		}
	}
};


//A cluster in detector coordinates
class GemCluster: public TObject {
public:
	Short_t detector; //detector index starting from 0
	Short_t typex, typey; //cluster type: 0 - normal, 1 - maximum on edge, 2 - one strip, 3 - amplitude overload
	Float_t x, y; //coordinates of clusters
	Float_t dx, dy; //width of clusters
	Float_t ax, ay; //amplitudes of clusters
	Float_t max, may; //maximum strip amplitudes in clusters
	Float_t rawStraight, rawStereo; //mean of clusters position in channel number
	GemCluster():
		detector(-1),
		typex(-1),
		typey(-1),
		x(-1),
		y(-1),
		dx(-1),
		dy(-1),
		ax(0),
		ay(0),
		max(0),
		may(0),
		rawStraight(-1),
		rawStereo(-1)
	{}
	bool isValid() const {
		return typex>=0 && x >= 0 && ax > 0 && detector >=0 && dx > 0;
	}
	ClassDefNV(GemCluster, 5)
};


inline std::ostream & operator<< (std::ostream & ostr, const GemCluster & cluster) {
	return ostr << "detector=" << cluster.detector << ", x=" << cluster.x << ", y="<< cluster.y << ", ax=" << cluster.ax << ", ay=" << cluster.ay;
}

class GemTrack : public TObject
{
public:
    enum { NDET = 4 };

    Bool_t valid;
    Float_t vx, vy, vz; //track direction, unit vector
    Float_t x0, y0, z0; //point on track
    Float_t dx[NDET], dy[NDET]; //deviation of clusters from track

    GemTrack():
        valid(kFALSE),
        vx(0), vy(0), vz(0),
        x0(0), y0(0), z0(0)
    {}

    ClassDefNV(GemTrack,1)
};

#endif
