/*
 * main.cpp
 *
 *  Created on: 4 nov. 2012
 *      Author: Aur�lien Labrosse <aurelien.labrosse at free.fr>
 */

#include "arduino/Arduino.h"
#include "arduino/libraries/SPI/SPI.h"


const int ledPin = 13;

/* Write an int as a {0,1} string
 *
 * buffer must have length >= sizeof(int) + 1
 * Write to the buffer backwards so that the binary representation
 * is in the correct order i.e.  the LSB is on the far right
 * instead of the far left of the printed string
 *
 */
#define BUF_SIZE 9
char *int2bin(int a, char *buffer, int buf_size) {
	buffer += (buf_size - 1);

	for (int i = 7; i >= 0; i--) {
		*buffer-- = (a & 1) + '0';

		a >>= 1;
	}

	return buffer;
}

/**
 * Convenient method used while configuring system clock
 */
void dumpInfo() {

	Serial.print("--- Mega2560 @ ");
	Serial.print(F_CPU / 1000000);
	Serial.println(" Mhz --- ");

	// clock management register
	char buffer[BUF_SIZE];

	buffer[BUF_SIZE - 1] = '\0';
	Serial.print("CLKPR (CLOCK MNGMT)");
	Serial.print(" : [");
	int2bin(CLKPR, buffer, BUF_SIZE - 1);
	Serial.print(buffer);
	Serial.println("]");

	buffer[BUF_SIZE - 1] = '\0';
	Serial.print("SPCR (SPI)");
	Serial.print(" : [");
	int2bin(SPCR, buffer, BUF_SIZE - 1);
	Serial.print(buffer);
	Serial.println("]");

	buffer[BUF_SIZE - 1] = '\0';
	Serial.print("SPSR (SPI)");
	Serial.print(" : [");
	int2bin(SPSR, buffer, BUF_SIZE - 1);
	Serial.print(buffer);
	Serial.println("]");

	buffer[BUF_SIZE - 1] = '\0';
	Serial.print("SPDR (SPI)");
	Serial.print(" : [");
	int2bin(SPDR, buffer, BUF_SIZE - 1);
	Serial.print(buffer);
	Serial.println("]");

}

void setup() {


	pinMode(ledPin, OUTPUT);

	// re-set the default configuration
	SPI.setDataMode(SPI_MODE0); //          SPCR = (SPCR & ~SPI_MODE_MASK) | SPI_MODE0;

	SPI.setBitOrder(MSBFIRST); //           SPCR &= ~(_BV(DORD));

	SPI.setClockDivider(SPI_CLOCK_DIV8); // SPCR = (SPCR & ~SPI_CLOCK_MASK) | (SPI_CLOCK_DIV8 & SPI_CLOCK_MASK);
										 // SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((SPI_CLOCK_DIV8 >> 2) & SPI_2XCLOCK_MASK);

	SPI.begin(); // other pins configuration (OUTPUTs, default Slave Select)

	Serial.begin(115200);

	// purge UART
	while (Serial.available()) {
		Serial.read();
	}

	delay(1000);
	dumpInfo();
}

void serialEvent() {

	while (Serial.available()) {

		// get the new byte
		char inChar = (char) Serial.read();

		char buffer[BUF_SIZE];

		buffer[BUF_SIZE - 1] = '\0';
		int2bin(inChar, buffer, BUF_SIZE - 1);
		Serial.print("read : ");
		Serial.println(buffer);

		// transfer
		digitalWrite(SS, LOW);
		SPDR = inChar;
		while (!(SPSR & (1 << SPIF)))
			;

		SPDR = 0xF; // send dummy to ensure char has been
		// sent. see datasheet p.196 :
		//"The last incoming byte will be kept in the Buffer Register for later use."
		while (!(SPSR & (1 << SPIF)))
			;

		digitalWrite(SS, HIGH);

	}
}

void loop() {

}

int main(void) {
	init();

	setup();

	for (;;) {
		loop();
		if (serialEventRun)
			serialEventRun();
	}

	return 0;
}
