/*
 Name:		PirMotionSensorEsp32.ino
 Created:	1/19/2018 4:03:16 PM
 Author:	gibon
*/

#include <PubSubClient.h>
#include <EEPROM.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <FS.h>
#include "CellarConfigManagement.h"
#include "CellarWifiManagement.h"
#include "CellarESP32Utils.h"


PROGMEM const char htmlHeader[] = "<!DOCTYPE html><html lang=\"en\" xmlns=\"http://www.w3.org/1999/xhtml\"><head> <meta charset=\"utf-8\"/> <meta name=\"viewport\" content=\"width=device-width\" /> <title>WeMos AP - Config</title> <style type=\"text/css\"> html, body { margin: 0px; padding: 0px; width: 100%; height: 100%; font-family: Segoe UI; background-color: #cccccc; background: -moz-linear-gradient(top, #eeeeee 0%, #cccccc 100%); background: -webkit-linear-gradient(top, #eeeeee 0%, #cccccc 100%); background: linear-gradient(to bottom, #eeeeee 0%, #cccccc 100%); filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#eeeeee', endColorstr='#cccccc', GradientType=0); background-repeat: no-repeat; overflow: hidden; } .header { position: absolute; left: 0; top: 0; right: 0; width: 100%; height: 65px; margin: 0px; padding: 0px; background: #eeeeee; background: -moz-linear-gradient(top, #eeeeee 0%, #cccccc 99%, #595959 100%); background: -webkit-linear-gradient(top, #eeeeee 0%, #cccccc 99%, #595959 100%); background: linear-gradient(to bottom, #eeeeee 0%, #cccccc 99%, #595959 100%); filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#eeeeee', endColorstr='#595959', GradientType=0); } .wifi-strength-0 { list-style-image: url(\"data:image/svg+xml;utf8,<svg height='24' viewBox='0 0 48 48' width='24' xmlns='http://www.w3.org/2000/svg'><path d='M24.02 42.98l23.26-28.98c-.9-.68-9.85-8-23.28-8-13.43 0-22.38 7.32-23.28 8l23.26 28.98.02.02.02-.02z' fill-opacity='.3'/><path d='M0 0h48v48h-48z' fill='none'/></svg>\"); } .wifi-strength-1 { list-style-image: url(\"data:image/svg+xml;utf8,<svg height='24' viewBox='0 0 48 48' width='24' xmlns='http://www.w3.org/2000/svg'><path d='M24.02 42.98l23.26-28.98c-.9-.68-9.85-8-23.28-8s-22.38 7.32-23.28 8l23.26 28.98.02.02.02-.02z' fill-opacity='.3'/><path d='M0 0h48v48h-48z' fill='none'/><path d='M13.34 29.72l10.65 13.27.01.01.01-.01 10.65-13.27c-.53-.41-4.6-3.72-10.66-3.72s-10.13 3.31-10.66 3.72z'/></svg>\"); } .wifi-strength-2 { list-style-image: url(\"data:image/svg+xml;utf8,<svg height='24' viewBox='0 0 48 48' width='24' xmlns='http://www.w3.org/2000/svg'><path d='M24.02 42.98l23.26-28.98c-.9-.68-9.85-8-23.28-8s-22.38 7.32-23.28 8l23.26 28.98.02.02.02-.02z' fill-opacity='.3'/><path d='M0 0h48v48h-48z' fill='none'/><path d='M9.58 25.03l14.41 17.95.01.02.01-.02 14.41-17.95c-.72-.56-6.22-5.03-14.42-5.03s-13.7 4.47-14.42 5.03z'/></svg>\"); } .wifi-strength-3 { list-style-image: url(\"data:image/svg+xml;utf8,<svg height='24' viewBox='0 0 48 48' width='24' xmlns='http://www.w3.org/2000/svg'><path d='M24.02 42.98l23.26-28.98c-.9-.68-9.85-8-23.28-8s-22.38 7.32-23.28 8l23.26 28.98.02.02.02-.02z' fill-opacity='.3'/><path d='M0 0h48v48h-48z' fill='none'/><path d='M7.07 21.91l16.92 21.07.01.02.02-.02 16.92-21.07c-.86-.66-7.32-5.91-16.94-5.91-9.63 0-16.08 5.25-16.93 5.91z'/></svg>\"); } .wifi-strength-4 { list-style-image: url(\"data:image/svg+xml;utf8,<svg height='24' viewBox='0 0 48 48' width='24' xmlns='http://www.w3.org/2000/svg'><path d='M24.02 42.98l23.26-28.98c-.9-.68-9.85-8-23.28-8s-22.38 7.32-23.28 8l23.26 28.98.02.02.02-.02z'/><path d='M0 0h48v48h-48z' fill='none'/></svg>\"); } .header-container { width: 350px; height: 65px; margin: 0 auto; } .logo { height: 60px; margin-top: 3px; width: 70px; float: left; } .title-container { width: 280px; } .title { width: 100%; font-size: 1.5em; font-weight: bold; padding-left: 4.4em; color: #888888; } .subtitle { width: 100%; font-size: 1.1em; padding-left: 6.5em; color: #888888; } .main-container { width: 350px; margin: 0 auto; } .controls { width: 350px; margin: 70px auto 5px auto; text-align: center; } .button { height: 35px; width: 110px; font-size: 1.1em; color: #595959; } .config-form { } .form-line { margin-top: 10px; } .form-label { float: left; width: 140px; text-align: right; font-size: 1.1em; color: #595959; } .form-value { margin-left: 145px; } .form-control { width: 190px; font-size: 1.1em; color: #595959; } </style></head>";
PROGMEM const char htmlConfigForm[] = "<body> <input type=\"hidden\" id=\"hdnIsApMode\" value=\"{IsApModeValue}\" /> <input type=\"hidden\" id=\"hdnDeviceIp\" value=\"{DeviceIpValue}\" /><div class=\"header\"> <div class=\"header-container\"> <div class=\"logo\"> <svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\" viewBox=\"0 0 107.7 110.6\" style=\"enable-background: new 0 0 107.7 110.6; height: 58px; width: 70px;\" xml:space=\"preserve\"> <style type=\"text/css\"> .st0 { display: none; } .st1 { fill: #2B66B1; } .st2 { fill: #478FCC; } </style> <g id=\"Layer_1\" class=\"st0\"></g> <g id=\"Layer_2\"> <g> <path class=\"st1\" d=\"M52.3,110.5h-41c-7.6,0-11.7-3.6-11.7-10.1V10.1C-0.3,3.6,3.8,0,11.4,0h41c7.7,0,12,3.6,12,10.1V36H44.7V14.2 H19.3v82.1h25.5V73h19.6v27.3C64.4,106.9,60.1,110.5,52.3,110.5z\"/> </g> <g> <path class=\"st2\" d=\"M95.9,110.6H56.5c-7.5,0-11.8-3.6-11.8-10.1V73h19.5v23.6h24V75L49,43.6c-3.3-2.5-4.3-4.6-4.3-8.8V10.2 c0-6.5,4.3-10.1,11.8-10.1h39.4c7.5,0,11.8,3.6,11.8,10v24.7H88.2V14h-24v20l39.2,31.2c3.3,2.6,4.3,4.8,4.3,9v26.3 C107.7,106.9,103.4,110.6,95.9,110.6z\"/> </g> </g> </svg> </div> <div class=\"title-container\"> <div class=\"title\"> C e l l a r s t o n e </div> <div class=\"subtitle\"> ESP8266 configuration </div> </div> </div></div><div class=\"main-container\"> <div class=\"controls\"> <button class=\"button\" onclick=\"saveValues();\">Save</button> <button class=\"button\" onclick=\"scanWifi();\">Scan</button> <button class=\"button\" onclick=\"reset();\">Reset</button> </div> <div class=\"config-form\"> <div class=\"form-line\"> <div class=\"form-label\"> <label for=\"txtWifiSsid\">WiFi SSID:</label> </div> <div class=\"form-value\"> <input type=\"text\" class=\"form-control\" id=\"txtWifiSsid\" value=\"{WifiSsidValue}\"/> </div> </div> <div class=\"form-line\"> <div class=\"form-label\"> <label for=\"txtWifiPass\">WiFi pass:</label> </div> <div class=\"form-value\"> <input type=\"password\" class=\"form-control\" id=\"txtWifiPass\" value=\"{WifiPassValue}\"/> </div> </div> <div class=\"form-line\"> <div class=\"form-label\"> <label for=\"txtDeviceName\">Device name:</label> </div> <div class=\"form-value\"> <input type=\"text\" class=\"form-control\" id=\"txtDeviceName\" value=\"{DeviceNameValue}\" /> </div> </div> <div class=\"form-line\"> <div class=\"form-label\"> <label for=\"txtMqttServer\">MQTT server:</label> </div> <div class=\"form-value\"> <input type=\"text\" class=\"form-control\" id=\"txtMqttServer\" value=\"{MqttServerValue}\" /> </div> </div> <div class=\"form-line\"> <div class=\"form-label\"> <label for=\"txtMqttServer\">MQTT port:</label> </div> <div class=\"form-value\"> <input type=\"text\" class=\"form-control\" id=\"txtMqttPort\" value=\"{MqttPortValue}\"/> </div> </div> <div class=\"form-line\"> <div class=\"form-label\"> <label for=\"txtMqttTopic\">MQTT topic:</label> </div> <div class=\"form-value\"> <input type=\"text\" class=\"form-control\" id=\"txtMqttTopic\" value=\"{MqttTopicValue}\" /> </div> </div> <div class=\"form-line\"> <div class=\"form-label\"> <label for=\"txtMqttUser\">MQTT user:</label> </div> <div class=\"form-value\"> <input type=\"text\" class=\"form-control\" id=\"txtMqttUser\" value=\"{MqttUserValue}\" /> </div> </div> <div class=\"form-line\"> <div class=\"form-label\"> <label for=\"txtMqttPass\">MQTT pass:</label> </div> <div class=\"form-value\"> <input type=\"password\" class=\"form-control\" id=\"txtMqttPass\" value=\"{MqttPassValue}\" /> </div> </div> </div></div>";
PROGMEM const char htmlFooter[] = "<script type=\"text/javascript\">var DeviceIpAddress=document.getElementById('hdnDeviceIp').value; function saveValues(){var url=\"http://\" + DeviceIpAddress + \"/save?wifiSsid=\" + document.getElementById('txtWifiSsid').value + \"&wifiPass=\" + document.getElementById('txtWifiPass').value + \"&deviceName=\" + document.getElementById('txtDeviceName').value + \"&mqttServer=\" + document.getElementById('txtMqttServer').value + \"&mqttTopic=\" + document.getElementById('txtMqttTopic').value + \"&mqttUserName=\" + document.getElementById('txtMqttUser').value + \"&mqttUserPass=\" + document.getElementById('txtMqttPass').value + \"&mqttPort=\" + document.getElementById('txtMqttPort').value; /*httpGetAsync(url, onSaveValuesSuccess, onSaveValuesError);*/ location.href=url;}; function scanWifi(){/*var url=location.href + \"?action=scan\"; httpGetAsync(url, onScanSuccess, onScanError);*/ alert(DeviceIpAddress);}; function reset(){location.href=\"http://\" + DeviceIpAddress + \"/reset\";}; function onSaveValuesSuccess(responseText){alert('Configuration saved! The device will reset.');}; function onSaveValuesError(){alert('Save configuration failed!');}; function onScanSuccess(responseText){document.getElementById('wifiList').innerHTML=responseText;}; function onScanError(){}; function httpGetAsync(url, successCallback, errorCallback){var xmlHttp=new XMLHttpRequest(); xmlHttp.onreadystatechange=function(){if (xmlHttp.readyState==4 && xmlHttp.status==200) successCallback(xmlHttp.responseXml); if (xmlHttp.readyState==4 && xmlHttp.status==404) errorCallback();}; xmlHttp.open('GET', url, true); xmlHttp.send(null);};</script></body></html>";
PROGMEM const char htmlResetForm[] = "<body> <input type=\"hidden\" id=\"hdnIsApMode\" value=\"{IsApModeValue}\" /> <input type=\"hidden\" id=\"hdnDeviceIp\" value=\"{DeviceIpValue}\" /> <div class=\"header\"> <div class=\"header-container\"> <div class=\"logo\"> <svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\" viewBox=\"0 0 107.7 110.6\" style=\"enable-background: new 0 0 107.7 110.6; height: 58px; width: 70px;\" xml:space=\"preserve\"> <style type=\"text/css\"> .st0 { display: none; } .st1 { fill: #2B66B1; } .st2 { fill: #478FCC; }</style> <g id=\"Layer_1\" class=\"st0\"></g> <g id=\"Layer_2\"> <g> <path class=\"st1\" d=\"M52.3,110.5h-41c-7.6,0-11.7-3.6-11.7-10.1V10.1C-0.3,3.6,3.8,0,11.4,0h41c7.7,0,12,3.6,12,10.1V36H44.7V14.2 H19.3v82.1h25.5V73h19.6v27.3C64.4,106.9,60.1,110.5,52.3,110.5z\" /> </g> <g> <path class=\"st2\" d=\"M95.9,110.6H56.5c-7.5,0-11.8-3.6-11.8-10.1V73h19.5v23.6h24V75L49,43.6c-3.3-2.5-4.3-4.6-4.3-8.8V10.2 c0-6.5,4.3-10.1,11.8-10.1h39.4c7.5,0,11.8,3.6,11.8,10v24.7H88.2V14h-24v20l39.2,31.2c3.3,2.6,4.3,4.8,4.3,9v26.3 C107.7,106.9,103.4,110.6,95.9,110.6z\" /> </g> </g> </svg> </div> <div class=\"title-container\"> <div class=\"title\"> C e l l a r s t o n e </div> <div class=\"subtitle\"> ESP8266 configuration </div> </div> </div> </div> <div class=\"main-container\"> <div class=\"controls\"> <button class=\"button\" onclick=\"return false;\">Save</button> <button class=\"button\" onclick=\"return false;\">Scan</button> <button class=\"button\" onclick=\"return false;\">Reset</button> </div> <div style=\"margin: 0 auto; text-align: center;\"> R E S T A R T I N G&nbsp;&nbsp;D E V I C E<br/> P L E A S E&nbsp;&nbsp;W A I T </div> </div>";
PROGMEM const char htmlResetFooter[] = "<script type=\"text/javascript\"> var DeviceIpAddress = document.getElementById('hdnDeviceIp').value; setTimeout(function() { location.href = \"http://\" + DeviceIpAddress; }, 9000); </script></body></html>";


