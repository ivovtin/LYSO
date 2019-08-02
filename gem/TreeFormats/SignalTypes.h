/*
 * SignalTypes.h
 *
 *  Created on: 10.09.2012
 *      Author: gulevich
 */

#ifndef SIGNALTYPES_H_
#define SIGNALTYPES_H_
#include <Rtypes.h>
#include <TObject.h>


class Cluster: public TObject {
public:
	UInt_t id;
	Double_t x, y, z;
	Double_t dx, dy, dz;
	Cluster(): id(-1), x(0), y(0), z(0), dx(-1), dy(-1), dz(-1){}
};

class Track: public TObject {
public:
	UInt_t id;
	Double_t vx, vy, vz;
	Double_t x0, y0, z0;
	ClassDef(Track, 1)
};


#endif /* SIGNALTYPES_H_ */
