#!/bin/bash

incfgfn="gem_geom_config_rough_align.dat"
for f in "$@"; do
  gemfn=$(dirname $f)/gem_$(basename $f)
  ts=$(basename $f)
  ts=${ts%'.root'}
#  echo "Running gemCluster..."
#  ../../git/gemCluster/Debug/gemCluster -o $gemfn $f
  outcfgfn="gem_geom_config_$ts.dat"
  echo "Running calibrateAlign..."
  ../../git/gemCluster/Debug/calibrateAlign -B -F -b 2 -t 0 -i $incfgfn -o $outcfgfn $gemfn
done
