from flask import Flask, render_template
import RPi.GPIO as GPIO

app = Flask(__name__)
GPIO.setmode(GPIO.BCM)
led_pin = 11

# 웹 서버의 루트 페이지에 대한 요청을 처리하는 함수
@app.route('/')
def index():
    return render_template('index.html')

# 버튼 클릭 시 LED를 토글하는 함수
@app.route('/toggle')
def toggle():
    GPIO.setup(led_pin, GPIO.OUT)
    GPIO.output(led_pin, not GPIO.input(led_pin))
    return ''

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080, debug=True)
