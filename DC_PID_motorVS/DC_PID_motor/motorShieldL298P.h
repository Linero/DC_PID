#pragma once
#include <Arduino.h>

class Engine
{

public:
	int speed = 80;

private:
	int delaylegnth;
	int start;
	int pwm;
	int dir;
	const byte buffSize = 45;
	char inputBuffer[45];
	const char startMarker = '<';
	const char endMarker = '>';
	const char cmdMarker = '#';
	const char cmdEndMarker = '$';
	byte bytesRecvd = 0;
	boolean readInProgress = false;
	String inputString;         // a string to hold incoming data
	boolean stringComplete = false;  // whether the string is complete
	String commandString;
	int inputValue;

	

public:
	Engine();
	~Engine();
	void motorMove(double);
	void getDataFromPC();


private:
	void parseData();
};