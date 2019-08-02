CLUSTER_BASE:=$(dir $(abspath $(lastword $(MAKEFILE_LIST))))
BASEDIR:=$(shell cd $(CLUSTER_BASE)../; pwd)

BOOST_ROOT=/usr/local/boost_1_54_0/
TREEFORMATS=$(BASEDIR)/TreeFormats/
RECONSTRUCTION=$(BASEDIR)/reconstruction/
INCLUDE+=$(BASEDIR) $(BOOST_ROOT)
VPATH+=$(CLUSTER_BASE) $(RECONSTRUCTION)/GEM $(BOOST_ROOT)

CC:=g++
CXX:=g++
CPPFLAGS+=`root-config --cflags` $(addprefix -I,$(INCLUDE)) -std=c++0x -g

UNITS=main 
#calibrateAlign trackFinder

all: gemCluster 
#calibrateAlign trackFinder

OBJECTS=$(addsuffix .o, $(UNITS))
CLEAN+=$(OBJECTS) libTreeFormats.a libreconstruction.a gemCluster

.PHONY: all clean report libTreeFormats.a libreconstruction.a
	
libTreeFormats.a:
	$(MAKE) -f $(TREEFORMATS)Makefile AROPTS=rsT
	
libreconstruction.a:
	$(MAKE) -f $(RECONSTRUCTION)Makefile AROPTS=rsT

gemCluster: gemCluster2018.o libTreeFormats.a $(RECONSTRUCTION)libreconstruction.a #main.o
	$(CXX) -o $@ $^ `root-config --libs`

calibrateAlign: calibrateAlign.o libTreeFormats.a libreconstruction.a
	$(CXX) -o $@ $^ `root-config --libs` -lMinuit

trackFinder: trackFinder.o libTreeFormats.a libreconstruction.a
	$(CXX) -o $@ $^ `root-config --libs` -lMinuit

report:
	@echo CPPFLAGS: $(CPPFLAGS)
	@echo BASEDIR: $(BASEDIR)
	@echo VPATH: $(VPATH)
	
clean:
	rm -f $(CLEAN) 
	$(MAKE) -f $(RECONSTRUCTION)Makefile clean
	$(MAKE) -f $(TREEFORMATS)Makefile clean

DEPS=$(addsuffix .cpp.d, $(UNITS))
CLEAN+=$(DEPS)

DEPGEN=$(CC) -M $(CPPFLAGS) $< | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@

%.cpp.d: %.cpp
	@$(DEPGEN)

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif
