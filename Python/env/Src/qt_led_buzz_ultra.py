import sys
from PyQt5 import QtGui, QtWidgets, uic
from PyQt5.QtWidgets import *
import RPi.GPIO as GPIO
from PyQt5.QtCore import *
import time

flag = False
sw = False
led = 11
buzzPin = 13
TRIG = 18
ECHO = 15
print("초음파 거리 측정기")

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

GPIO.setup(led,GPIO.OUT)
GPIO.setup(buzzPin,GPIO.OUT)
GPIO.setup(TRIG,GPIO.OUT)
GPIO.setup(ECHO,GPIO.IN)

buzz=GPIO.PWM(buzzPin,440)
GPIO.output(TRIG, False)
print("초음파 출력 초기화")
time.sleep(2)

class Thread1(QThread):
    global flag
    def __init__(self, parent):
        super().__init__(parent)
        self.parent = parent

    def run(self):
        try:
            while flag:
                GPIO.output(TRIG, True)
                time.sleep(0.00001)
                GPIO.output(TRIG,False)

                while GPIO.input(ECHO)==0:
                    start = time.time()

                while GPIO.input(ECHO)==1:
                    stop = time.time()

                cheak_time = stop - start
                distance = cheak_time * 34300 / 2
                print("Distance : %.1f cm " %distance)
                time.sleep(0.4)   

        except KeyboardInterrupt:
            print("거리측정완료")

    def stop(self):
        self.power = False
        self.quit()
        self.wait(1000)

class qtApp(QWidget):

    def __init__(self):
        super().__init__()
        uic.loadUi('./mainApp.ui', self)

        # Qt Designer에서 구성한 위젯 시그널
        self.btn_led.clicked.connect(self.btnLEDClicked)
        self.btn_distan.clicked.connect(self.btnDISTANClicked)
        self.btn_buzz.clicked.connect(self.btnBUZZClicked)

    def btnLEDClicked(self): # 슬롯함수
        global sw
        if(sw == False):
            GPIO.output(led,True)
            sw = True
        else:
            GPIO.output(led,False)
            sw = False

    def btnDISTANClicked(self):
        global flag
        h1 = Thread1(self)
        if(flag == False):
            h1.start()
            flag = True
        else:
            h1.stop()
            flag = False

    def btnBUZZClicked(self):
        buzz.start(50)
        buzz.ChangeFrequency(1000)
        time.sleep(1)
        buzz.stop()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = qtApp()
    ex.show()
    sys.exit(app.exec_())