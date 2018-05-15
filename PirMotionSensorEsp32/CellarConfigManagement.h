#ifndef CELLAR_CONFIG_MANAGEMENT_h
#define CELLAR_CONFIG_MANAGEMENT_h

#include <Arduino.h>
#include <EEPROM.h>

class CellarConfigManagement {
private:
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


public:
	/* IP adresa MQTT serveru */
	String MqttHostName;

	/* Port MQTT serveru */
	int MqttPort;

	/* uzivatel MQTT serveru */
	String MqttUserName;

	/* heslo MQTT uzivatele */
	String MqttUserPass;

	/* nazev MQTT fronty */
	String MqttTopicName;

	/* nazev zarizeni */
	String DeviceName;
	
	/* nazev wifi site */
	String WifiSsid;

	/* wifi heslo */
	String WifiPass;


	CellarConfigManagement();
	~CellarConfigManagement();


	void loadConfigurationFromEeprom();
	void saveConfigurationToEeprom(const char* wifiSsid, const char* wifiPass, const char* deviceName, const char* mqttHost, const char* mqttTopic, const char* mqttUserName, const char* mqttUserPass, int mqttPort);

	String loadWifiSsid();
	void saveWifiSsid(String val);

	String loadWifiPassword();
	void saveWifiPassword(String val);

	String loadMqttHostName();
	void saveMqttHostName(String val);

	String loadMqttUserName();
	void saveMqttUserName(String val);

	String loadMqttPassName();
	void saveMqttPassName(String val);

	String loadDeviceName();
	void saveDeviceName(String val);

	String loadAccessPointName();
	void saveAccessPointName(String val);

	int loadMqttPort();
	void saveMqttPort(int val);

private:
	void writeTextToEEPROM(int addr, int size, String s);
	void writeIntToEEPROM(int addr, int val);
	String readTextFromEEPROM(int addr, int size);
	int readIntFromEEPROM(int addr);
};


#endif