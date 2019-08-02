#ifndef LINKDEF_H_
#define LINKDEF_H_

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class GemCluster+;
#pragma link C++ class PdpcHit+;
#pragma link C++ class PDPCFarichHitData+;
#pragma link C++ class PDPCFarichRecoHit+;
#pragma link C++ class PDPCFarichRecoEvent+;
//#pragma link C++ class Track+;

#endif

#endif /* LINKDEF_H_ */
