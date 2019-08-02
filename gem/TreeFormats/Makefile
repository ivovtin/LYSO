TREEFORMATS_BASE:=$(dir $(abspath $(lastword $(MAKEFILE_LIST))))
BASEDIR:=$(shell cd $(TREEFORMATS_BASE)../; pwd)

UNITS:=TreeFormats PDPCFarichHitData PDPCFarichRecoEvent TreeFormatsRootDict

INCLUDE+=$(BASEDIR)
VPATH+=$(TREEFORMATS_BASE)

CC:=g++
LD:=g++
CPPFLAGS+=`root-config --cflags` $(addprefix -I,$(INCLUDE))
CXXFLAGS+=-std=c++0x -fPIC -g
AROPTS:=r

.PHONY: all solib report clean

all: libTreeFormats.a

solib: libTreeFormats.so

OBJECTS=$(addsuffix .o, $(UNITS))
CLEAN+=$(OBJECTS) libTreeFormats.a libTreeFormats.so

libTreeFormats.a: $(OBJECTS) 
	ar $(AROPTS) $@ $^

libTreeFormats.so: $(OBJECTS) 
	$(LD) -shared $(LDFLAGS) $^ -o $@

CLEAN+=TreeFormatsRootDict.cpp TreeFormatsRootDict.h
TreeFormatsRootDict.cpp TreeFormatsRootDict.h: pdpcevent.h GemEvent.h PDPCFarichHitData.h PDPCFarichRecoEvent.h LinkDef.h
	rootcint -f $@ -c $^

report:
	@echo CPPFLAGS: $(CPPFLAGS)
	@echo BASEDIR: $(BASEDIR)
	@echo VPATH: $(VPATH)
	
clean:
	rm -f $(CLEAN) 

DEPS=$(addsuffix .cpp.d, $(UNITS))
CLEAN+=$(DEPS)

DEPGEN=$(CC) -M $(CPPFLAGS) $< | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@

%.cpp.d: %.cpp
	@$(DEPGEN)

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif
