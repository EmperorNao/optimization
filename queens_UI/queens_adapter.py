import subprocess


class QueensSolver:

    def __init__(self, bin_path=None):
        self.BIN_PATH = bin_path or "/home/emperornao/projects/optimization/queens_UI/queens_solver"

    def solve(self, config_path, log_file, time_file="time_measure"):

        f = open(log_file, "w")
        subprocess.run(['/usr/bin/time',
                                  "-felapsed time: %E\nuser time: %U\nsystem time: %S\n",
                                  f'-o{time_file}',
                                  self.BIN_PATH,
                                  '--conf',
                                  config_path,
                                  '--verbose'], stdout=f, universal_newlines=True)
        f.close()

