/*
 Name:		DC_PID_motor.ino
 Created:	15.01.2019 19:22:27
 Author:	Mateusz Kotasinski
*/

#include <PID_v1.h>
#include "motorShieldL298P.h"

const byte encoder0pinA = 2; //A pin -> the interrupt pin 0
const byte encoder0pinB = 3; //B pin -> the digital pin 3
int M_pwm = 3; //The enabling of motor driver board connection to the digital interface responsible for pwm signal
int M_dir = 12; //The enabling of L298PDC motor driver board connection to the digital interface responsible for direction signal
int M_break = 9; //The enabling of L298PDC motor driver board connection to the digital interface responsible for break signal
byte encoder0PinALast;
double duration, abs_duration;//the number of the pulses
boolean Direction; //the rotation direction 
boolean result;

double val_output; //Power supplied to the motor PWM value.
double Setpoint;
double Kp = 0.6, Ki = 5, Kd = 0;
PID myPID(&abs_duration, &val_output, &Setpoint, Kp, Ki, Kd, DIRECT);
Engine engine;

void setup()
{
	Serial.begin(9600); //Initialize the serial port
	pinMode(M_dir, OUTPUT);   //Control port settings DC motor driver board for the output mode
	pinMode(M_pwm, OUTPUT);
	Setpoint = 80;  //Set the output value of the PID
	myPID.SetMode(AUTOMATIC); //PID is set to automatic mode
	myPID.SetSampleTime(100); //Set PID sampling frequency is 100ms
	EncoderInit(); //Initialize the module
}

void loop()
{
	engine.getDataFromPC();
	Setpoint = engine.speed;
	engine.motorMove(val_output);

	abs_duration = abs(duration);
	result = myPID.Compute(); //PID conversion is complete and returns 1
	if (result)
	{
		Serial.print("<");
		Serial.print((int)duration*(-1));
		Serial.print(">\n");
		duration = 0; //Count clear, wait for the next count
	}


}

void EncoderInit()
{
	Direction = true; //default -> Forward  
	pinMode(encoder0pinB, INPUT);
	attachInterrupt(0, wheelSpeed, CHANGE);
}

void wheelSpeed()
{
	int Lstate = digitalRead(encoder0pinA);
	if ((encoder0PinALast == LOW) && Lstate == HIGH)
	{
		int val = digitalRead(encoder0pinB);
		if (val == LOW && Direction)
		{
			Direction = false; //Reverse
		}
		else if (val == HIGH && !Direction)
		{
			Direction = true;  //Forward
		}
	}
	encoder0PinALast = Lstate;

	if (!Direction)  duration++;
	else  duration--;

}