#ifndef GEMDETECTOR_H_
#define GEMDETECTOR_H_

#include <vector>
#include <cassert>
#include "cluster.h"

namespace gem {

class Detector {
private:
	size_t _index; //0..9 - thick detectors, >=10 - thin detectors

public:
    // GEM constructor
	// index - a position of detector in event structure
	// intersections a number of straight strips intersections with a single inclined one
	Detector(size_t index) : _index(index)
	{}
	~Detector()
	{}
	size_t index() const {
		return _index;
	}
	
	virtual size_t channelCount() const = 0;

	virtual std::vector<PlaneCluster> buildLocalClusters(GemEvent & event, std::vector<LinearCluster> & unboundStereoClusters) const = 0;
};

// Thick GEM constructor
class ThickDetector : public Detector {
private:
	Clusterization _clusterizationX;
	Clusterization _clusterizationY;

public:
	// index - a position of detector in event structure (0..9)
	ThickDetector(size_t index) : Detector(index),
		_clusterizationX(true, STRIP_NUM_THICK),
		_clusterizationY(false, STRIP_NUM_THICK)
	{
	    assert(index<10);
	}
	~ThickDetector()
	{
	}
	size_t channelCount() const {
        return STRIP_NUM_THICK;
    }

	std::vector<PlaneCluster> buildLocalClusters(GemEvent & event, std::vector<LinearCluster> & unboundStereoClusters) const;
};

// Thin GEM constructor
class ThinDetector : public Detector {
private:
    size_t _frame;
	Clusterization _clusterizationX;
	Clusterization _clusterizationY;
	mutable Short_t _amp_x[STRIP_NUM_THIN], _amp_y[STRIP_NUM_THIN];

public:
    // Thin GEM constructor
	// index - a position of detector in event structure 
	ThinDetector(size_t index, size_t frame) : Detector(index),
		_frame(frame),
		_clusterizationX(true, STRIP_NUM_THIN),
		_clusterizationY(false, STRIP_NUM_THIN)
	{
	    assert(index>=0 && index<14 && _frame<=16);
	}
	~ThinDetector()
	{}

	size_t channelCount() const {
        return STRIP_NUM_THIN;
    }

	std::vector<PlaneCluster> buildLocalClusters(GemEvent & event, std::vector<LinearCluster> & unboundStereoClusters) const;
};

} /* namespace gem */
#endif /* GEMDETECTOR_H_ */
