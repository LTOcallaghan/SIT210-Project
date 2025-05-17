# SIT210-Project

Libraries for Arduno that are required to be installed

 BH1750 - the light sensor
	
 WiFiNINA - about connecting to wifi
	
 PubSubClient - about MQTT

 
	To install everything from the top for the Raspberry Pi
	
	Install Python with
		sudo apt install python3 -y
	Install pip with
		sudo apt install python3-pip python3-venv -y
	
	To create and activate a virtual environment (this is required as the pip commands didn't like being done normally)
		sudo apt install python3-full python3-venv -y
		python3 -m venv virt-env
		source virt-env/bin/activate
	
	Then install paho mqtt to allow mqtt, and dotenv to allow .env file compatability
		pip install paho-mqtt 
		pip install python-dotenv
	
Once everything has been installed, to set up the Raspberry Pi, create a .env file within the same location as the code file. This will contain:


MQTT_BROKER=
MQTT_USERNAME=
MQTT_PASSWORD=


The Arduino also requires a secrets.h file, this is similar to the .env file. This will contain this code, with the values added in. 


#ifndef SECRETS_H
#define SECRETS_H
const char* SECRET_SSID = "";      // wifi name    
const char* SECRET_PASS = "";  // wifi password
const char* SECRET_MQTT_SERVER = "";  // mqtt broker
const char* SECRET_MQTT_USER = ""; // mqtt username
const char* SECRET_MQTT_PASSWORD = ""; //mqtt password
#endif





To install node-red, you must first install nodejs via https://nodejs.org/en/
Then run the following command in cmd to install node-red
npm install -g --unsafe-perm node-red


You must also set up node-red and create a flow consisting of an mqtt subscription to sensor/light, into a function, into a mqtt publish to sensor/servo
The function contains

var light_status = msg.payload;
var light_rating = false;
if (light_status > 150) {
    light_rating = true;
}
msg.payload = light_rating;
return msg;

This should look like this
![image](https://github.com/user-attachments/assets/89f4e2b6-cba5-4295-a0ac-336d9c5fc74a)


To connect the MQTT bricks to MQTT, 
