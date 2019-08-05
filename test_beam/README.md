## Для сборки в test_beam:
gcc -o plotmagnegf_jan2018 -lstdc++ `root-config --libs --glibs --cflags` plotmag_neg_jan2018.cc <br />
gcc -o procfltr18jan2 -lstdc++ `root-config --libs --glibs --cflags ` procn2jan18.cc <br />


## Сборка gemCluster:
1) сделать во всех 3-х директориях(gemCluster,reconstruction,TreeFormats) make clean
и удалить все объектвные файлы (.o) <br />
2) удалить неверные пути в файлах заменив на свою директорию (alexbarn -> ovtin/development): для этого использовать grep -r ovtin . <br />
3) сделать make в reconstruction <br />
4) сделать make в gemCluster<br />

исходники кода лежат в /home/alexbarn/utils/ <br />
данные лежат с прототипов в /home/alexbarn/csipc/ <br />

procn2jan18.cc  - перекачка бинарных файлов от оцифровщика в root-файлы <br />
tot - порог на время <br />
total - число событий <br />

Работаю в директории  ->  2017-12-30_00-49-06 <br />

Файл с данными от daq ->  2017-12-30_00-49.root <br />

1) формирование root-файла с данными из бинарных файлов <br />
../procfltr18jan2 wave_0.dat wave_4.dat wave_2.dat test.root -10000 10 0 0 2 2017-12-30_00-49.root <br />
 если 5 параметр - число событий для обработки, если меньше 0 то обрабатываются все события; <br />
 6 параметр - порог фиксированный в каналах оцифровщика,если меньше нуля, то будет брабатывать 
 всего два файла с оцифровщика, в которых будет искать положительный сигнал и триггер. <br />
 7,8 параметры - x,y координаты подвижки и т.п.           
 dtp в программе в микросекундах, установил 2 секунды, чтобы прокачались все события. <br /> 

2) ../plotmagnegf_jan2018 test.root test_t.root 0 10000 1 0  (if 5 parametr is 1 -> draw graphics) <br />

3) запуск gemCluster <br />
../../gem/gemCluster/gemCluster -o test_gem.root 2017-12-30_00-49.root <br />

test.root <br />
htrg -> амплитуда тригерного МКП в каналах (wave_0.dat) (отрицательный триггер) пьедестал вычитается <br />
htest1 -> амплитуда iMCP(ch1)(wave_4.dat) -> амплитуда прототипа с вычетом пьедестала <br />
ttsimcp1 -> TTS iMCP(T_iMCP-T_MCPPMT -> время между триггером и прототипом <br />

## Сдруживание деревьев

root -l test_gem.root <br />
gem->AddFriend("timedat","test_t.root"); <br />
.ls <br />
gem->Print() <br />
gem->Draw("detClusters.x") <br />
gem->Draw("ch1ftcr")  <br />
gem->Draw("ch1ftcr","detClusters.x==60 && detClusters.y==20","") <br />
gem->Draw("ch1ftcr-trgtcr") <br />
gem->Draw("ch1ftcr-trgtcr","detClusters.x>50 && detClusters.x>70","") <br />

