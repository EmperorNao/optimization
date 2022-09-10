import os
import math

from PyQt5 import QtWidgets
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
import pyqtgraph as pg
from PyQt5.QtCore import *
from PyQt5.QtWidgets import QGridLayout
from PyQt5.QtWidgets import QPushButton

from queens_adapter import QueensSolver


class QueensUI(QtWidgets.QMainWindow):

    def __init__(self):
        super().__init__()

        self.setWindowTitle("Queens")
        self.setFixedSize(1200, 700)

        # main layout and main widget
        self.general_layout = QHBoxLayout()
        self.centralWidget = QtWidgets.QWidget(self)
        self.setCentralWidget(self.centralWidget)
        self.centralWidget.setLayout(self.general_layout)

        # init inclusive layouts
        self._init_left_layout()
        self._init_center_layout()
        self._init_right_layout()

        #
        self.solution = []
        self.app_dir = "/home/{user}/.queens".format(user=os.getlogin())
        self.solver = QueensSolver()

    def tune_handler(self, name, widget):

        obj = widget()

        if name == "n":
        # QLineEdit widget
            int_validator = QIntValidator()
            int_validator.setRange(1, 100)
            obj.setValidator(int_validator)
            obj.setText("40")

        elif name == "max iterations":
        # QLineEdit widget
            int_validator = QIntValidator()
            int_validator.setRange(1, 10000000)
            obj.setValidator(int_validator)
            obj.setText("10000")

        elif name == "start temperature":
        # QLineEdit widget
            double_validator = QDoubleValidator()
            double_validator.setBottom(0)
            obj.setValidator(double_validator)
            obj.setText("100.0")

        elif name == "min temperature":
        # QLineEdit widget
            double_validator = QDoubleValidator()
            double_validator.setBottom(0)
            obj.setValidator(double_validator)
            obj.setText("0.05")

        elif name == "fixed temperature iterations":
        # QLineEdit widget
            int_validator = QIntValidator()
            int_validator.setRange(1, 10000000)
            obj.setValidator(int_validator)
            obj.setText("100")

        elif name == "decreasing rule":
        # QComboBox widget
            obj.addItem("exponential")

        elif name == "decreasing coefficient":
        # QDoubleSpinBox widget
            obj.setDecimals(2)
            obj.setSingleStep(0.01)
            obj.setRange(0, 0.99)
            obj.setValue(0.98)

        elif name == "energy difference coefficient":
        # QComboBox widget
            obj.addItem("simple")

        elif name == "remove old graphics":
        # QCheckBox widget
            obj.setChecked(True)

        return obj

    def _init_left_layout(self):
        # left
        self.left_layout = QGridLayout()
        self.general_layout.addLayout(self.left_layout)

        self.handlers = dict()

        # name, description, object, func
        handlers = [
            ("max iterations", "Maximum number\nof iterations", QLineEdit),
            ("n", "Number of \nqueens", QLineEdit),
            ("start temperature", "Temperature to\nstart", QLineEdit),
            ("min temperature", "Minimal temperature", QLineEdit),
            ("fixed temperature iterations", "Number of iterations\nwith fixed temperature", QLineEdit),
            # ("decreasing rule", "Rule to\ndecrease temperature", QComboBox),
            ("decreasing coefficient", "Exponential\ncoefficient", QDoubleSpinBox),
            ("remove old graphics", "Remove old\ngraphics", QCheckBox),
            # ("energy difference coefficient", "Rule to count\ndifference between energies", QComboBox),
        ]

        max_label_width = 180
        max_width = 70

        for idx, obj in enumerate(handlers):
            name, description, object = obj

            self.handlers[name] = self.tune_handler(name, object)
            self.handlers[name].setMaximumWidth(max_width)
            if isinstance(obj, QLineEdit):
                self.handlers[name].setAlignment(Qt.AlignTop)

            self.handlers[name + "_label"] = QLabel()
            self.handlers[name + "_label"].setText(description)
            self.handlers[name + "_label"].setMaximumWidth(max_label_width)
            self.handlers[name + "_label"].setAlignment(Qt.AlignTop)

            self.left_layout.addWidget(self.handlers[name + "_label"], idx, 0)
            self.left_layout.addWidget(self.handlers[name], idx, 1)

        self.info = QTextEdit()
        self.info.setReadOnly(True)
        self.info.setMinimumHeight(300)
        self.info.setMinimumWidth(250)

        self.left_layout.addWidget(self.info)

        self.solve_button = QPushButton()
        self.solve_button.setText("Solve")
        self.solve_button.clicked.connect(self.solve)
        self.left_layout.addWidget(self.solve_button)

    def _init_center_layout(self):
        self.central_layout = QGridLayout()
        self.general_layout.addLayout(self.central_layout)
        self.empty_widget = QWidget()
        self.empty_widget.setMinimumWidth(500)
        self.central_layout.addWidget(self.empty_widget)

    def _init_right_layout(self):
        self.right_layout = QVBoxLayout()
        self.right_layout.setSizeConstraint(QLayout.SetFixedSize)
        self.general_layout.addLayout(self.right_layout)

        self.prob_graphic = pg.PlotWidget()
        self.temp_graphic = pg.PlotWidget()
        self.cost_graphic = pg.PlotWidget()

        self.prob_graphic.setBackground('w')
        self.prob_graphic.setLabel('left', 'avg wrong choise proba')
        self.prob_graphic.setLabel('bottom', 'temperature')

        self.temp_graphic.setBackground('w')
        self.temp_graphic.setLabel('left', 'temperature')
        self.temp_graphic.setLabel('bottom', 'iteration')

        self.cost_graphic.setBackground('w')
        self.cost_graphic.setLabel('left', 'energy')
        self.cost_graphic.setLabel('bottom', 'temperature')

        self.right_layout.addWidget(self.prob_graphic)
        self.right_layout.addWidget(self.temp_graphic)
        self.right_layout.addWidget(self.cost_graphic)

    def paintEvent(self, event=None):

        if self.solution:

            n = int(self.handlers['n'].text())
            if n != len(self.solution):
                return

            start_coord_x = 400
            start_coord_y = 100

            orig_size = 400
            size = int(orig_size // n)

            qp = QPainter()
            qp.begin(self)

            black_brush = QBrush(QColor(100, 0, 0, 0))
            # white_brush = QBrush(QColor(100, 100, 100, 0))

            for x in range(n):
                for y in range(n):
                    # if (x + y) % 2:
                    #     br = white_brush
                    # else:
                    #     br = black_brush

                    qp.setBrush(black_brush)
                    pos = QRect(start_coord_x + x * size, start_coord_y + y * size, size, size)
                    qp.drawRect(pos)
                    if self.solution[x] == y:
                        qp.drawImage(pos, QImage("/home/emperornao/projects/optimization/queens_UI/queen.svg"))

            qp.end()

    def create_options(self, annealing_config, queens_config):

        with open(queens_config, "w") as queens_file:
            queens_file.write("n: " + self.handlers["n"].text() + '\n')

        with open(annealing_config, "w") as annealing_file:
            annealing_file.write("seed: " + "0" + '\n')
            annealing_file.write("precision: " + "1e-6" + '\n')
            annealing_file.write("decreasing_rule: " + "exponential" + '\n')
            
            annealing_file.write("solution_config_file: " + queens_config + '\n')

            annealing_file.write("max_iterations: " + self.handlers["max iterations"].text() + '\n')
            annealing_file.write("start_temperature: " + self.handlers["start temperature"].text() + '\n')
            annealing_file.write("min_temperature: " + self.handlers["min temperature"].text() + '\n')
            annealing_file.write("fixed_temperature_iterations: " + self.handlers["fixed temperature iterations"].text() + '\n')
            annealing_file.write("decreasing_coefficient: " + str(self.handlers["decreasing coefficient"].value()) + '\n')

    def parse_logs(self, time_log, program_log):

        time_stats = "\n".join(open(time_log).readlines())

        best_solution = []
        best_cost = float("+inf")

        stats = []
        with open(program_log) as file:

            started = False
            cur_iter = None

            avg_prob = None
            cnt_inner_iters = 0

            for line in file:

                if line == "\n":
                    continue
                if not started and not line.startswith('Starting annealing'):
                    continue
                elif line.startswith('Starting annealing'):
                    started = True

                if line.startswith("iter") or line.startswith("Ended search"):
                    if cur_iter:
                        cur_iter["avg_probability"] = avg_prob / cnt_inner_iters
                        stats.append(cur_iter)

                    cur_iter = {}
                    avg_prob = None
                    cnt_inner_iters = 0

                elif line.startswith("best cost"):
                    cur_iter["cost"] = float(line.split("=")[-1].strip())

                elif line.startswith("current temperature"):
                    cur_iter["temperature"] = float(line.split("=")[-1].strip())

                elif line.startswith("inner iter"):
                    if avg_prob is None:
                        avg_prob = 0
                        cnt_inner_iters = 0

                elif line.startswith("probability"):
                    avg_prob += float(line.split("=")[-1].strip())
                    cnt_inner_iters += 1

                elif line.startswith("Found best solution"):
                    best_solution = line.split("=")[-1].strip().split()

                elif line.startswith("Best cost"):
                    best_cost = float(line.split("=")[-1].strip())

        return best_solution, best_cost, time_stats, stats

    def draw_graphics(self, stats):

        temperature = list(map(lambda x: x['temperature'], stats))
        avg_proba = list(map(lambda x: x['avg_probability'], stats))
        cost = list(map(lambda x: x['cost'], stats))
        iterations = [i for i in range(len(stats))]

        if self.handlers['remove old graphics'].isChecked():
            self.prob_graphic.clear()
            self.temp_graphic.clear()
            self.cost_graphic.clear()

        self.prob_graphic.getPlotItem().plot(temperature, avg_proba, pen='blue', width=10)
        self.temp_graphic.getPlotItem().plot(iterations, temperature, pen='orange', width=10)
        self.cost_graphic.getPlotItem().plot(temperature, cost, pen='red', width=10)

    def solve(self):

        # get options
        # create options file
        if not os.path.exists(self.app_dir):
            os.mkdir(self.app_dir)

        annealing_config = os.path.join(self.app_dir, "annealing.conf")
        queens_config = os.path.join(self.app_dir, "queens.conf")

        self.create_options(annealing_config, queens_config)

        # start bin
        # need to do it in threads to do not stop main window

        time_log = os.path.join(self.app_dir, "time.log")
        program_log = os.path.join(self.app_dir, "stdout.log")

        self.solver.solve(annealing_config, program_log, time_log)

        # parse results
        best_solution, best_cost, time_stats, stats = self.parse_logs(time_log, program_log)

        text = [time_stats]
        text.append("Best founded solution = " + " ".join(best_solution) + "\n")
        text.append("Best cost = " + str(best_cost) + "\n")
        text = "".join(text)

        self.info.clear()
        self.info.setText(text)

        self.solution = list(map(int, best_solution))

        self.draw_graphics(stats)

        self.update()

        # draw graphics

        # save solution

        # paintevent automatic
        # import math
        # self.prob_graphic.getPlotItem().plot([i for i in range(100)], [math.exp(i) for i in range(100)], width = 5, pen='r')
