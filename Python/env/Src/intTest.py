import RPi.GPIO as GPIO
import time

swPin = 22

GPIO.setmode(GPIO.BCM)
GPIO.setup(swPin, GPIO.IN)

def callbackfunc(channel):
	print("Interrupt!!")

GPIO.add_event_detect(swPin, GPIO.RISING, callback=callbackfunc)

try:
	while True:
		pass
except KeyboardInterrupt:
	GPIO.cleanup()
