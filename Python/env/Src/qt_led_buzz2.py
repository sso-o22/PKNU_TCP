import RPi.GPIO as GPIO
import sys
import time
from PyQt5 import uic
from PyQt5.QtWidgets import *
from PyQt5.QtCore import Qt, QThread, pyqtSignal

flag = False
LED_PIN = 11
BUZZER_PIN = 13
TRIG_PIN = 18
ECHO_PIN = 15

GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)
GPIO.setup(BUZZER_PIN, GPIO.OUT)
GPIO.setup(TRIG_PIN, GPIO.OUT)
GPIO.setup(ECHO_PIN, GPIO.IN)

class GpioThread(QThread):
    button_pressed = pyqtSignal(int)
    distance_updated = pyqtSignal(float)

    def __init__(self):
        super().__init__()

    def run(self):
        GPIO.setmode(GPIO.BCM)
        try:
            while True:
                pass
        except KeyboardInterrupt:
            GPIO.cleanup()

    def button_callback(self, channel):
        self.button_pressed.emit(channel)

class Thread1(QThread):
    distance_updated = pyqtSignal(float)

    def __init__(self):
        super().__init__()

    def run(self):
        try:
            while flag:
                GPIO.output(TRIG_PIN, True)
                time.sleep(0.00001)
                GPIO.output(TRIG_PIN, False)

                while GPIO.input(ECHO_PIN)==0:
                    start = time.time()

                while GPIO.input(ECHO_PIN)==1:
                    stop = time.time()

                check_time = stop - start
                distance = check_time * 34300 / 2
                print("Distance: %.1f cm" % distance)

                self.distance_updated.emit(distance)
                time.sleep(0.4)

        except KeyboardInterrupt:
            print("거리측정완료")

    def stop(self):
        global flag
        flag = False
        self.quit()
        self.wait(1000)

class MainWindow(QWidget):
    def __init__(self):
        super().__init__()

        self.led_state = False
        self.buzzer_state = False
        self.ultrasonic_state = False

        # UI 파일 불러오기
        uic.loadUi('./mainApp.ui', self)

        # 버튼 시그널 / 슬롯함수 지정
        self.BtnOn.clicked.connect(self.BtnOnClicked)
        self.BtnOff.clicked.connect(self.BtnOffClicked)
        self.BtnBuzzer.clicked.connect(self.BtnBuzzerClicked)
        self.BtnUltraSonic.clicked.connect(self.BtnUltraSonicClicked)

    def BtnOnClicked(self):
        GPIO.output(LED_PIN, GPIO.HIGH)
    
    def BtnOffClicked(self):
        GPIO.output(LED_PIN, GPIO.LOW)

    def BtnBuzzerClicked(self):
        self.buzzer_state = not self.buzzer_state
        if self.buzzer_state:
            buzz.start(50)  # duty cycle 50으로 pwm 출력 시작
            GPIO.output(BUZZER_PIN, GPIO.HIGH)
            self.BtnBuzzer.setText("Buzzer Off")
        else:
            GPIO.output(BUZZER_PIN, GPIO.LOW)
            buzz.stop()  # pwm 종료
            self.BtnBuzzer.setText("Buzzer On")

    def BtnUltraSonicClicked(self):
        global flag
        if not flag:
            flag = True
            self.thread1 = Thread1()
            self.thread1.distance_updated.connect(self.update_distance)
            self.thread1.start()
        else:
            self.thread1.stop()

    def update_distance(self, distance):
        self.LblDistance.setText("Distance: %.1f cm" % distance)

if __name__ == "__main__":
    app = QApplication(sys.argv)

    buzz = GPIO.PWM(BUZZER_PIN, 440)  # 440Hz를 갖는 객체 생성

    gpio_thread = GpioThread()
    gpio_thread.button_pressed.connect(lambda channel: print(f"Button pressed: {channel}"))

    main_window = MainWindow()
    main_window.show()

    gpio_thread.start()

    sys.exit(app.exec_())
