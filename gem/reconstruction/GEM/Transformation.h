#include <TRotation.h>
#ifndef TRANSFORMATION_H_
#define TRANSFORMATION_H_

class Transformation {
public:
	virtual TVector3 transform(const TVector3 & input) const = 0;
	virtual ~Transformation(){}
};

class Shift: public Transformation {
	TVector3 _shift;
public:
	TVector3 shift() const;
	void shift(const TVector3 & shift ) {
		_shift = shift;
	}
	virtual TVector3 transform(const TVector3 & input) const {
		return input+_shift;
	}
};

class PhiTrans: public Transformation {
	TVector3 _shift;
	Double_t _phi;
	TRotation _rotation;
public:
	PhiTrans(){}
	void setParameters(const TVector3 & shift, Double_t phi) {
		_rotation=TRotation().RotateZ(phi);
		_phi = phi;
		_shift=shift;
	}
	Double_t phi() const {return _phi;}
	TVector3 shift() const {return _shift;}
	virtual TVector3 transform(const TVector3 & input) const {
		TVector3 rv = input + _shift;
		return _rotation * rv;
	}
};

#endif /* TRANSFORMATION_H_ */
