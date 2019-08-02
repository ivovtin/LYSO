#ifndef ServiceEvent_H_
#define ServiceEvent_H_


struct ServiceData {
	UInt_t time, eventIndex;
	ULong64_t utime;
	ServiceData() { time=utime=eventIndex=0; }
};

#endif