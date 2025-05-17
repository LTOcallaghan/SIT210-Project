import time
import ssl
import os
import paho.mqtt.client as mqtt
from gpiozero import AngularServo
from dotenv import load_dotenv

# load .env variables
load_dotenv()

# MQTT config from .env
broker = os.getenv('MQTT_BROKER')
port = 8883
topic = 'sensor/servo'
username = os.getenv('MQTT_USERNAME')
password = os.getenv('MQTT_PASSWORD')

# set up servo motor on pin gpio 18
servo = AngularServo(18, min_pulse_width=0.0006, max_pulse_width=0.0023)

# variables 
cooldown_until = 0  # timestamp
messages = []

# attempt to connect to MQTT
def on_connect(client, userdata, flags, rc):
    global connected
    if rc == 0:
        print("Connected to MQTT broker")
        client.subscribe(topic)
    else:
        print(f"Failed to connect, error code: {rc}")


# When receiving a message decode it and print it out, if it is "true", spin the servo
# After the time passes (and a new message is received to trigger), unspin it 
def on_message(client, userdata, msg):
    global cooldown_until
    message = msg.payload.decode().lower()
    print(f"received message: {message}")
    now = time.time()
    
    # when the next message comes, if cooldown is over, then reopen the blinds (reset servo)
    if now >= cooldown_until and cooldown_until > 0:
        print("1 hour has passed, reopening blinds")
        reset_servo()
        cooldown_until = 0
        print("")
    else:
        # if on cooldown (keeping blinds closed for 1h)
        if now < cooldown_until:
            print(f"No new messages being processed until 1 hour passes")
            print("")
        else: 
        # if it is not on cooldown, and the blinds are open, if true message: close the blinds and set cooldown for an hour (no messages processed for an hour). 
        # If false message: do nothing
                
            if message == "true":
                print("Servo activated and closing blinds for an hour")
                move_servo()
                cooldown_until = now + 10 # 3600 is 1 hour, set to 10s for testing
                print("")
            elif message == "false":
                print("Message ignored, brightness level too low")
                print("")


def move_servo():
    print("Rotating servo 90 degree")
    servo.angle = 90
    time.sleep(1)

def reset_servo():
    print("Returning servo to base")
    servo.angle = -90
    time.sleep(1)

# setup mqtt client
client = mqtt.Client()
client.username_pw_set(username, password)
client.tls_set(tls_version=ssl.PROTOCOL_TLS)
client.on_connect = on_connect
client.on_message = on_message

# run
try:
    client.connect(broker, port, 60)
    client.loop_forever()
except KeyboardInterrupt:
    print("Program interrupted by press")
