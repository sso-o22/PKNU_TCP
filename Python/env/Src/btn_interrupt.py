# btn_interrupt.py
import RPi.GPIO as GPIO
import time
GPIO.setmode(GPIO.BCM)
count = 0
def handler(channel):
	global count
	count = 
