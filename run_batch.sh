#!/bin/bash
Nev=500
for((i=0; i<=12; i+=3))
do
  echo "i=" "$i"
  y1=$[14+$i]
  y2=$[17+$i]
  echo "Nev=" "$Nev"
  echo "y1=" "$y1"
  echo "y2=" "$y2"
  qsub -shell n -b y -V -N work1_"$i" -cwd root -l -b -q 'plotxy_new.C("lyso30minus",'$Nev',0,0,'$y1','$y2')'
  qsub -shell n -b y -V -N work2_"$i" -cwd root -l -b -q 'plotxy_new.C("lysozero",'$Nev',0,0,'$y1','$y2')'
  qsub -shell n -b y -V -N work3_"$i" -cwd root -l -b -q 'plotxy_new.C("lyso30plus",'$Nev',0,0,'$y1','$y2')'
done
