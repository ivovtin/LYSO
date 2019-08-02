#ifndef QdcEvent_H_
#define QdcEvent_H_

#include <QDC_C0312.h>

struct QdcEvent{
	u16_t channels[QDC_C0312::nChannels];
};

#endif
