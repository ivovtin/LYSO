Для сборки в test_beam:
gcc -o plotmagnegf_jan2018 -lstdc++ `root-config --libs --glibs --cflags` plotmag_neg_jan2018.cc
gcc -o procfltr18jan2 -lstdc++ `root-config --libs --glibs --cflags ` procn2jan18.cc

Сборка gemCluster:
1) сделать во всех 3-х директориях(gemCluster,reconstruction,TreeFormats) make clean
и удалить все объектвные файлы (.o)
2) удалить неверные пути в файлах заменив на свою директорию (alexbarn -> ovtin/development): для этого использовать grep -r ovtin .
3) сделать make

исходники кода лежат в
/home/alexbarn/utils/

данные лежат с прототипов в
/home/alexbarn/csipc/

procn2jan18.cc  - перекачка бинарных файлов от оцифровщика в root-файлы
tot - порог на время
total - число событий

Работаю в директории  ->  2017-12-30_00-49-06

Файл с данными от daq ->  2017-12-30_00-49.root

1) формирование root-файла с данными из бинарных файлов
../procfltr18jan2 wave_0.dat wave_4.dat wave_2.dat test.root -10000 10 0 0 2 2017-12-30_00-49.root
 если 5 параметр - число событий для обработки, если меньше 0 то обрабатываются все события
 6 параметр - порог фиксированный в каналах оцифровщика,если меньше нуля, то будет брабатывать
 всего два файла с оцифровщика, в которых будет искать положительный сигнал и триггер.
 7,8 параметры - x,y координаты подвижки и т.п. 
 dtp в программе в микросекундах, установил 2 секунды, чтобы прокачались все события. 

2)../plotmagnegf_jan2018 test.root test_t.root 0 10000 1 0  (if 5 parametr is 1 -> draw graphics)


3) запуск gemCluster
../../gem/gemCluster/gemCluster -o test_gem.root 2017-12-30_00-49.root

test.root
===========
htrg -> амплитуда тригерного МКП в каналах (wave_0.dat) (отрицательный триггер) пьедестал вычитается
htest1 -> амплитуда iMCP(ch1)(wave_4.dat) -> амплитуда прототипа с вычетом пьедестала
ttsimcp1 -> TTS iMCP(T_iMCP-T_MCPPMT -> время между триггером и прототипом

Сдруживание деревьев

root -l test_gem.root
gem->AddFriend("timedat","test_t.root");
.ls
gem->Print()
gem->Draw("detClusters.x")
gem->Draw("ch1ftcr")
gem->Draw("ch1ftcr","detClusters.x==60 && detClusters.y==20","")
gem->Draw("ch1ftcr-trgtcr")
gem->Draw("ch1ftcr-trgtcr","detClusters.x>50 && detClusters.x>70","")





