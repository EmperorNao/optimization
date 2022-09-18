Утилита для решения задачи коммивояжера с помощью муравьиного алгоритма
```
Usage: commivoyager ants solver [options] 

Optional arguments:
-h --help    	shows help message and exits [default: false]
-v --version 	prints version information and exits [default: false]
--conf       	file with settings for solving [required]
--verbose    	increase output verbosity [default: false]
```

Конфиг файл с параметрами алгоритма:
```
# путь к матрице смежности графа
data_path: /home/emperornao/projects/optimization/ants/data2.matrix
# параметр степени при феромонах
alpha: 0.9
# параметр степени при расстоянии
beta: 0.9
# параметр отношения сохранения феромонов по итерациям
rho: 0.9
# параметр при обновлении феромонов
Q: 10
# количество муравьёв (внутренних итераций)
k: 1000
# стартовая инициализация матрицы феромонов
start_pheromone: 1
# общее количество итераций для алгоритма
max_iterations: 1000
```

Файл подаётся в формате .matrix:

На 1 строке количество вершин n,n (в бинаре ожидается одинаковое значения). Далее n строк с n значениями через запятые, 
характеризующие матрицу смежности графа. Значения на диагонали не будут использоваться.


Например:
```
3,3
1,10,3
3,1,10
10,3,10
```