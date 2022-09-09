from PyQt5 import QtWidgets
import sys

from ui import QueensUI


if __name__ == "__main__":

    app = QtWidgets.QApplication(sys.argv)
    app.setStyle('Fusion')

    view = QueensUI()

    view.showMaximized()

    try:
        sys.exit(app.exec_())
    except:
        sys.exit()