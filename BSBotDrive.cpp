#include "BSBotDrive.h"
#include <Math.h>

namespace 
{
	const float wheelBase = 13.0;
	const float wheelTrack = 10.0;
	const float wheelRatio = wheelBase / wheelTrack;
	const float bigWheelDiameter = 3.0;
	const float mecanumWheelDiameter = 5.5;

	/**
	 * @brief Returns the square of the val preserving the sign of val
	 *
	 * Exmaple SignedSquare (2) == 4 and SignedSquare (-2) == -4
	 */
	inline float SignedSquare (float val)
	{
		return val < 0 ? - (val * val) : (val * val);
	}

	enum joystickAxis 
	{
		rightStickX = 1,
		rightStickY = 2,
		leftStickY = 3,
		leftStickX = 4
	}
};

BSBotDrive::BSBotDrive(int frontRightPin, int frontLeftPin, 
		int rearRightPin, int rearLeftPin)
{
	frontRightMotor.attach(frontRightPin);
	frontLeftMotor.attach(frontLeftPin);
	rearRightMotor.attach(rearRightPin);
	rearLeftMotor.attach(rearRightPin);

	this->PowerMotors(0.0,0.0);
	this->Stop();
}

void BSBotDrive::ArcadeDrive(PPM &controller)
{
	float rotateValue = controller.getChannel(rightStickx);
	float moveValue = contoller.getChannel(leftStickY);
	this->ArcadeDrive((moveValue - 90), (rotateValue - 90), true);
}

void BSBot::ArcadeDrive (float moveValue, float rotateValue, 
		bool squaredInputs)
{
	float left;
	float right;

	if (squaredInputs)
	{
		moveValue = SignedSquare(moveValue);
		rotateValue = SignedSquare(moveValue);
	}

	if (moveValue >= 0.0)
	{
		if (rotateValue > 0.0)
		{
			left = moveValue - rotateValue;
			right = max (moveValue, rotateValue);
		}
		else
		{
			left = max (moveValue, -rotateValue);
			right = moveValue + rotateValue;
		}
	}
	else
	{
		if (rotateValue > 0.0)
		{
			left = -max (-moveValue, rotateValue);
			right = moveValue + rotateValue;
		}
		else
		{
			left = moveValue - rotateValue;
			right = -max (-moveValue, -rotateValue);
		}
	}
	this->PowerMotors(left, right);
}

void BSBotDrive::Stop()
{
	PowerMotors(0.0, 0.0, 0.0, 0.0);
}

void BSBotDrive::PowerMotors(float frontLeft, float frontRight)
{
		// Calculate the desired rear mecanum speeds.
	float rearLeft  = frontLeft  * 
		(0.5 + wheelRatio) + frontRight * (0.5 - wheelRatio);
	float rearRight = frontRight * 
		(0.5 + wheelRatio) + frontLeft  * (0.5 - wheelRatio);

	// Scale the rear speeds down if either of them exceed 1
	const float maxSpeed = max (fabs (rearLeft), fabs (rearRight));
	if (maxSpeed > 1.0)
	{
		rearLeft /= maxSpeed;
		rearRight /= maxSpeed;
	}

	this->PowerMotors (frontLeft, rearLeft, frontRight, rearRight);
}

void BSBotDrive::PowerMotors (
		float frontLeft,
		float rearLeft,
		float frontRight,
		float rearRight)
{
	rearRightMotor.write(180 - (rearRight + 90));
	rearLeftMotor.write(rearLeft + 90);
	frontRightMotor.write(180 - (frontRight + 90));
	frontLeftMotor.write(frontLeft + 90);
}

