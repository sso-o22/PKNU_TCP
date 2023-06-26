import RPi.GPIO ad GPIO
import time

GPIO.setmode(GPIO.BCM)

btnPin = 22

segments = (15, 18, 23, 24, 25, 26, 27)

for segment in segments:
	GPIO.setup(segment, GPIO.OUT)
	GPIO.output(segment, 0)
	
num = {' ':(0,0,0,0,0,0,0),
			'0':(1,1,1,1,1,1,0),
			'1':(0,1,1,0,0,0,0),
			'2':(1,1,0,1,1,0,1),
			'3':(1,1,1,1,0,0,1),
			'4':(0,1,1,0,0,1,1),
			'5':(1,0,1,1,0,1,1),
			'6':(1,0,1,1,1,1,1),
			'7':(1,1,1,0,0,0,0),
			'8':(1,1,1,1,1,1,1),
			'9':(1,1,1,1,0,1,1)}
			
try:
	while True:
		n = time.ctime()[11:13]+time.ctime()[14:16]
		for loop in range(0, 7):
			GPIO.output(segments[loop])
			if ( 
