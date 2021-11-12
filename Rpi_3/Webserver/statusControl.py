import RPi.GPIO as GPIO
from flask import Flask, render_template, request
app = Flask(__name__)
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

# define GPIOs
wire1 = 19
wire2 = 26

# initialize GPIO status variables
wire1Sts = 0
wire2Sts = 0

# Define wire pins as output
GPIO.setup(wire1, GPIO.OUT)
GPIO.setup(wire2, GPIO.OUT)

# turn wires OFF
GPIO.output(wire1, GPIO.LOW)
GPIO.output(wire2, GPIO.LOW)


@app.route("/")
def index():

    # Read GPIO Status
    wire1Sts = GPIO.input(wire1)
    wire2Sts = GPIO.input(wire2)
    templateData = {
        'wire1': wire1Sts,
        'wire2': wire2Sts,
    }
    return render_template('index.html', **templateData)


@app.route("/<deviceName>/<action>")
def action(deviceName, action):
    if deviceName == 'wire1':
        actuator = wire1
    if deviceName == 'wire2':
        actuator = wire2

    if action == "on":
        GPIO.output(actuator, GPIO.HIGH)
    if action == "off":
        GPIO.output(actuator, GPIO.LOW)

    wire1Sts = GPIO.input(wire1)
    wire2Sts = GPIO.input(wire2)

    templateData = {
        'wire1': wire1Sts,
        'wire2': wire2Sts,
    }
    return render_template('index.html', **templateData)


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=80, debug=True)
