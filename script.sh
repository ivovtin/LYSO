#!/bin/bash

for dir in /home/chizhik/root/2019-06-07_LYSO/*
do
	echo $dir
	cd ${dir}
	adress=${dir#/home/chizhik/root/2019-06-07_LYSO/}
	echo $adress
	adress="${adress%-*}"
	echo $adress
	/home/akatcin/utils/procfltr18jan2 wave_0.dat wave_4.dat wave_2.dat sipm${adress}.root -1000 20 0.0 0.0 2 ${dir}/${adress}.root
done
