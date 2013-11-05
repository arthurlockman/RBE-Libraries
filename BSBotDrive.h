#ifndef BSBOTDRIVE_H
#define BSBOTDRIVE_H

#include "Arduino.h"

class BSBotDrive
{
public:
	BSBotDrive(int frontRightPin, int frontLeftPin, 
		int rearRightPin, int rearLeftPin);
	//! Drives the robot using an XBox controller
	void ArcadeDrive (FRCXboxJoystick& controller);
	//! Drives the robot using move and rotate values
	void ArcadeDrive (float moveValue, float rotateValue, 
		bool squaredInputs);
	//! Stops the robot
	void Stop();

	Servo frontRightMotor;
    Servo frontLeftMotor;
    Servo rearRightMotor;
    Servo rearLeftMotor;
private:
	void PowerMotors (float frontLeft, float frontRight);
	void PowerMotors (float frontLeft, float rearLeft, 
		float frontRight, float rearRight);
};

#endif