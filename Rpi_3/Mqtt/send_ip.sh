#MQTT broker. If this isn´t up check https://github.com/mqtt/mqtt.github.io/wiki/public_brokers
MQTTHOST="test.mosquitto.org"

# Change this to become something unique, so that you get your own topic path
MQTTPREFIX="proj/nanoctrl"

# Get interface name from parameters
if [ $# -eq 0 ]
then
 IFC="wlan0"
else
 IFC="$1"
fi

ifconfig $IFC &> /dev/null
if [ $? -ne 0 ]
then
 exit 1
fi

PRIVATE=$(ifconfig $IFC | grep "inet " | awk '{ print $2 }')

while :
do
while [ -z $PRIVATE ]
do
# Get current private IP address for the selected interface
PRIVATE=$(ifconfig $IFC | grep "inet " | awk '{ print $2 }')
done

/usr/bin/mosquitto_pub -h $MQTTHOST -t "$MQTTPREFIX" -m "ip-a"

while [ $PRIVATE ]
do
# Get current private IP address for the selected interface
PRIVATE=$(ifconfig $IFC | grep "inet " | awk '{ print $2 }')
done
done