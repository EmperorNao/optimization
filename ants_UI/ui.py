import os
import math

from PyQt5 import QtWidgets
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
import pyqtgraph as pg
from PyQt5.QtCore import *
from PyQt5.QtWidgets import QGridLayout
from PyQt5.QtWidgets import QPushButton
from drawing_math import Point, get_points

from functools import reduce


class QueensUI(QtWidgets.QMainWindow):

    def __init__(self):
        super().__init__()

        self.setWindowTitle("Ants simulator")
        self.setFixedSize(1300, 700)

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
        self.best_cost = float('+inf')
        self.app_dir = "/home/{user}/.ants".format(user=os.getlogin())
        self.pheromone_matrix = None
        self.distance_matrix = [
            [0, 18, 40, 27, 15, 4, 13, 38, 15],
            [18, 0, 33, 9, 19, 26, 18, 8, 35],
            [38, 33, 0, 17, 22, 14, 26, 22, 11],
            [25, 10, 15, 0, 33, 22, 6, 20, 5],
            [15, 21, 21, 31, 0, 10, 26, 33, 27],
            [6, 27, 16, 24, 10, 0, 22, 25, 32],
            [12, 19, 26, 5, 25, 21, 0, 28, 20],
            [36, 7, 24, 21, 31, 27, 26, 0, 13],
            [15, 33, 10, 5, 27, 32, 19, 12, 0],
        ]
        self.pheromone_matrix = [
            [1.74787e-46, 65.9527, 14.6191, 7.69346, 48.6673, 402.16, 130.301, 17.9324, 42.4507],
            [67.9444, 1.74787e-46, 44.8813, 161.558, 100.369, 19.501, 42.2772, 271.421, 21.8246],
            [24.1731, 38.7565, 1.74787e-46, 45.662, 94.737, 138.596, 64.9664, 96.2969, 226.589],
            [14.585, 77.1425, 26.4272, 1.74787e-46, 13.6072, 15.7934, 318.869, 15.9818, 247.37],
            [105.416, 129.58, 217.644, 18.8981, 1.74787e-46, 97.3297, 69.5985, 58.5558, 32.7549],
            [175.803, 26.9637, 81.4932, 19.2092, 354.188, 1.74787e-46, 25.7592, 41.1385, 5.22206],
            [255.828, 53.8591, 55.3374, 210.555, 44.0462, 18.3342, 1.74787e-46, 68.2209, 23.5963],
            [30.3524, 327.636, 76.8277, 17.8158, 52.2899, 31.6945, 63.1912, 1.74787e-46, 129.969],
            [55.6752, 9.88627, 212.547, 248.385, 21.8723, 6.36811, 14.8135, 160.229, 1.74787e-46],
        ]

    def tune_handler(self, name, widget):

        obj = widget()

        if name == "n":
        # QLineEdit widget
            int_validator = QIntValidator()
            int_validator.setRange(1, 100)
            obj.setValidator(int_validator)
            obj.setText("10")

        if name == "Q":
        # QLineEdit widget
            int_validator = QIntValidator()
            int_validator.setBottom(1)
            obj.setValidator(int_validator)
            obj.setText("10")

        elif name == "max_iterations":
        # QLineEdit widget
            int_validator = QIntValidator()
            int_validator.setRange(1, 10000000)
            obj.setValidator(int_validator)
            obj.setText("100")

        elif name == "k":
        # QLineEdit widget
            double_validator = QDoubleValidator()
            double_validator.setBottom(0)
            obj.setValidator(double_validator)
            obj.setText("100")

        elif name == "alpha":
        # QLineEdit widget
            double_validator = QDoubleValidator()
            double_validator.setBottom(0)
            obj.setValidator(double_validator)
            obj.setText("0.9")

        elif name == "beta":
        # QLineEdit widget
            double_validator = QDoubleValidator()
            double_validator.setBottom(0)
            obj.setValidator(double_validator)
            obj.setText("0.9")

        elif name == "start_pheromone":
        # QLineEdit widget
            int_validator = QIntValidator()
            int_validator.setRange(1, 10000000)
            obj.setValidator(int_validator)
            obj.setText("1")

        elif name == "rho":
        # QDoubleSpinBox widget
            obj.setDecimals(2)
            obj.setSingleStep(0.01)
            obj.setRange(0, 1)
            obj.setValue(0.9)

        return obj

    def _init_left_layout(self):
        # left
        self.left_layout = QGridLayout()
        self.general_layout.addLayout(self.left_layout)

        self.handlers = dict()

        # name, description, object, func
        handlers = [
            ("max iterations", "Number of\niterations", QLineEdit),
            ("n", "Number of\nvertexes", QLineEdit),
            ("alpha", "Alpha", QLineEdit),
            ("beta", "Beta", QLineEdit),
            ("rho", "Rho", QDoubleSpinBox),
            ("k", "Number of\nants", QLineEdit),
            ("Q", "Q", QLineEdit),
            ("start_pheromone", "Start\npheromone", QLineEdit),
            # ("", "Remove old\ngraphics", QCheckBox),
            # ("energy difference coefficient", "Rule to count\ndifference between energies", QComboBox),
        ]

        max_label_width = 100
        max_width = 50

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

        self.solve_button = QPushButton()
        self.solve_button.setText("Solve")
        self.solve_button.clicked.connect(self.solve)
        self.left_layout.addWidget(self.solve_button, len(handlers), 0)

        self.load_file_button = QPushButton()
        self.load_file_button.setText("Load matrix")
        self.load_file_button.clicked.connect(self.load_file)
        self.left_layout.addWidget(self.load_file_button, len(handlers) + 1, 0)

        self.info = QTextEdit()
        self.info.setReadOnly(True)
        self.info.setMinimumHeight(300)
        self.info.setMinimumWidth(200)

        self.left_layout.addWidget(self.info, len(handlers) + 2, 0)


    def _init_center_layout(self):
        self.central_layout = QGridLayout()
        self.general_layout.addLayout(self.central_layout)
        self.empty_widget = QWidget()
        self.empty_widget.setMinimumWidth(700)
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

        # if self.graph:

        vertexes = 9
        point_radius = 10
        orig_size = 700
        graph_radius = 250

        start_coord_x = 400
        start_coord_y = 150

        center_point = Point(625, 350)

        points = get_points(center_point, graph_radius, vertexes)
        points_to_numbers = get_points(center_point, graph_radius + 40, vertexes)

        qp = QPainter()
        qp.begin(self)
        black_pen = QPen(QColor(0, 0, 0), 1)
        qp.setPen(black_pen)

        for idx, point in enumerate(points):
            qp.drawEllipse(QPoint(point.x, point.y), 5, 5)
            qp.drawText(QPoint(points_to_numbers[idx].x, points_to_numbers[idx].y), str(idx))

        qp.end()

        if self.pheromone_matrix:

            qp = QPainter()

            qp.begin(self)
            all_values_pheromones = sorted(reduce(lambda accum, x: accum + x, self.pheromone_matrix))
            all_values_distances = sorted(reduce(lambda accum, x: accum + x, self.distance_matrix))
            def get_color(all_values, value):
                colors = [QColor(0, 0, 0),
                          QColor(255, 0, 255),
                          QColor(15, 0, 80),
                          QColor(20, 0, 120),
                          # QColor(30, 0, 180),
                          # QColor(40, 0, 240),
                          # QColor(0, 160, 210),
                          # QColor(0, 210, 40),
                          # QColor(90, 210, 0),
                          # QColor(255, 240, 10),
                          QColor(255, 110, 0),
                          QColor(255, 0, 0),
                          ]

                if len(all_values) < len(colors):
                    return colors[-all_values.find(value)]

                idx = all_values.index(value)
                step = math.ceil(len(all_values) / len(colors))
                colors_idx = idx // step
                return colors[-colors_idx]

            def get_width(all_values, value):
                width = [3 + 1.5 ** i for i in range(5)]

                if len(all_values) < len(width):
                    return width[-all_values.find(value)]

                idx = all_values.index(value)
                step = math.ceil(len(all_values) / len(width))
                colors_idx = idx // step
                return width[-colors_idx]

            for x in range(len(self.pheromone_matrix)):
                for y in range(len(self.pheromone_matrix)):
                    pen = QPen(get_color(all_values_pheromones, self.pheromone_matrix[x][y]),
                               get_width(all_values_distances, self.distance_matrix[x][y]),
                               Qt.SolidLine)
                    qp.setPen(pen)
                    qp.drawLine(QPoint(points[x].x, points[x].y), QPoint(points[y].x, points[y].y))

            # black_pen = QPen(QColor(0, 0, 0), 3, Qt.SolidLine)
            # qp.setPen(black_pen)
            # qp.drawLine()

            qp.end()
            self.update()

            # white_brush = QBrush(QColor(100, 100, 100, 0))

            # for x in range(n):
            #     for y in range(n):
            #         # if (x + y) % 2:
            #         #     br = white_brush
            #         # else:
            #         #     br = black_brush
            #
            #         qp.setBrush(black_brush)
            #         pos = QRect(start_coord_x + x * size, start_coord_y + y * size, size, size)
            #         qp.drawRect(pos)
            #         if self.solution[x] == y:
            #             qp.drawImage(pos, QImage("/home/emperornao/projects/optimization/queens_UI/queen.svg"))
            #
            # qp.end()

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

    def load_file(self):
        pass

    def solve(self):

        pass
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

        # draw graphics

        # save solution

        # paintevent automatic
        # import math
        # self.prob_graphic.getPlotItem().plot([i for i in range(100)], [math.exp(i) for i in range(100)], width = 5, pen='r')
