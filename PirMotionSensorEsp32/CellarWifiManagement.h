#ifndef CELLAR_WIFI_MANAGEMENT_h
#define CELLAR_WIFI_MANAGEMENT_h

#include <Arduino.h>
#include <WiFi.h>

class CellarWifiManagement {
private:
	String _wifiSsid;
	String _wifiPass;
	char* _apName;
	char* _apPass;
	int _connectionTimeout;

public:
	bool IsApMode;

	CellarWifiManagement();
	~CellarWifiManagement();

	void connectToWifiOrCreateAp(String wifiSsid, String wifiPass);
	String getIpAddress();
	void checkWifiStatus();

private:
	void createAp();
	bool connect(int interval, bool infinite);

};

#endif