# LYSO
## Getting the code
Fork to your directory the code here: https://github.com/ivovtin/LYSO<br />
Clone it locally:<br />
git clone https://github.com/[YOURNAME]/LYSO <br />
You should create your own branch, then do pull requests when you are done. <br />
- Information about experiment - logbook.txt <br />
- Base script is plotxy_new.C <br />
  Example: <br />
  root -l <br /> 
  root [0] .x plotxy_new.C("lyso30minus",200,0,0,23,26) <br />
  or <br />
  root -l -q 'plotxy_new.C("lyso30minus",200,0,0,23,26)'

- Script for create chain: <br />
  make_chain.C

- Start tasks on batch system: <br />
  ./run_batch.sh

- test_beam is directory with script for create in root-file format from binary files <br />

- gem is directory for create gemCluster
