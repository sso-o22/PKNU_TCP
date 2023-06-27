import RPi.GPIO as GPIO
import time

led_pin = 6
switch_pin = 22

GPIO.setmode(GPIO.BCM)
GPIO.setup(led_pin, GPIO.OUT)
GPIO.setup(switch_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)

switch_state = False

def callbackfunc(channel):
    global switch_state
    switch_state = not switch_state

    print("Interrupt!!")

GPIO.add_event_detect(switch_pin, GPIO.RISING, callback=callbackfunc, bouncetime=200)

try:
    while True:
        if switch_state:
        	GPIO.output(led_pin, GPIO.HIGH)
        else:
        	GPIO.output(led_pin, GPIO.LOW)

except KeyboardInterrupt:
    GPIO.cleanup()
