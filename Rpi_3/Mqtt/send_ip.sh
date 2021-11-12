#MQTT broker. If this isn´t up check https://github.com/mqtt/mqtt.github.io/wiki/public_brokers
MQTTHOST="iot.eclipse.org"

# Change this to become something unique, so that you get your own topic path
MQTTPREFIX="nona"

# Get interface name from parameters
if [ $# -eq 0 ]
then
 IFC="eth0"
else
 IFC="$1"
fi

ifconfig $IFC &> /dev/null
if [ $? -ne 0 ]
then
 exit 1
fi

while :
do
while [ -z $PRIVATE ]
do
# Get current private IP address for the selected interface
PRIVATE=$(ifconfig $IFC | grep "inet addr:" | awk '{ print $2 }')
done

/usr/bin/mosquitto_pub -h $MQTTHOST -t "$MQTTPREFIX" -m "$PRIVATE"

while [ $PRIVATE ]
do
PRIVATE=$(ifconfig $IFC | grep "inet addr:" | awk '{ print $2 }')
done
done