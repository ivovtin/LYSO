#ifndef LINKDEF_H_
#define LINKDEF_H_

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class PDPCFarichGeometry+;
#pragma link C++ class PDPCFarichGeometry::Geometry+;
#pragma link C++ class PDPCFarichGeometry::Tile+;

#endif

#endif /* LINKDEF_H_ */
