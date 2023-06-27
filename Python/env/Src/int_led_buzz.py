import RPi.GPIO as GPIO
import time

led_pin = 6
switch_pin = 22
buzzer_pin = 13

GPIO.setmode(GPIO.BCM)
GPIO.setup(led_pin, GPIO.OUT)
GPIO.setup(switch_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(buzzer_pin, GPIO.OUT)

buzz = GPIO.PWM(buzzer_pin, 440)

switch_state = False

def callbackfunc(channel):
    global switch_state
    switch_state = not switch_state

    print("Interrupt!!")

def siren_sound():
	melody = [1000, 500]

	for i in range(0, len(melody)):
		buzz.ChangeFrequency(melody[i])
		time.sleep(0.3)

GPIO.add_event_detect(switch_pin, GPIO.RISING, callback=callbackfunc, bouncetime=200)

try:
    while True:
      if switch_state:
      	buzz.start(50)
      	GPIO.output(led_pin, GPIO.HIGH)
      	siren_sound()
      else:
        GPIO.output(led_pin, GPIO.LOW)
        buzz.stop()

except KeyboardInterrupt:
    GPIO.cleanup()
