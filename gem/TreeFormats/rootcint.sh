#!/bin/sh

TARGET="$PWD"
cd ..
rootcint -f rootdict.cpp -c GemEvent.h LinkDef.h || exit 1
true