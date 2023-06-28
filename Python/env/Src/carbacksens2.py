import RPi.GPIO as GPIO
import time

# 초음파 센서와 부저에 연결된 핀 번호
trig_pin = 18
echo_pin = 24
buzzer_pin = 12

# GPIO 초기화
GPIO.setmode(GPIO.BCM)
GPIO.setup(trig_pin, GPIO.OUT)
GPIO.setup(echo_pin, GPIO.IN)
GPIO.setup(buzzer_pin, GPIO.OUT)

def measure_distance():
    # 초음파 발신을 위해 트리거 핀을 10us 동안 활성화
    GPIO.output(trig_pin, GPIO.HIGH)
    time.sleep(0.00001)
    GPIO.output(trig_pin, GPIO.LOW)
    
    # 에코 핀이 HIGH로 변할 때까지 시간 측정
    start_time = time.time()
    while GPIO.input(echo_pin) == GPIO.LOW:
        start_time = time.time()
        
    end_time = time.time()
    while GPIO.input(echo_pin) == GPIO.HIGH:
        end_time = time.time()
        
    # 거리 계산 (소리의 속도: 343m/s, 거리 = 속도 x 시간 / 2)
    duration = end_time - start_time
    distance = (34300 * duration) / 2
    
    return distance

try:
    while True:
        dist = measure_distance()
        print("Distance:", dist, "cm")
        
        # 거리에 따라 부저 속도 조절
        if dist < 20:
            frequency = 1000 - (dist * 50)  # 주파수 계산
            speed = (20 - dist) / 20  # 속도 계산
            duration = speed * 0.1  # 속도에 따른 딜레이 계산
            p = GPIO.PWM(buzzer_pin, frequency)
            p.start(50)
            time.sleep(duration)
            p.stop()
        else:
            GPIO.output(buzzer_pin, GPIO.LOW)
            
        time.sleep(0.1)

except KeyboardInterrupt:
    GPIO.cleanup()
