from PyQt5 import QtWidgets
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
import pyqtgraph as pg
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QGridLayout
from PyQt5.QtWidgets import QPushButton
from datetime import datetime


class QueensUI(QtWidgets.QMainWindow):
    """
    Class that provides all UI components and signals to work with them, also it builds main logical dependencies
    in plotter and parser
    """

    def __init__(self):
        super().__init__()

        self.setWindowTitle("Navigation")
        self.setFixedSize(1920, 1080)

        # main layout and main widget
        self.general_layout = QHBoxLayout()
        self.centralWidget = QtWidgets.QWidget(self)
        self.setCentralWidget(self.centralWidget)
        self.centralWidget.setLayout(self.general_layout)

        # init inclusive layouts
        self._init_left_layout()
        self._init_center_layout()
        self._init_right_layout()


    def _init_left_layout(self):

        # left
        self.left_layout = QGridLayout()
        self.general_layout.addLayout(self.left_layout)

        self.handlers = dict()

        # name, description, object, func
        handlers = [
            ("max iterations", "Maximum number of iterations", QLineEdit),
            ("start temperature", "Temperature to start", QLineEdit),
            ("min temperature", "Minimal temperature", QLineEdit),
            ("fixed temperature iterations", "Number of iterations with", QLineEdit),
            ("decreasing rule", "Rule to decrease temperature", QComboBox),
            ("decreasing coefficient", "Exponential coefficient", QDoubleSpinBox),
            ("energy difference coefficient", "Rule to count difference between energies", QComboBox),
        ]

        for name, description, object, func in handlers:
            self.handlers[name] = func(object)
            self.layout.addWidget()


        self.left_layout.addWidget(self.label_info, 0, 0)
        self.left_layout.addWidget(self.content, 1, 0)

    def _init_center_layout(self):
        """
        Do not call this method, it is for internal usage in __init__
        to build center part of NavigationUI
        :return:
        """
        self.central_layout = QGridLayout()
        self.general_layout.addLayout(self.central_layout, 0, 4)

        self.import_btn = QPushButton("Import")
        # self.import_btn.clicked.connect(self.import_btn)

        self.number = QLCDNumber()
        self.spinbox = QSpinBox()
        self.double_spinbox = QDoubleSpinBox()
        self.double_spinbox.setDecimals(2)

        self.line = QLineEdit()

        onlyInt = QIntValidator()
        onlyInt.setRange(1, 1000000)
        self.line.setValidator(onlyInt)

        self.label = QLabel()
        self.label.setText("Очень весело")


        self.format_selection = QComboBox()

        self.central_layout.addWidget(self.import_btn, 0, 0)
        self.central_layout.addWidget(self.format_selection, 8, 0)
        self.central_layout.addWidget(self.number, 4, 0)
        self.central_layout.addWidget(self.label, 3, 0)
        self.central_layout.addWidget(self.line, 1, 0)
        self.central_layout.addWidget(self.double_spinbox, 2, 0)
        self.central_layout.addLayout(QGridLayout(), 18, 0, 6, 1)

    def _init_right_layout(self):
        """
        Do not call this method, it is only for internal usage in __init__
        to build right part of NavigationUI
        :return:
        """
        self.right_layout = QGridLayout()
        self.general_layout.addLayout(self.right_layout, 0, 5, 7, 7)


        self.graph_widget = pg.PlotWidget()
        self.graph_widget.setBackground('w')

        self.graph_widget.setLabel('left', '')
        self.graph_widget.setLabel('bottom', '')


        self.right_layout.addWidget(self.graph_widget, 0, 0, 5, 5)
