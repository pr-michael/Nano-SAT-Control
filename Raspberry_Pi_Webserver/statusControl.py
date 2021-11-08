import RPi.GPIO as GPIO
from flask import Flask, render_template, request
app = Flask(__name__)
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
# define sensors GPIOs
button = 20
senPIR = 16
# define actuators GPIOs
wire1 = 6
wire2 = 13
wire3 = 19
wire4 = 26
# initialize GPIO status variables
buttonSts = 0
senPIRSts = 0
wire1Sts = 0
wire2Sts = 0
wire3Sts = 0
wire4Sts = 0
# Define button and PIR sensor pins as an input
GPIO.setup(button, GPIO.IN)
GPIO.setup(senPIR, GPIO.IN)
# Define led pins as output
GPIO.setup(wire1, GPIO.OUT)
GPIO.setup(wire2, GPIO.OUT)
GPIO.setup(wire3, GPIO.OUT)
GPIO.setup(wire4, GPIO.OUT)
# turn leds OFF
GPIO.output(wire1, GPIO.LOW)
GPIO.output(wire2, GPIO.LOW)
GPIO.output(wire3, GPIO.LOW)
GPIO.output(wire4, GPIO.LOW)


@app.route("/")
def index():
    # Read GPIO Status
    buttonSts = GPIO.input(button)
    senPIRSts = GPIO.input(senPIR)
    wire1Sts = GPIO.input(wire1)
    wire2Sts = GPIO.input(wire2)
    wire3Sts = GPIO.input(wire3)
    wire4Sts = GPIO.input(wire4)
    templateData = {
        'button': buttonSts,
        'senPIR': senPIRSts,
        'wire1': wire1Sts,
        'wire2': wire2Sts,
        'wire3': wire3Sts,
        'wire4': wire4Sts,
    }
    return render_template('index.html', **templateData)


@app.route("/<deviceName>/<action>")
def action(deviceName, action):
    if deviceName == 'wire1':
        actuator = wire1
    if deviceName == 'wire2':
        actuator = wire2
    if deviceName == 'wire3':
        actuator = wire3
    if deviceName == 'wire4':
        actuator = wire4

    if action == "on":
        GPIO.output(actuator, GPIO.HIGH)
    if action == "off":
        GPIO.output(actuator, GPIO.LOW)

    buttonSts = GPIO.input(button)
    senPIRSts = GPIO.input(senPIR)
    wire1Sts = GPIO.input(wire1)
    wire2Sts = GPIO.input(wire2)
    wire3Sts = GPIO.input(wire3)
    wire4Sts = GPIO.input(wire4)

    templateData = {
        'button': buttonSts,
        'senPIR': senPIRSts,
        'wire1': wire1Sts,
        'wire2': wire2Sts,
        'wire3': wire3Sts,
        'wire4': wire4Sts,
    }
    return render_template('index.html', **templateData)


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=80, debug=True)
