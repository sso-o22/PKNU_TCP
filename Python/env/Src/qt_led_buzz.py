import RPi.GPIO as GPIO
import sys
from PyQt5 import uic
from PyQt5.QtWidgets import *

Led_pin = 11
Buzzer_pin = 13

class qtApp(QWidget):
    def __init__(self):
        super().__init__()
        uic.loadUi('./mainApp.ui', self)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = qtApp()
    ex.show()
    sys.exit(app.exec())