long lastMillis = 0;

AsyncWebServer server(80);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 

CellarConfigManagement configManagement;
CellarWifiManagement wifiManagement;
CellarESP32Utils espUtils;

// the setup function runs once when you press reset or power the board
void setup() {
	//init esp
	espUtils.init();

	delay(5);

	configManagement.loadConfigurationFromEeprom();
	
	delay(5);
	
	wifiManagement.connectToWifiOrCreateAp(configManagement.WifiSsid, configManagement.WifiPass);

	delay(5);

	setupWebServer();
}

// the loop function runs over and over again until power down or reset
void loop() {
	wifiManagement.checkWifiStatus();
}



//-----------------------------------------------------------------------------------------
/*
Nastaveni web serveru
*/
void setupWebServer() {
	server.begin();

	server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/plain", "Hello World");
	});

	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		Serial.println("");
		Serial.println("get root");
		request->send(200, "text/html", String(htmlHeader) + getConfigForm() + String(htmlFooter));
	});

	server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/plain", "Reset OK");
		delay(10000);
		ESP.restart();
	});

	server.on("/isAlive", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/plain", "I am alive!");
	});

	server.on("/save", HTTP_GET, [](AsyncWebServerRequest *request) {
		String wifiSsid = request->getParam("wifiSsid")->value();
		String wifiPass = request->getParam("wifiPass")->value();
		String deviceName = request->getParam("deviceName")->value();
		String mqttServer = request->getParam("mqttServer")->value();
		String mqttTopic = request->getParam("mqttTopic")->value();
		String mqttUserName = request->getParam("mqttUserName")->value();
		String mqttUserPass = request->getParam("mqttUserPass")->value();
		int mqttPort = request->getParam("mqttPort")->value().toInt();

		configManagement.saveConfigurationToEeprom(wifiSsid.c_str(), wifiPass.c_str(), deviceName.c_str(), mqttServer.c_str(), mqttTopic.c_str(), mqttUserName.c_str(), mqttUserPass.c_str(), mqttPort);
		
		request->send(200, "text/plain", "OK");
		delay(500);
		ESP.restart();
	});

	server.on("/scan", HTTP_GET, [](AsyncWebServerRequest *request) {
		//String wifiXml = getWifiNetworksXml();
		//		//webServer.send(200, "text/xml", wifiXml);
		//		////String wifiHtml = getWifiNetworksHtml();
		//		////webServer.send(200, "text/html", wifiHtml);
		//		//delay(500);
		//		return;
	});	
}

