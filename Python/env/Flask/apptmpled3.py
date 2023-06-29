from flask import Flask, render_template, request
import RPi.GPIO as GPIO

app = Flask(__name__)
GPIO.setmode(GPIO.BCM)
led_pin = 11

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/toggle')
def toggle():
    state = request.args.get('state', '')
    GPIO.setup(led_pin, GPIO.OUT)
    
    if state == 'on':
        GPIO.output(led_pin, GPIO.HIGH)
    elif state == 'off':
        GPIO.output(led_pin, GPIO.LOW)
    
    return ''

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080, debug=True)
