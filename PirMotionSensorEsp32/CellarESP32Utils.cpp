#include "CellarESP32Utils.h"

CellarESP32Utils::CellarESP32Utils() {

}

CellarESP32Utils::~CellarESP32Utils() {

}

/* start serial communication and switch the onboard LED off */
void CellarESP32Utils::init() {
	Serial.begin(115200);
	delay(500);
	Serial.println("");
	Serial.println("Cellarstone ESP32 is starting");

	pinMode(BUILTIN_LED, OUTPUT);
	
	CellarESP32Utils::switchOnBoardLedOff();
}

/* blink the onboard LED by the given parameters */
void CellarESP32Utils::blinkOnBoardLed(int highMillis, int lowMillis, int cycles) {
	CellarESP32Utils::switchOnBoardLedOff();

	for (int i = 0; i < cycles; i++) {
		CellarESP32Utils::blink(highMillis, lowMillis);
	}
}

void CellarESP32Utils::blinkOnBoardLedOnce(int millis) {
	CellarESP32Utils::switchOnBoardLedOn();
	delay(millis);
	CellarESP32Utils::switchOnBoardLedOff();
	delay(millis);
}

void CellarESP32Utils::switchOnBoardLedOn() {
	digitalWrite(BUILTIN_LED, LOW);
}

void CellarESP32Utils::switchOnBoardLedOff() {
	digitalWrite(BUILTIN_LED, HIGH);
}

void CellarESP32Utils::blink(int highMillis, int lowMillis) {
	digitalWrite(BUILTIN_LED, HIGH);

	digitalWrite(BUILTIN_LED, LOW);
	delay(highMillis);
	digitalWrite(BUILTIN_LED, HIGH);
	delay(lowMillis);
}