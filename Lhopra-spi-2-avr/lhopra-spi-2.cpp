/**
 *
 * Simple application qui attend des saisies sur un UART et
 * transf�re ces saisies dans un canal SPI. Les pins standards
 * �tant utilis�s pour les branchements mat�riels, il y � tr�s
 * peu a faire ici.
 *
 */

#include "arduino/Arduino.h"
#include "arduino/libraries/SPI/SPI.h"

void setup() {

	// pour les phase & polarit� de l'horloge
	SPI.setDataMode(SPI_MODE0);

	// par d�faut, mais re-configur�
	SPI.setBitOrder(MSBFIRST);

	// SPI_CLOCK_DIV8 nous donne une frequence de 2Mhz
	// puisque la carte tourne a 16Mhz
	SPI.setClockDivider(SPI_CLOCK_DIV8);

	// activation des pins
	SPI.begin();

	// activation du lien UART via la connexion USB
	Serial.begin(115200);

	// purge UART
	while (Serial.available()) {
		Serial.read();
	}

	// message d'info pour l'utilisateur
	Serial.println("Ready to read!");
}

void loop() {

	// transf�re d�s qu'un caract�re est lisible sur l'UART
	while (Serial.available()) {

		char inChar = (char) Serial.read();

		Serial.print("Sending : ");
		Serial.println(inChar);

		// transfer
		digitalWrite(SS, LOW);
		SPI.transfer(inChar);
		digitalWrite(SS, HIGH);

	}
}
