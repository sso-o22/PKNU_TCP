from flask import Flask, request, render_template
import RPi.GPIO as GPIO

app = Flask(__name__)

LED_PIN = 11
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)

@app.route('/')
def get():
   return render_template('button.html')

@app.route('/led/on', methods=['POST'])
def led_on():
   GPIO.output(LED_PIN, GPIO.HIGH)
   return render_template('button.html')

@app.route('/led/off', methods=['POST'])
def led_off():
   GPIO.output(LED_PIN, GPIO.LOW)
   return render_template('button.html')

if __name__=="__main__":
   app.run(host='0.0.0.0', port='8080', debug=True)
