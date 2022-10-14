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


class GeneticUI(QtWidgets.QMainWindow):

    def __init__(self):
        super().__init__()

        self.setWindowTitle("Genetic commivoyager")
        self.setFixedSize(1500, 900)

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
        self.app_dir = "/home/{user}/.genetic".format(user=os.getlogin())
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
            obj.setText("9")
            obj.textChanged.connect(self.redraw)

        elif name == "max_iterations":
        # QLineEdit widget
            int_validator = QIntValidator()
            int_validator.setRange(1, 1000)
            obj.setValidator(int_validator)
            obj.setText("100")


        elif name == "population_cardinality":
        # QLineEdit widget
            int_validator = QIntValidator()
            int_validator.setRange(1, 100)
            obj.setValidator(int_validator)
            obj.setText("100")

        elif name == "p_c":
        # QDoubleSpinBox widget
            obj.setDecimals(2)
            obj.setSingleStep(0.01)
            obj.setRange(0, 1)
            obj.setValue(0.5)

        elif name == "p_m":
        # QDoubleSpinBox widget
            obj.setDecimals(2)
            obj.setSingleStep(0.01)
            obj.setRange(0, 1)
            obj.setValue(0.05)


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
            ("p_c", "Crossover\nprobability", QDoubleSpinBox),
            ("p_m", "Mutation\nprobability", QDoubleSpinBox),
            ("population_cardinality", "Population\ncardinality", QLineEdit),
            ("remove_old_graphics", "Remove old\ngraphics", QCheckBox),
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

        self.cost_parent_widget = QWidget()
        self.cost_parent_widget.setMinimumHeight(450)

        self.best_cost_graphic = pg.PlotWidget(self.cost_parent_widget)
        self.avg_cost_graphic = pg.PlotWidget(self.cost_parent_widget)

        self.best_cost_graphic.setBackground('w')
        self.best_cost_graphic.setLabel('left', 'best_cost')
        self.best_cost_graphic.setLabel('bottom', 'iterations')

        self.avg_cost_graphic.setBackground('w')
        self.avg_cost_graphic.setLabel('left', 'avg_cost')
        self.avg_cost_graphic.setLabel('bottom', 'iterations')

        self.right_layout.addWidget(self.best_cost_graphic)
        self.right_layout.addWidget(self.avg_cost_graphic)
        self.right_layout.addWidget(self.cost_parent_widget)

    def paintEvent(self, event=None):

        try:
            vertexes = int(self.handlers["n"].text())
        except:
            pass

        if not vertexes:
            return

        point_radius = 10
        orig_size = 700
        graph_radius = 300

        center_point = Point(750, 450)

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
                width = [1 + 1.5 ** i for i in range(5)]

                if len(all_values) < len(width):
                    return width[-all_values.find(value)]

                idx = all_values.index(value)
                step = math.ceil(len(all_values) / len(width))
                colors_idx = idx // step
                return width[-colors_idx]

            for x in range(vertexes):
                for y in range(vertexes):
                    pen = QPen(get_color(all_values_pheromones, self.pheromone_matrix[x][y]) if self.pheromone_matrix else Qt.black,
                               get_width(all_values_distances, self.distance_matrix[x][y]) if self.distance_matrix else 1,
                               Qt.SolidLine)
                    qp.setPen(pen)
                    qp.drawLine(QPoint(points[x].x, points[x].y), QPoint(points[y].x, points[y].y))

            qp.end()
            self.update()

    def create_options(self, genetic_config):

        with open(genetic_config, "w") as genetic_file:
            genetic_file.write("seed: " + "0" + '\n')

            genetic_file.write("data_path: " + self.filename + '\n')
            genetic_file.write("n: " + self.handlers["n"].text() + '\n')
            genetic_file.write("max_iterations: " + self.handlers["max_iterations"].text() + '\n')
            genetic_file.write("p_m: " + str(self.handlers["p_c"].value()) + '\n')
            genetic_file.write("p_c: " + str(self.handlers["p_m"].value()) + '\n')
            genetic_file.write("population_cardinality: " + self.handlers["population_cardinality"].text() + '\n')

    def parse_logs(self, time_log, program_log):

        time_stats = "\n".join(open(time_log).readlines())

        with open(program_log) as log:

            best_cost = None
            best_path = None
            best_path_next = False
            best_cost_by_iter = []
            avg_cost_by_iter = []
            end = False
            for line in log:

                if "Avg" in line:
                    avg_cost_by_iter.append(float(line.split(":")[-1]))
                if best_path_next and not best_path:
                    best_path = line.strip().split(" ")
                    continue
                if end and "Best cost" in line:
                    best_cost = int(line.split(":")[-1])
                    continue
                if end and "Best solution" in line:
                    best_path_next = True
                    continue
                if "Best cost" in line:
                    best_cost_by_iter.append(int(line.split(":")[-1]))
                    continue
                if "Ended" in line:
                    end = True
                    continue
                if not end:
                    continue

        # best_path, best_cost, time_stats, stats
        return best_path, best_cost, time_stats, {"best_cost_iter": best_cost_by_iter, "avg_cost_iter": avg_cost_by_iter}

    def draw_graphics(self, stats):

        best_cost = stats["best_cost_iter"]
        avg_cost = stats["avg_cost_iter"]
        iterations = [i for i in range(len(best_cost))]

        if self.handlers['remove_old_graphics'].isChecked():
            self.best_cost_graphic.clear()
            self.avg_cost_graphic.clear()

        self.best_cost_graphic.getPlotItem().plot(iterations, best_cost, pen='blue', width=10)
        self.avg_cost_graphic.getPlotItem().plot(iterations, avg_cost, pen='blue', width=10)

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

        genetic_config = os.path.join(self.app_dir, "genetic.conf")

        self.create_options(genetic_config)

        runner = BinRunner("/home/emperornao/projects/optimization/genetic_UI/genetic")

        time_log = os.path.join(self.app_dir, "time.log")
        program_log = os.path.join(self.app_dir, "stdout.log")

        runner.run(genetic_config, program_log, time_log)

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