String getConfigForm() {
	String str = String(htmlConfigForm);
	str.replace("{WifiSsidValue}", configManagement.WifiSsid);
	str.replace("{WifiPassValue}", configManagement.WifiPass);
	str.replace("{DeviceNameValue}", configManagement.DeviceName);
	str.replace("{MqttServerValue}", configManagement.MqttHostName);
	str.replace("{MqttTopicValue}", configManagement.MqttTopicName);
	str.replace("{MqttUserValue}", configManagement.MqttUserName);
	str.replace("{MqttPassValue}", configManagement.MqttUserPass);
	str.replace("{MqttPortValue}", String(configManagement.MqttPort));
	str.replace("{IsApModeValue}", (wifiManagement.IsApMode) ? "true" : "false");
	str.replace("{DeviceIpValue}", wifiManagement.getIpAddress());

	return str;
}

/*
Nastaveni MQTT klienta
*/
void setupMqttClient() {
	Serial.println("");
	Serial.print("MqttHostName: ");
	Serial.print(configManagement.MqttHostName);
	Serial.println("");

	Serial.println("");
	Serial.print("MqttPort: ");
	Serial.print(configManagement.MqttPort);
	Serial.println("");
	//char* n = "192.168.1.234";
	mqttClient.setServer(configManagement.MqttHostName.c_str(), configManagement.MqttPort);
	mqttClient.setCallback(messageRecievedCallback);
}


