from queens_adapter import QueensSolver



solver = QueensSolver()

config_file = "annealing.conf"
log_file = "stdout.log"
time_file = "time.log"

solver.solve(config_file, log_file, time_file)
print("Ended")