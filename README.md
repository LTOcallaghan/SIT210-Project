# SIT210-Project

## Libraries for Arduino that are required to be installed
```
 BH1750 - the light sensor
	
 WiFiNINA - about connecting to wifi
	
 PubSubClient - about MQTT
```
 
 
 ## To install everything from the top for the Raspberry Pi
 
Install Python with
```
sudo apt install python3 -y
```
Install pip with and a virtual environment with 
```
sudo apt install python3-pip python3-venv -y
```
To create and activate a virtual environment (this is required as the pip commands didn't like being done normally)
```
python3 -m venv virt-env --system-site-packages
source virt-env/bin/activate
```

Then install paho mqtt to allow mqtt, and dotenv to allow .env file compatability
```
pip install paho-mqtt
pip install python-dotenv
```

Once everything has been installed, to set up the Raspberry Pi, create a .env file within the same location as the code file. This will contain this, with your own values added after the equals sign.
```
# MQTT Broker
MQTT_BROKER =

# MQTT Username
MQTT_USERNAME =

# MQTT Password
MQTT_PASSWORD =  
```

The Arduino also requires a secrets.h file, this is similar to the .env file. This will contain this code, add your own values added in between the quotation marks ("").
```
#ifndef SECRETS_H
#define SECRETS_H
const char* SECRET_SSID = "";      // wifi name    
const char* SECRET_PASS = "";  // wifi password
const char* SECRET_MQTT_SERVER = "";  // mqtt broker
const char* SECRET_MQTT_USER = ""; // mqtt username
const char* SECRET_MQTT_PASSWORD = ""; //mqtt password
#endif
```



## Node-red

To install node-red, you must first install nodejs via https://nodejs.org/en/

Then run the following command in cmd to install node-red
```
npm install -g --unsafe-perm node-red
```

To open node-red, open command prompt (cmd), type "node-red", and open the location is says in browser, which is likely http://127.0.0.1:1880

You must also set up node-red and create a flow consisting of an mqtt subscription to sensor/light, into a function, into a mqtt publish to sensor/servo
The function contains

```
var light_status = msg.payload;
var light_rating = false;
if (light_status > 150) {
    light_rating = true;
}
msg.payload = light_rating;
return msg;
```

This should look like this
![image](https://github.com/user-attachments/assets/89f4e2b6-cba5-4295-a0ac-336d9c5fc74a)


### Connecting the MQTT nodes to MQTT
Double click to open the menu

Click the pencil to open the mqtt broker settings


Input these values:

**Connection**
```
 
Server: The MQTT Broker 
   
Port: The same port as used before
     
Connect automatically ✅
       
Use TLS ✅
```
	 
**Security**
```
 
Username: The MQTT Username
   
Password: The MQTT Password
```

Then set the server to the newly created one for both of the nodes


Edit the topic for the IN node to 
```
sensor/light
```
set QoS to 2



Edit the topic for the OUT node to 
```
sensor/servo
```
set QoS to 2