/*
Do MQTT message queue posle zpravu, ze je senzor aktivni
*/
void sendActiveSensorMessage() {
	Serial.println("");
	long timeStamp = millis();



	if (millis() - timeStamp < 60000) {
		//pokud ubehlo vice nez 60 sekund, nema uz cenu zpravu posilat
		Serial.println("");
		Serial.print("MqttTopicName: ");
		Serial.print(configManagement.MqttTopicName);
		Serial.println("");
		mqttClient.publish(configManagement.MqttTopicName.c_str(), "1");
		Serial.println("Message sent!");
	}
}

/*
Posle heart beat zpravu
*/
void sendHeartBeatMessage() {

}

/*
Pripoji MQTT klienta, pokud neni pripojen

- bool infiniteLoop urcuje, zda bude pokus o pripojeni opakovat dokud se pripojeni nezdari

- return true, pokud se pripoji, false kdyz pokusy o pripojeni selzou
*/
bool connectMqttClientIfNotConnected(bool infiniteLoop) {
	long timeStamp = millis();
	//neni-li MQTT klient pripojen, budem se snazit pripojit
	while (!mqttClient.connected()) {
		Serial.println("Connecting to mqtt server.");

		//mqttClient.connect(DeviceName.c_str(), MqttUserName.c_str(), MqttUserPass.c_str());
		Serial.println("");
		Serial.print("DeviceName: ");
		Serial.print(configManagement.DeviceName);
		Serial.println("");
		mqttClient.connect(configManagement.DeviceName.c_str(), configManagement.MqttUserName.c_str(), configManagement.MqttUserPass.c_str());
		delay(100);
		if (mqttClient.connected()) {
			Serial.println("Mqtt client connected!");
		}
		else {
			//pokud se nepodari pripojit, poslem na serial kod chyby, pockame 1 sek. a budem pokus opakovat
			Serial.println("Mqtt client connection failed!");
			Serial.println("");
			Serial.println(mqttClient.state());
			delay(1000);
		}

		//neni-li receno, ze ma byt infinite smycka, tak to po 15 sek vzdame
		if (!infiniteLoop && millis() - timeStamp > 15000) {
			return false;
		}
	}
	return true;
}

/*
Zpracuje signal z PIR senzoru
odpoji naslouchani preruseni -> posle zpravu do fronty -> pripoji zpet naslouchani preruseni
*/
void handlePirInterupt() {
	//pri zachyceni preruseni ho hned odpojime, aby se to nevolalo vicekrat
	detachInterrupt(5);

	Serial.println("Sensor ACTIVE!!!");

	//posleme zpravu do MQTT fronty
	sendActiveSensorMessage();

	//opet zaregistrujem zachytavani preruseni
	attachInterrupt(5, handlePirInterupt, RISING);
}

/*
Callback funkce, jez se zavola pri prichozi zprave z MQTT
*/
void messageRecievedCallback(char* topic, byte* payload, unsigned int length) {

	// TODO (if needed...)
}
