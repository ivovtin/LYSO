RECONSTRUCTION_BASE:=$(dir $(abspath $(lastword $(MAKEFILE_LIST))))
BASEDIR:=$(shell cd $(RECONSTRUCTION_BASE)../; pwd)

UNITS:=GEM 
#PDPCFarich

INCLUDE+=$(BASEDIR)
VPATH+=$(RECONSTRUCTION_BASE) $(addprefix $(RECONSTRUCTION_BASE),$(UNITS))

CC:=g++
CPPFLAGS+=`root-config --cflags` $(addprefix -I,$(INCLUDE))
CXXFLAGS+=-std=c++0x -fPIC -g
AROPTS:=r

all: libreconstruction.a

SOURCES:=$(notdir $(wildcard $(patsubst %,$(RECONSTRUCTION_BASE)%/*.cpp,$(UNITS))))
OBJECTS:=$(SOURCES:.cpp=.o) 
#PDPCFarichRootDict.o

CLEAN+=$(OBJECTS) libreconstruction.a

libreconstruction.a: $(OBJECTS)
	ar $(AROPTS) $@ $^

PDPCFarichRootDict.cpp PDPCFarichRootDict.h: PDPCFarichGeometry.h PDPCFarichLinkDef.h
	rootcint -f $@ -c $^

report:
	@echo CPPFLAGS: $(CPPFLAGS)
	@echo BASEDIR: $(BASEDIR)
	@echo VPATH: $(VPATH)
	
DEPS=$(addsuffix .d,$(SOURCES))
CLEAN+=$(DEPS)

DEPGEN=$(CC) -M $(CPPFLAGS) $< | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@

clean:
	rm -f $(CLEAN) 

%.cpp.d: %.cpp
	@echo Generating dependency file for $<...
	@$(DEPGEN)

ifeq ($(findstring clean,$(MAKECMDGOALS)),)
  -include $(DEPS)
endif
