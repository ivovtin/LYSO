#ifndef TRACKFINDER_H_
#define TRACKFINDER_H_

#include <vector>
#include "../TreeFormats/SignalTypes.h"


class TrackFinder {
public:
	typedef std::vector<Cluster> Clusters;
	struct TrackWithClusters {
		Track track;
		TrackFinder::Clusters clusters;
	};
	//Returns Clusters grouped by tracks
	virtual void findTracks(const Clusters & clusters, std::vector<TrackWithClusters> & oTracks) = 0;
	virtual ~TrackFinder(){}
};


class FussyRadonFinder: public TrackFinder {
public:
	virtual void findTracks(const Clusters & clusters, std::vector<TrackWithClusters> & oTracks) override;
	struct Matcher {
		virtual double match(const Track & track, const Cluster & cluster) =0;
	};
	FussyRadonFinder(Matcher & simulator);
private:
	Matcher & _simulator;
};

class SimpleTrackMatcher: public FussyRadonFinder::Matcher {
public:
	//Sigma of distance from track to cluster
	double distance;
	virtual double match(const Track & track, const Cluster & cluster) override;
};


#endif /* TRACKFINDER_H_ */
