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

from run_executable import BinRunner


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
        self.distance_matrix = None
        self.pheromone_matrix = None
        self.filename = None

    def tune_handler(self, name, widget):

        obj = widget()

        if name == "n":
        # QLineEdit widget
            int_validator = QIntValidator()
            int_validator.setRange(1, 100)
            obj.setValidator(int_validator)
            obj.setText("10")
            obj.textChanged.connect(self.redraw)

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
            ("max_iterations", "Number of\niterations", QLineEdit),
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
        try:
            vertexes = int(self.handlers["n"].text())
        except:
            pass

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

        if self.pheromone_matrix or self.distance_matrix:

            qp = QPainter()

            qp.begin(self)
            all_values_pheromones = sorted(reduce(lambda accum, x: accum + x, self.pheromone_matrix)) if self.pheromone_matrix else None
            all_values_distances = sorted(reduce(lambda accum, x: accum + x, self.distance_matrix)) if self.distance_matrix else None
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

            for x in range(vertexes):
                for y in range(vertexes):
                    pen = QPen(get_color(all_values_pheromones, self.pheromone_matrix[x][y]) if self.pheromone_matrix else Qt.black,
                               get_width(all_values_distances, self.distance_matrix[x][y]) if self.distance_matrix else 3,
                               Qt.SolidLine)
                    qp.setPen(pen)
                    qp.drawLine(QPoint(points[x].x, points[x].y), QPoint(points[y].x, points[y].y))

            qp.end()
            self.update()

    def create_options(self, ants_config):

        with open(ants_config, "w") as ants_file:
            ants_file.write("seed: " + "0" + '\n')

            ants_file.write("data_path: " + self.filename + '\n')
            ants_file.write("max_iterations: " + self.handlers["max_iterations"].text() + '\n')
            ants_file.write("alpha: " + self.handlers["alpha"].text() + '\n')
            ants_file.write("beta: " + self.handlers["beta"].text() + '\n')
            ants_file.write("k: " + self.handlers["k"].text() + '\n')
            ants_file.write("Q: " + self.handlers["Q"].text() + '\n')
            ants_file.write("start_pheromone: " + self.handlers["start_pheromone"].text() + '\n')

            ants_file.write("rho: " + str(self.handlers["rho"].value()) + '\n')

    def parse_logs(self, time_log, program_log):

        time_stats = "\n".join(open(time_log).readlines())

        with open(program_log) as log:

            best_cost = None
            best_path = None
            self.pheromone_matrix = []
            end = False
            best_path_next = False
            for line in log:

                if best_path_next:
                    best_path = line.strip()
                    continue
                if end and "Best cost" in line:
                    best_cost = int(line.split("=")[-1])
                    continue
                if end and "Best path" in line:
                    best_path_next = True
                    continue

                if "Ended" in line:
                    end = True
                    continue
                if not end:
                    continue

                if line != '\n' and "Pheromone matrix" not in line:
                    self.pheromone_matrix.append(list(map(float, line.strip()[:-1].split(","))))

        # best_path, best_cost, time_stats, stats
        return best_path, best_cost, time_stats, []

    def draw_graphics(self, stats):

        # temperature = list(map(lambda x: x['temperature'], stats))
        # avg_proba = list(map(lambda x: x['avg_probability'], stats))
        # cost = list(map(lambda x: x['cost'], stats))
        # iterations = [i for i in range(len(stats))]
        #
        # if self.handlers['remove old graphics'].isChecked():
        #     self.prob_graphic.clear()
        #     self.temp_graphic.clear()
        #     self.cost_graphic.clear()
        #
        # self.prob_graphic.getPlotItem().plot(temperature, avg_proba, pen='blue', width=10)
        # self.temp_graphic.getPlotItem().plot(iterations, temperature, pen='orange', width=10)
        # self.cost_graphic.getPlotItem().plot(temperature, cost, pen='red', width=10)
        pass

    def load_file(self):
        self.filename = QtWidgets.QFileDialog.getOpenFileName(self, 'Get adjacency matrix')[0]

        with open(self.filename) as file:
            self.distance_matrix = []
            n = int(file.readline().split(',')[0])
            for idx in range(n):
                self.distance_matrix.append(file.readline().strip().split(","))

        self.update()

    def redraw(self):
        if self.handlers['n'].text():
            self.update()

    def solve(self):

        if not self.filename:
            return

        if not os.path.exists(self.app_dir):
            os.mkdir(self.app_dir)

        ants_config = os.path.join(self.app_dir, "ants.conf")

        self.create_options(ants_config)

        runner = BinRunner("/home/emperornao/projects/optimization/ants_UI/ants")

        time_log = os.path.join(self.app_dir, "time.log")
        program_log = os.path.join(self.app_dir, "stdout.log")

        runner.run(ants_config, program_log, time_log)

        # parse results
        best_path, best_cost, time_stats, stats = self.parse_logs(time_log, program_log)

        text = [time_stats]
        text.append("Best founded path = " + " ".join(best_path) + "\n")
        text.append("Best cost = " + str(best_cost) + "\n")
        text = "".join(text)

        self.info.clear()
        self.info.setText(text)

        self.solution = best_path

        self.draw_graphics(stats)
