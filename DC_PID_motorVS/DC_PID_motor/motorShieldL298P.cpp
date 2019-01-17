#include "motorShieldL298P.h"


Engine::Engine()
{

	pinMode(12, OUTPUT); //Initiates Motor Channel A pin
	pinMode(9, OUTPUT); //Initiates Brake Channel A pin
	start = 0;
	pwm = 0;
	dir = 0;

}


Engine::~Engine()
{
}


void Engine::motorMove(double pwm) {

	if (start == 0) {
		digitalWrite(9, HIGH);
	}
	else {
		digitalWrite(9, LOW);
		analogWrite(3, pwm);
	}
	if (dir == 0) {
		digitalWrite(12, HIGH);
	}
	else {
		digitalWrite(12, LOW);
	}

}

void Engine::getDataFromPC() {

	while (Serial.available() > 0) {

		char x = Serial.read();

		// the order of these IF clauses is significant

		if (x == this->endMarker) {
			this->readInProgress = false;
			this->inputBuffer[this->bytesRecvd] = 0;
			parseData();
		}

		if (this->readInProgress) {
			this->inputBuffer[this->bytesRecvd] = x;
			this->bytesRecvd++;
			if (this->bytesRecvd == this->buffSize) {
				this->bytesRecvd = this->buffSize - 1;
			}
		}

		if (x == this->startMarker) {
			this->bytesRecvd = 0;
			this->readInProgress = true;
		}
	}
}

// This function split data into its parts
void Engine::parseData() {

	char * strtokIndx; // this is used by strtok() as an index

	strtokIndx = strtok(inputBuffer, ",");
	this->start = atoi(strtokIndx);

	strtokIndx = strtok(NULL, ",");
	this->speed = atoi(strtokIndx);

	strtokIndx = strtok(NULL, ",");
	this->dir = atoi(strtokIndx);
}