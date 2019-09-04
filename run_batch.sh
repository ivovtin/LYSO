#!/bin/bash
Nev=400000
trh=0.50
#for((i=0; i<=12; i+=3))
#do
#  echo "i=" "$i"
#  y1=$[14+$i]
#  y2=$[17+$i]
  y1=14
  y2=28 
  echo "Nev=" "$Nev"
  echo "trh=" "$trh"
  echo "y1=" "$y1"
  echo "y2=" "$y2"
  qsub -shell n -b y -V -N work1_"$i"_"$trh" -cwd root -l -b -q 'plotxy_new.C("lyso30minus",'$Nev',0,0,'$y1','$y2','$trh',1)'
  qsub -shell n -b y -V -N work2_"$i"_"$trh" -cwd root -l -b -q 'plotxy_new.C("lysozero",'$Nev',0,0,'$y1','$y2','$trh',1)'
  qsub -shell n -b y -V -N work3_"$i"_"$trh" -cwd root -l -b -q 'plotxy_new.C("lyso30plus",'$Nev',0,0,'$y1','$y2','$trh',1)'
#done
