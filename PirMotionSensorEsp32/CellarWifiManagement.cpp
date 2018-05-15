/************* CellarWifiManagement.cpp *************/

#include "CellarWifiManagement.h"
#include "CellarESP32Utils.h"

//String _wifiSsid;
//String _wifiPass;
//
//char* _apName;
//char* _apPass;
//int _connectionTimeout;
//
//bool IsApMode;

CellarESP32Utils _espUtils;

CellarWifiManagement::CellarWifiManagement() {
	//set default timeout to 15 sec
	_connectionTimeout = 15000;

	//set default access point name to ESP32_AP
	_apName = "ESP32_AP";

	//set defalt access point password to Fibonacci
	_apPass = "11235813";
}

CellarWifiManagement::~CellarWifiManagement() {

}


void CellarWifiManagement::connectToWifiOrCreateAp(String wifiSsid, String wifiPass) {
	CellarWifiManagement::_wifiSsid = wifiSsid;
	CellarWifiManagement::_wifiPass = wifiPass;
	
	if (_wifiSsid == "" || !CellarWifiManagement::connect(_connectionTimeout, false)) {
		CellarWifiManagement::createAp();
	}
}



String CellarWifiManagement::getIpAddress() {
	return (CellarWifiManagement::IsApMode) ? WiFi.softAPIP().toString() : WiFi.localIP().toString();
}

void CellarWifiManagement::checkWifiStatus() {
	if (!CellarWifiManagement::IsApMode) {		
		if (WiFi.status() != WL_CONNECTED) {
			Serial.println("");
			Serial.print("WIFI connection to "); 
			Serial.print(CellarWifiManagement::_wifiSsid);
			Serial.print(" lost. Will try to reconect...");
			Serial.println("");
			
			WiFi.disconnect();

			CellarWifiManagement::connect(6000, true);
		}
	}
}

void CellarWifiManagement::createAp() {
	Serial.println("");
	Serial.println("Starting Access Point...");

	//spustime rezim Access Pointu
	WiFi.mode(WIFI_AP);

	////nastavime MAC adresu
	//uint8_t mac[WL_MAC_ADDR_LENGTH];
	//WiFi.softAPmacAddress(mac);
	
	////z MAC adresy udelame unikatni suffix pro nazev AP
	//String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
	//	String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
	//macID.toUpperCase();

	//WiFi.softAP((AccessPointName + macID).c_str(), DEFAULT_AP_PASS);

	WiFi.softAP(_apName, _apPass);

	Serial.println("");
	Serial.print("Access Point ");
	Serial.print(_apName);
	Serial.print(" started!");
	Serial.println("");
	Serial.print("IP address: ");
	Serial.print(WiFi.softAPIP());
	Serial.println("");
	Serial.print("AP host name: ");
	Serial.print(WiFi.softAPgetHostname());

	CellarWifiManagement::IsApMode = true;
}

bool CellarWifiManagement::connect(int interval, bool infinite) {
	//rezim clienta (stanice)
	WiFi.mode(WIFI_STA);
	
	Serial.println("");
	Serial.print("Connecting to ");
	Serial.print(CellarWifiManagement::_wifiSsid);

	//pripojime se k wifi
	WiFi.begin(CellarWifiManagement::_wifiSsid.c_str(), CellarWifiManagement::_wifiPass.c_str());

	long lastMillis = millis();
	//behem pripojovani budem blikat diodou - po pripojeni zustane sviti
	while (WiFi.status() != WL_CONNECTED) {
		_espUtils.blinkOnBoardLedOnce(50);

		if ((millis() - lastMillis) > interval) {
			if (infinite) {
				WiFi.disconnect();
				delay(100);
				WiFi.begin(CellarWifiManagement::_wifiSsid.c_str(), CellarWifiManagement::_wifiPass.c_str());
				Serial.println("");
				Serial.println("Connecting...");
				lastMillis = millis();
			}
			else {
				_espUtils.switchOnBoardLedOff();
				Serial.println("");
				Serial.print("Can not connect to ");
				Serial.print(CellarWifiManagement::_wifiSsid);

				return false;
			}
		}
	}
	Serial.println("");
	Serial.println("Connected!");
	Serial.println("");
	Serial.print("IP address: ");
	Serial.print(WiFi.localIP());
	CellarWifiManagement::IsApMode = false;
	_espUtils.switchOnBoardLedOn();

	return true;
}