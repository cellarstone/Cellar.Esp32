/************* CellarConfigManagement.cpp *************/
#include "CellarConfigManagement.h"

int _EE_WIFI_SSID_ADDR;
int _EE_WIFI_SSID_SIZE;

int _EE_WIFI_PASS_ADDR;
int _EE_WIFI_PASS_SIZE;
int _EE_DEVICE_NAME_ADDR;
int _EE_DEVICE_NAME_SIZE;

int _EE_MQTT_SERVER_ADDR;
int _EE_MQTT_SERVER_SIZE;

int _EE_MQTT_TOPIC_ADDR;
int _EE_MQTT_TOPIC_SIZE;

int _EE_MQTT_USER_NAME_ADDR;
int _EE_MQTT_USER_NAME_SIZE;

int _EE_MQTT_USER_PASS_ADDR;
int _EE_MQTT_USER_PASS_SIZE;

int _EE_MQTT_PORT_ADDR;

/* IP address of MQTT server*/
String MqttHostName;

/* Port of MQTT server */
int MqttPort;

/* MQTT user */
String MqttUserName;

/* MQTT password */
String MqttUserPass;

/* MQTT topic name */
String MqttTopicName;

/* device name */
String DeviceName;

/* name of WIFI network */
String WifiSsid;

/* WIFI password */
String WifiPass;



CellarConfigManagement::CellarConfigManagement() {
	_EE_WIFI_SSID_ADDR = 10;
	_EE_WIFI_SSID_SIZE = 32;

	_EE_WIFI_PASS_ADDR = 42;
	_EE_WIFI_PASS_SIZE = 32;
	_EE_DEVICE_NAME_ADDR = 74;
	_EE_DEVICE_NAME_SIZE = 32;

	_EE_MQTT_SERVER_ADDR = 106;
	_EE_MQTT_SERVER_SIZE = 32;

	_EE_MQTT_TOPIC_ADDR = 138;
	_EE_MQTT_TOPIC_SIZE = 32;

	_EE_MQTT_USER_NAME_ADDR = 170;
	_EE_MQTT_USER_NAME_SIZE = 20;

	_EE_MQTT_USER_PASS_ADDR = 190;
	_EE_MQTT_USER_PASS_SIZE = 32;

	_EE_MQTT_PORT_ADDR = 222;
}

CellarConfigManagement::~CellarConfigManagement() {
	
}

/* reads all parameters from EEPROM */
void CellarConfigManagement::loadConfigurationFromEeprom() {
	CellarConfigManagement::WifiSsid = readTextFromEEPROM(_EE_WIFI_SSID_ADDR, _EE_WIFI_SSID_SIZE);
	delay(5);
	CellarConfigManagement::WifiPass = readTextFromEEPROM(_EE_WIFI_PASS_ADDR, _EE_WIFI_PASS_SIZE);
	delay(5);
	CellarConfigManagement::DeviceName = readTextFromEEPROM(_EE_DEVICE_NAME_ADDR, _EE_DEVICE_NAME_SIZE);
	delay(5);
	CellarConfigManagement::MqttHostName = readTextFromEEPROM(_EE_MQTT_SERVER_ADDR, _EE_MQTT_SERVER_SIZE);
	delay(5);
	CellarConfigManagement::MqttTopicName = readTextFromEEPROM(_EE_MQTT_TOPIC_ADDR, _EE_MQTT_TOPIC_SIZE);
	delay(5);
	CellarConfigManagement::MqttUserName = readTextFromEEPROM(_EE_MQTT_USER_NAME_ADDR, _EE_MQTT_USER_NAME_SIZE);
	delay(5);
	CellarConfigManagement::MqttUserPass = readTextFromEEPROM(_EE_MQTT_USER_PASS_ADDR, _EE_MQTT_USER_PASS_SIZE);
	delay(5);
	CellarConfigManagement::MqttPort = readIntFromEEPROM(_EE_MQTT_PORT_ADDR);
	delay(5);
}

/* saves all given parameters to EEPROM */
void CellarConfigManagement::saveConfigurationToEeprom(const char* wifiSsid, const char* wifiPass, const char* deviceName, const char* mqttHost, const char* mqttTopic, const char* mqttUserName, const char* mqttUserPass, int mqttPort) {
	writeTextToEEPROM(_EE_WIFI_SSID_ADDR, _EE_WIFI_SSID_SIZE, wifiSsid);
	delay(5);
	writeTextToEEPROM(_EE_WIFI_PASS_ADDR, _EE_WIFI_PASS_SIZE, wifiPass);
	delay(5);
	writeTextToEEPROM(_EE_DEVICE_NAME_ADDR, _EE_DEVICE_NAME_SIZE, deviceName);
	delay(5);
	writeTextToEEPROM(_EE_MQTT_SERVER_ADDR, _EE_MQTT_SERVER_SIZE, mqttHost);
	delay(5);
	writeTextToEEPROM(_EE_MQTT_TOPIC_ADDR, _EE_MQTT_TOPIC_SIZE, mqttTopic);
	delay(5);
	writeTextToEEPROM(_EE_MQTT_USER_NAME_ADDR, _EE_MQTT_USER_NAME_SIZE, mqttUserName);
	delay(5);
	writeTextToEEPROM(_EE_MQTT_USER_PASS_ADDR, _EE_MQTT_USER_PASS_SIZE, mqttUserPass);
	delay(5);
	writeIntToEEPROM(_EE_MQTT_PORT_ADDR, mqttPort);
	delay(5);
}




/************************************************************/
/*                   PRIVATE METHODS                        */
/************************************************************/

/* writes a string to EEPROM on given address */
void CellarConfigManagement::writeTextToEEPROM(int addr, int size, String s)
{
	EEPROM.begin(512);

	for (int i = 0; i < s.length(); i++)
	{
		EEPROM.write(addr + i, (byte)s[i]);
	}
	for (int i = s.length(); i < size; i++)
	{
		EEPROM.write(addr + i, ' ');
	}

	EEPROM.commit();
	EEPROM.end();
}

/* writes an integer to EEPROM on given address */
void CellarConfigManagement::writeIntToEEPROM(int addr, int val)
{
	EEPROM.begin(512);

	byte byte1 = val;
	byte byte2 = val >> 8;
	EEPROM.write(addr, byte1);
	EEPROM.write(addr + 1, byte2);

	EEPROM.commit();
	EEPROM.end();
}

/* reads a string from EEPROM on given address */
String CellarConfigManagement::readTextFromEEPROM(int addr, int size)
{
	EEPROM.begin(512);

	String TextFromEEPROM = "";

	for (int i = 0; i < size; i++)
	{
		char znak = (char)EEPROM.read(addr + i);
		TextFromEEPROM = TextFromEEPROM + znak;
	}
	while (TextFromEEPROM[TextFromEEPROM.length() - 1] == ' ')
	{
		TextFromEEPROM = TextFromEEPROM.substring(0, TextFromEEPROM.length() - 1);
	}

	EEPROM.end();
	
	return TextFromEEPROM;
}

/* reads an integer from EEPROM on given address */
int CellarConfigManagement::readIntFromEEPROM(int addr)
{
	EEPROM.begin(512);

	int val = EEPROM.read(addr) + ((EEPROM.read(addr + 1)) << 8);
	return val;

	EEPROM.end();
}