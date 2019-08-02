#ifndef CLUSTER_H_
#define CLUSTER_H_
#include <vector>
#include <numeric> // std::accumulate
#include <cmath>

#include <boost/optional.hpp>

#include "TreeFormats/GemEvent.h"

namespace gem {
enum DetectorType {THICK_DET = 0, THIN_DET = 1};
enum {STRIP_NUM_THICK = 256, STRIP_NUM_THIN = 320}; // A number of straight strips in detector
enum ClusterType {NORMAL_CLS = 0, EDGE_CLS = 1, ONESTRIP_CLS = 2, OVERLOAD_CLS = 3}; //cluster types: 0 - normal, 1 - maximum on edge, 2 - one strip, 3 - amplitude overload

bool lessByAbs(Short_t a1, Short_t a2);

class LinearCluster {
	int _type; //cluster type
	short _max_strip_amp;
	float _amplitude;
	float _mean; // weighted position
	float _width;
	unsigned _begin, _end;
public:
	int type() const { return _type; }
	short max_strip_amplitude() const {return _max_strip_amp;}
	float amplitude() const {return _amplitude;}
	float width() const {return _width;}
	float mean() const {return _mean;}
	unsigned begin() const {return _begin;}
	unsigned end() const {return _end;}
	//end point to first channel after the last channel of cluster
	LinearCluster(unsigned begin, unsigned end, unsigned begin_av, unsigned end_av, unsigned maximum, const Short_t * amplitudes);
};

class Clusterization {
public:
	typedef LinearCluster Cluster;
	enum {MAX_CLUSTER_COUNT = 5, MIN_CLUSTER_AMPLITUDE = 10, EDGE_CUT = 36};
	static float gRelThreshold;
	static unsigned gNchannelAveraging;
	static unsigned short gAmpOverload;

private:
	bool _straight;
	size_t _nch; //number of straight strips

	unsigned findFront(const Short_t *  amplitudes, unsigned start, int direction, float threshold) const;
	Cluster extractCluster(const Short_t *  amplitudes, size_t start) const;
 
	static void setRelativeThreshold(float f) { gRelThreshold = f; }
	static void setNchannelAveraging(int i) { gNchannelAveraging = i; }

public:
	size_t channelCount() const {
		return _nch;
	}
	std::vector<Cluster> getClusters(const Short_t *  amplitudes) const;

	Clusterization(bool straight, size_t n) : _straight(straight), _nch(n)
	{}
};

class PlaneCluster {
	LinearCluster _xCluster;
	boost::optional<LinearCluster> _yCluster;
	DetectorType _type;
public:
	//Returns negative number if coordinates do not correspond
	static float yChannelToMMThick(float xInChannels, float yInChannels) {
		float delta = xInChannels - yInChannels + 1;
		if (  0.0 <= delta && delta <=  8.0 ) return  0.0 + (delta -  0.0) * 5.0;
		if (  8.0 <  delta && delta <= 28.0 ) return 40.0 + (delta -  8.0) * 1.0;
		if ( 28.0 <  delta && delta <= 36.0 ) return 60.0 + (delta - 28.0) * 5.0;
		return -1;
	}
	static float yChannelToMMThin(float xInChannels, float yInChannels) {
		float delta = xInChannels - yInChannels + 1;
		if ( delta<=40 )
			return delta;
		else
			return -1;
	}
	LinearCluster straight() const {
		return _xCluster;
	}
	boost::optional<LinearCluster> stereo() const {
		return _yCluster;
	}
	float amplitudeX() const {
		return fabs(_xCluster.amplitude());
	}
	float amplitudeY() const {
		return fabs(_yCluster ? _yCluster->amplitude() : 0);
	}
	//Detector index is not filled here
	void save(GemCluster & oCluster) const;
	PlaneCluster(const LinearCluster & x, const LinearCluster & y, DetectorType type):
		_xCluster(x),
		_yCluster(y),
		_type(type)
	{}
	PlaneCluster(const LinearCluster & x):
		_xCluster(x)
	{}
};

}

#endif /* CLUSTER_H_ */
