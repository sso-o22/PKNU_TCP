import RPi.GPIO as GPIO
import time

# GPIO 핀 번호 설정
switch_pin = 22
segment_pins = [15, 18, 23, 24, 25, 26, 27]

# GPIO 모드 설정
GPIO.setmode(GPIO.BCM)

# 스위치 핀 입력 설정
#GPIO.setup(switch_pin, GPIO.IN)
GPIO.setup(switch_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)

# 7세그먼트 핀 출력 설정
for pin in segment_pins:
    GPIO.setup(pin, GPIO.OUT)
    GPIO.output(pin, GPIO.LOW)
   #GPIO.output(pin, GPIO.HIGH)

# 숫자 패턴 배열
number_patterns = [
		[1, 1, 1, 0, 1, 1, 1], # 0
		[0, 0, 1, 0, 0, 0, 1], # 1
		[1, 1, 0, 1, 0, 1, 1], # 2
		[0, 1, 1, 1, 0, 1, 1], # 3
		[0, 0, 1, 1, 1, 0, 1], # 4
		[0, 1, 1, 1, 1, 1, 0], # 5
		[1, 1, 1, 1, 1, 0, 0], # 6
		[0, 0, 1, 0, 0, 1, 1], # 7
		[1, 1, 1, 1, 1, 1, 1], # 8
		[0, 0, 1, 1, 1, 1, 1], # 9
		[1, 0, 1, 1, 1, 1, 1], # A
		[1, 1, 1, 1, 1, 1, 1], # B
		[1, 1, 0, 0, 1, 1, 0], # C
		[1, 1, 1, 0, 1, 1, 1], # D
		[1, 1, 0, 1, 1, 1, 0], # E
		[1, 0, 0, 1, 1, 1, 0]  # F
]

# 초기 숫자와 카운터 변수 설정
number = 0
counter = 0

try:
    while True:
        # 스위치가 눌리면 카운터 변수 증가
        if GPIO.input(switch_pin) == GPIO.LOW:
            counter += 1
            time.sleep(0.2)  # 디바운싱을 위한 딜레이

        # 카운터가 16 이상이면 0으로 초기화
        if counter >= 16:
            counter = 0

        # 현재 숫자 표시
        number = counter

        # 7세그먼트 출력
        pattern = number_patterns[number]
        for i, pin in enumerate(segment_pins):
            GPIO.output(pin, pattern[i])

except KeyboardInterrupt:
    GPIO.cleanup()
