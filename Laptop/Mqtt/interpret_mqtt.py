import random
import webbrowser

from paho.mqtt import client as mqtt_client

# register browser firefox
webbrowser.register('firefox',
	None,
	webbrowser.BackgroundBrowser("C://Program Files//Mozilla Firefox//firefox.exe"))

# configure broker, port and topic
broker = 'test.mosquitto.org'
topic = "proj/nanoctrl" # mosquitto_sub -h test.mosquitto.org -t home/helolot/#
# lastMsg = ''

# generate client ID with pub prefix randomly
client_id = f'python-mqtt-{random.randint(0, 100)}'

def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    client.connect(broker)
    return client


def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        #? Method that prevents from opening the webserver twice
        # global lastMsg
        # if (msg.payload.decode() != lastMsg):
        #     lastMsg = msg.payload.decode()
        #     webbrowser.get('firefox').open(msg.payload.decode())

        webbrowser.get('firefox').open(msg.payload.decode())

    client.subscribe(topic)
    client.on_message = on_message


def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()


if __name__ == '__main__':
    run()