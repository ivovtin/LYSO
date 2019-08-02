#ifndef PDPCEVENT_H
#define PDPCEVENT_H

#include <ostream>
#include <iomanip>

#include <Rtypes.h>
#include <TObject.h>

class PdpcHit: public TObject {
public:
	Int_t total_frame_time;
	UInt_t frame_number, target, die, ts, sync;
	UInt_t ampl[4]; 
	
public:
	PdpcHit()
	{
		reset();
	}
	~PdpcHit()
	{
	}
	void reset() 
	{
		total_frame_time = -1;
		frame_number = target = die = ts = sync = -1;
		ampl[0] = ampl[1] = ampl[2] = ampl[3] = -1;
	}
	ClassDef(PdpcHit, 1)
};

static std::ostream & operator << (std::ostream & ostr, const PdpcHit & h)
{
	return ostr <<
	h.total_frame_time << " " <<
	h.frame_number << " " <<
	h.sync << " " <<
	std::hex << h.target << " " << std::dec <<
	h.die << " " <<
	h.ts << " " <<
	h.ampl[0] << " " <<
	h.ampl[1] << " " <<
	h.ampl[2] << " " <<
	h.ampl[3] << " ";
}

#endif // PDPCEVENT_H
