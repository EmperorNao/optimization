from PyQt5 import QtWidgets
import sys

from ui import NNUi


if __name__ == "__main__":

    app = QtWidgets.QApplication(sys.argv)
    app.setStyle('Fusion')

    view = NNUi()

    view.showMaximized()

    try:
        sys.exit(app.exec_())
    except:
        sys.exit()