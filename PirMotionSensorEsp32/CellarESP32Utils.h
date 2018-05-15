#ifndef CELLAR_ESP32_UTILS_h
#define CELLAR_ESP32_UTILS_h

#include <Arduino.h>

class CellarESP32Utils {
public:
	CellarESP32Utils();
	~CellarESP32Utils();

	void init();
	void blinkOnBoardLed(int highMillis, int lowMillis, int cycles);
	void blinkOnBoardLedOnce(int millis);
	void switchOnBoardLedOn();
	void switchOnBoardLedOff();

private:
	void blink(int highMillis, int lowMillis);
};

#endif