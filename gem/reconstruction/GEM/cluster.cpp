#include "cluster.h"


#include <algorithm> //max_element
#include <cmath> //fabs
#include <iostream>

using namespace std;

namespace gem {

LinearCluster::LinearCluster(unsigned begin, unsigned end, unsigned begin_av, unsigned end_av, unsigned maximum, const Short_t * amplitudes):
	_begin(begin),
	_end(end)
{
	_max_strip_amp = amplitudes[maximum];
	
	_amplitude = accumulate(amplitudes + begin, amplitudes + end, 0.);
	_width = end - begin;
	assert(_width > 0);

	_type = NORMAL_CLS;
	if( begin_av == maximum || end_av-1 == maximum )
		_type = EDGE_CLS;
	if( end_av-begin_av==1 )
		_type = ONESTRIP_CLS;
	if( _max_strip_amp>Clusterization::gAmpOverload )
		_type = OVERLOAD_CLS;

	//Determine mean position with up to 3 channels near the cluster's maximum
	double convolution = 0, weights = 0;
	for(unsigned i=begin_av; i<end_av; i++) {
#ifdef DEBUG
		cout<<" a["<<int(i-maximum)<<"]="<<amplitudes[i];
#endif
		convolution += double(amplitudes[i]) * i;
		weights += double(amplitudes[i]);
	}
	if( weights!=0. ) {
		_mean = (convolution / weights);
	} else {
		_mean = -1;
	}
#ifdef DEBUG
	cout<<"\n begin_av="<<begin_av<<" end_av="<<end_av<<" type="<<_type<<" weights="<<weights<<" mean="<<_mean<<endl;
#endif
}

float Clusterization::gRelThreshold = 0.1; //Default threshold given as fraction of maximum strip amplitude in a cluster
unsigned Clusterization::gNchannelAveraging = 0; //Number of channels to average position over, if 0 average all channels above threshold
unsigned short Clusterization::gAmpOverload = 1900; //Amplitude overload threshold

unsigned Clusterization::findFront(const Short_t *  amplitudes, unsigned start, int direction, float threshold) const {
	assert(direction == 1 || direction == -1);
	unsigned  i = start;
	int sign = amplitudes[start] > 0 ? 1 : -1;
	while(i >= 0 && i < channelCount()) {
		if ( sign * amplitudes[i] <= threshold * sign * amplitudes[start] )
			break;
		i += direction;
	}
	i -= direction;
	return i;
}


Clusterization::Cluster  Clusterization::extractCluster(const Short_t *  amplitudes, size_t start) const {
	assert(start < channelCount());
	unsigned begin = findFront(amplitudes, start, -1, 0.); // back
	unsigned end = findFront(amplitudes, start, 1, 0.);    // front
	unsigned begin_av = start, end_av = start; 
	if( gNchannelAveraging ) {
		if( gNchannelAveraging%2==0 )
			cerr << "Warning in Clusterization::extractCluster(): Even number of weighted strips " << gNchannelAveraging << endl;
		unsigned shift = (gNchannelAveraging-1)/2;
		begin_av = (begin<start-shift)?start-shift:begin;
		end_av = (end>start+shift)?start+shift:end;
	} else {
		begin_av = findFront(amplitudes, start, -1, gRelThreshold); // back
		end_av = findFront(amplitudes, start, 1, gRelThreshold);    // front
		if( end_av==begin_av ) {
			if( begin<begin_av ) begin_av--;
			if( end_av<end ) end_av++;
		}
	}
	assert(begin_av <= end_av);
	assert(begin <= begin_av);
	assert(end_av <= end);
	return Cluster(begin, end+1, begin_av, end_av+1, start, amplitudes);
}

bool lessByAbs(Short_t a1, Short_t a2) {
	return abs(a1) < abs(a2);
}

std::vector<Clusterization::Cluster> Clusterization::getClusters(const Short_t *  amplitudes) const {
	vector<Cluster> rv;
	const size_t size = channelCount();
	Short_t data[size];

	copy(amplitudes, amplitudes+size, data);

	Short_t *begin = data, *end = data+size;

        //Cut edge noisy channels
	begin += EDGE_CUT;
	fill(data, begin, 0);
	end -= EDGE_CUT;
	fill(end, data+size, 0);

	const Short_t *maximum = max_element(begin, end, lessByAbs);
	int sign = (*maximum)>0 ? 1 : -1;
	if (maximum == end) return rv;

	//Search for opposite sign amplitudes
	int tot_amplitude = 0, opp_amplitude = 0;
	for(Short_t *p = begin; p < end; p++) {
		tot_amplitude += *p;
		if( sign * (*p) < 0 ) {
			opp_amplitude += *p;
			*p = 0;
		}
	}
#ifdef DEBUG
	cerr << "opp_amp=" << opp_amplitude << " tot_amp=" << tot_amplitude << "\n";
#endif
	//If too much signal with opposite sign drop the event
	if( -sign*opp_amplitude > 0.2*sign*tot_amplitude ) {
#ifdef DEBUG
		cerr << "Too much opposite sign amplitudes.\n";
#endif
		return rv;
	}

	while(rv.size() < MAX_CLUSTER_COUNT) {
		if (maximum == end || sign*(*maximum) < MIN_CLUSTER_AMPLITUDE)
			break;
		Cluster temp = extractCluster(amplitudes, (maximum - data));

		assert(temp.width() > 0);

		rv.push_back(temp);

		//Filling cluster channels with zeroes to prevent repeated detection.
		fill(data+temp.begin(), data+temp.end(), 0);

		maximum = max_element(begin, end, lessByAbs);
	}
	return rv;
}

void PlaneCluster::save(GemCluster & oCluster) const {
	oCluster.typex = _xCluster.type();
	oCluster.x = _xCluster.mean() * 0.5;
	oCluster.dx = _xCluster.width();
	oCluster.max = fabs(_xCluster.max_strip_amplitude());
	oCluster.rawStraight = _xCluster.mean();
	if (_yCluster) {
		oCluster.typey = _yCluster->type();
		if (_type==THICK_DET)
			oCluster.y = yChannelToMMThick(_xCluster.mean(), _yCluster->mean());
		else
			oCluster.y = yChannelToMMThin(_xCluster.mean(), _yCluster->mean());
		oCluster.dy = _yCluster->width();
		oCluster.may = fabs(_yCluster->max_strip_amplitude());
		oCluster.rawStereo = _yCluster->mean();
	}
	oCluster.ax = amplitudeX();
	oCluster.ay = amplitudeY();
}

}
