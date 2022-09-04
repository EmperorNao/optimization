
Утилита для запуска алгоритма отжига для задачи ферзей

```
Usage: queens_simulated_annealing [options] 

Optional arguments:
-h --help    	shows help message and exits [default: false]
-v --version 	prints version information and exits [default: false]
--conf       	file with settings for annealing [required]
--verbose    	increase output verbosity [default: false]
```

Формат annealing конфига:

```
# путь к конфигу с настройками для задачи ферзей, описано ниже
solution_config_file: /home/path/to/queens.conf
# сид для генератора, [int64_t, optional, default: 0]
seed: 42
# максимальное количество операций [int64_t, optional, default: 10000] 
max_iterations: 10000 
# начальная температура [double, optional, default: 30]
start_temperature: 150
# минимальная температура [double, optional, default: 0.05]
min_temperature: 0.05
# количество итераций с фиксированной температурой [int64_t, optional, default: 1000] 
fixed_temperature_iterations: 1000
# точность, которая используется при сравнении чисел и делении [double, optional, default: 1e-6] 
precision: 1e-9
# способ уменьшения температуры. По умолчанию - exponential, умножение на константу [string, optional, default: exponential]
decreasing_rule: exponential
# величиная для умножения температура при exponential decreasing_rule [double, optional, default: 0.98]
decreasing_coefficient: 0.98
# способ вычисления разницы между энергиями состояний. По умолчанию - simple, разность [string, optional, default: simple]
energy_difference_rule : simple
```

Формат конфига для задачи ферзей
```
# количество ферзей и размер доски [uint64_t, required]
n: 3
# решение, которое будет использоваться как начальное - n чисел, указанные через пробел [string, optional]
start_solution: 0 2 1
```