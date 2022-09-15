Утилита для алгоритма кластеризации ART-1 

TODO: shuffling flat

```
Usage: art clustering [options]

Optional arguments:
-h --help    	shows help message and exits [default: false]
-v --version 	prints version information and exits [default: false]
--conf       	file with settings for clustering [required]
--verbose    	increase output verbosity [default: false]
```

Формат конфига для кластеризации
```
# путь к данным
data_path: /home/emperornao/projects/optimization/art/data2.matrix
# формат данных: csv, matrix
data_format: matrix
# параметр внимательности
rho: 0.9
# параметр для анти-обнуления
beta: 1
# размер кластеров
N: 1000
```