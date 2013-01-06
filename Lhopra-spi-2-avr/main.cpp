#include "arduino/Arduino.h"
#include "arduino/libraries/SPI/SPI.h"

const int ledPin = 13;

void setup() {

	pinMode(ledPin, OUTPUT);

	SPI.setDataMode(SPI_MODE0);

	SPI.setBitOrder(MSBFIRST);

	SPI.setClockDivider(SPI_CLOCK_DIV8);

	SPI.begin();

	Serial.begin(115200);

	// purge UART
	while (Serial.available()) {
		Serial.read();
	}

	delay(2000);
}

int main(void) {

	setup();

	for (;;) {
		while (Serial.available()) {

			char inChar = (char) Serial.read();

			digitalWrite(ledPin, HIGH);
			// transfer
			digitalWrite(SS, LOW);
			SPI.transfer(inChar);
			digitalWrite(SS, HIGH);

			digitalWrite(ledPin, LOW);

		}
	}
	return 0;
}
