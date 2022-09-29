import os
import math
import numpy as np

from PyQt5 import QtWidgets
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5.QtWidgets import QGridLayout
from PyQt5.QtWidgets import QPushButton

import pyqtgraph as pg

from functools import reduce

from nn.nn import FullyConnectedNN
from nn.functional import softmax

import cv2


class NNUi(QtWidgets.QMainWindow):

    def __init__(self):
        super().__init__()

        self.setWindowTitle("Digits classification")
        self.setFixedSize(1000, 600)

        # main layout and main widget
        self.general_layout = QHBoxLayout()
        self.centralWidget = QtWidgets.QWidget(self)
        self.setCentralWidget(self.centralWidget)
        self.centralWidget.setLayout(self.general_layout)

        self.params = "/home/emperornao/projects/optimization/nn_py/bestmodel.bin"
        self.nn = FullyConnectedNN({})
        self.nn.load(self.params)

        self._init_layout()
        self.probabilities = None


    def _init_layout(self):
        # left
        self.left_layout = QGridLayout()
        self.general_layout.addLayout(self.left_layout)

        self.label = QLabel()

        self.left_layout.addWidget(self.label, 0, 0)

        self.load_file_button = QPushButton()
        self.load_file_button.setMaximumWidth(200)
        self.load_file_button.setText("Load matrix")
        self.load_file_button.clicked.connect(self.load_file)
        self.left_layout.addWidget(self.load_file_button, 2, 0)

        self.plot_widget = QWidget()
        self.plot_widget.setMinimumHeight(400)
        self.plot_widget.setMinimumWidth(400)
        self.left_layout.addWidget(self.plot_widget, 1, 1, 2, 2)


    def paintEvent(self, event=None):

        if self.probabilities is not None:

            start_coord_x = 370
            start_coord_y = 100
            step = 30
            width = 30

            qp = QPainter()
            qp.begin(self)

            black_brush = QBrush(QColor(0, 0, 0))

            # for x in range(10):
            for x in range(10):
                qp.setBrush(black_brush)

                pos = QRect(start_coord_x + x * step + x * width,
                            start_coord_y,
                            width,
                            int(200 * self.probabilities[x])
                            )
                qp.drawRect(pos)

                text_pos = QPoint(start_coord_x + x * step + x * width,
                            start_coord_y - 20
                            )
                qp.drawText(text_pos, "{:2.2f}%".format(self.probabilities[x] * 100))

                text_pos = QPoint(start_coord_x + x * step + x * width,
                                  start_coord_y - 40
                                  )
                qp.drawText(text_pos, str(x))

            qp.end()


    def load_file(self):

        self.filename = QtWidgets.QFileDialog.getOpenFileName(self, 'Get image')[0]

        im = QPixmap(self.filename)
        im = im.scaled(300, 300)
        self.label.setPixmap(im)

        self.classification()

    def classification(self):
        self.probabilities = [0.1, 0.25, 0.05, 0, 0.2, 0.1, 0.3, 0.5, 0.3, 0.1]

        img = cv2.imread(self.filename, cv2.IMREAD_UNCHANGED)

        data = np.expand_dims(np.array(img).flatten(), axis=0)

        output = self.nn(data, eval=True)
        self.probabilities = softmax(output).flatten()

        self.update()

