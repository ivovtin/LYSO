# LYSO
## Getting the code
Fork to your directory the code here: https://github.com/ivovtin/LYSO<br />
Clone it locally:<br />
git clone https://github.com/[YOURNAME]/LYSO <br />
You should create your own branch, then do pull requests when you are done. <br />

## Description
- Information about experiment - logbook.txt <br />
- Base script is plotxy_new.C <br />
  Выполняет обработку данных. Сдруживается дерево exbeamdata от МКП c деревьями SiPM, DAQ и GEM.
  Фронт сигнала фитируется прямой. И для нахождения времени на фронте ищется пересечение двух прямых - фита и константы 0.5х(пьедестал + минимум сигнала). Кристал LYSO по координате Y разбивается на несколько промежутков (14-29 мм) с помощью данных с GEM-детекторов, и для каждого получаем временное разрешение. Для отбора событий используется два тригерных детектора МКП - накладывается условие на амплитудные распределения. <br /> 

  Example: <br />
  root -l -q 'plotxy_new.C("lyso30minus",200,0,0,14,28,0.5)' <br />
  root -l -q 'plotxy_new.C("lyso30plus",200,0,0,14,28,0.5)' <br />
  root -l -q 'plotxy_new.C("lysozero",200,0,0,14,28,0.5)' <br />

- Start tasks on batch system: <br />
  ./run_batch.sh

- test_beam is directory with script for create in root-file format from binary files <br />

- gem is directory for create gemCluster
