#include <iostream>
#include <math.h>
#include "XBoxController.h"
#include "XInputWrapper.h"

XINPUT_STATE State;
XINPUT_VIBRATION Vibration;
XINPUT_BATTERY_INFORMATION BatteryInformation;

GamePad::XBoxController::XBoxController(XInputWrapper * xinput, DWORD id)
{
	this->xinput = xinput;
	this->id = id;
}

DWORD GamePad::XBoxController::GetControllerId()
{
	return id;
}

bool GamePad::XBoxController::IsConnected()
{
	bool result;
	DWORD dwResult;
	ZeroMemory(&State, sizeof(XINPUT_STATE));
	dwResult = xinput->XInputGetState(id, &State);
	if (dwResult == ERROR_SUCCESS) {
		result = TRUE;
	}
	else {
		result = FALSE;
	}
	return result;
}

bool GamePad::XBoxController::PressedA()
{
	xinput->XInputGetState(id, &State);
	return (State.Gamepad.wButtons & XINPUT_GAMEPAD_A);
}

bool GamePad::XBoxController::PressedB()
{
	xinput->XInputGetState(id, &State);
	return (State.Gamepad.wButtons & XINPUT_GAMEPAD_B);
}

bool GamePad::XBoxController::PressedX()
{
	xinput->XInputGetState(id, &State);
	return (State.Gamepad.wButtons & XINPUT_GAMEPAD_X);
}

bool GamePad::XBoxController::PressedY()
{
	xinput->XInputGetState(id, &State);
	return (State.Gamepad.wButtons & XINPUT_GAMEPAD_Y);
}

bool GamePad::XBoxController::PressedLeftShoulder()
{
	xinput->XInputGetState(id, &State);
	return (State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
}

bool GamePad::XBoxController::PressedRightShoulder()
{
	xinput->XInputGetState(id, &State);
	return (State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
}

bool GamePad::XBoxController::PressedLeftDpad()
{
	xinput->XInputGetState(id, &State);
	return (State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
}

bool GamePad::XBoxController::PressedRightDpad()
{
	xinput->XInputGetState(id, &State);
	return (State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
}

bool GamePad::XBoxController::PressedUpDpad()
{
	xinput->XInputGetState(id, &State);
	return (State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
}

bool GamePad::XBoxController::PressedDownDpad()
{
	xinput->XInputGetState(id, &State);
	return (State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
}

bool GamePad::XBoxController::PressedStart()
{
	xinput->XInputGetState(id, &State);
	return (State.Gamepad.wButtons & XINPUT_GAMEPAD_START);
}

bool GamePad::XBoxController::PressedBack()
{
	xinput->XInputGetState(id, &State);
	return (State.Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
}

bool GamePad::XBoxController::PressedLeftThumb()
{
	xinput->XInputGetState(id, &State);
	return (State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
}

bool GamePad::XBoxController::PressedRightThumb()
{
	xinput->XInputGetState(id, &State);
	return (State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
}

BYTE GamePad::XBoxController::LeftTriggerLocation()
{
	xinput->XInputGetState(id, &State);
	return (State.Gamepad.bLeftTrigger);
}

BYTE GamePad::XBoxController::RightTriggerLocation()
{
	xinput->XInputGetState(id, &State);
	return (State.Gamepad.bRightTrigger);
}

GamePad::Coordinate GamePad::XBoxController::LeftThumbLocation()
{
	xinput->XInputGetState(id, &State);
	//determine the direction the controller is pushed
	float LX = State.Gamepad.sThumbLX;
	float LY = State.Gamepad.sThumbLY;

	float magnitude = sqrt(LX * LX + LY * LY);

	//initially set normalized values equal to original values
	float normalizedLX = LX;
	float normalizedLY = LY;

	float normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (magnitude > Deadzone)
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= Deadzone;

		//calcualte the scale between 0.0 to 1.0
		normalizedMagnitude = magnitude / (32767 - Deadzone);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
	}

	//multiply the scale to get normalized magnitude
	normalizedLX *= normalizedMagnitude;
	normalizedLY *= normalizedMagnitude;

	//construct an object type Coordinate to pass in the calculated values
	GamePad::Coordinate result(normalizedLX, normalizedLY);

	return result;
}

GamePad::Coordinate GamePad::XBoxController::RightThumbLocation()
{
	xinput->XInputGetState(id, &State);
	//determine the direction the controller is pushed
	float RX = State.Gamepad.sThumbRX;
	float RY = State.Gamepad.sThumbRY;

	float magnitude = sqrt(RX * RX + RY * RY);

	//initially set normalized values equal to original values
	float normalizedRX = RX;
	float normalizedRY = RY;

	float normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (magnitude > Deadzone)
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= Deadzone;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (32767 - Deadzone);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
	}
	//multiply the scale to get normalized magnitude
	normalizedRX *= normalizedMagnitude;
	normalizedRY *= normalizedMagnitude;

	//construct an object type Coordinate to pass in the calculated values
	GamePad::Coordinate result(normalizedRX, normalizedRY);
	return result;
}

void GamePad::XBoxController::Vibrate(WORD left, WORD right)
{
	Vibration.wLeftMotorSpeed = left;
	Vibration.wRightMotorSpeed = right;
	xinput->XInputSetState(id, &Vibration);
}

void GamePad::XBoxController::SetDeadzone(unsigned int radius)
{
	Deadzone = radius;
}

WORD GamePad::XBoxController::ButtonPressed()
{
	xinput->XInputGetState(id, &State);
	return State.Gamepad.wButtons;
}

void GamePad::XBoxController::SetVibrateButton(DWORD buttonPressed)
{
	VibrateButton = buttonPressed;
}


bool StartConsole(GamePad::XBoxController player)
{
	std::cout << "Controller " << player.GetControllerId();
	if (!player.IsConnected()) {
		std::cout << " ------------NOT FOUND------------" << std::endl;
		return FALSE;
	}
	else {
		std::cout << "| Button Pressed: ";
		int Button = 0;
		Button = player.PressedA() + player.PressedB() + player.PressedX() + player.PressedY() +
			player.PressedLeftShoulder() + player.PressedRightShoulder() + player.PressedLeftDpad() +
			player.PressedRightDpad() + player.PressedUpDpad() + player.PressedDownDpad() + player.PressedStart() + player.PressedBack() +
			player.PressedLeftThumb() + player.PressedRightThumb();
		if (Button == 0) {
			std::cout << "NONE" << " ";
		}
		else {
			if (player.PressedA()) { std::cout << "A" << " "; }
			if (player.PressedB()) { std::cout << "B" << " "; }
			if (player.PressedX()) { std::cout << "X" << " "; }
			if (player.PressedY()) { std::cout << "Y" << " "; }
			if (player.PressedLeftShoulder()) { std::cout << "LB" << " "; }
			if (player.PressedRightShoulder()) { std::cout << "RB" << " "; }
			if (player.PressedLeftDpad()) { std::cout << "LEFT" << " "; }
			if (player.PressedRightDpad()) { std::cout << "RIGHT" << " "; }
			if (player.PressedUpDpad()) { std::cout << "UP" << " "; }
			if (player.PressedDownDpad()) { std::cout << "DOWN" << " "; }
			if (player.PressedStart()) { std::cout << "START" << " "; }
			if (player.PressedBack()) { std::cout << "BACK" << " "; }
			if (player.PressedLeftThumb()) { std::cout << "LeftT" << " "; }
			if (player.PressedRightThumb()) { std::cout << "RightT" << " "; }
		}
		std::cout << "| LEFT_TRIG: " << (int)player.LeftTriggerLocation() << " " << "| RIGHT_TRIG: " << (int)player.RightTriggerLocation() << " ";
		std::cout << "| LEFT_STICK " << player.LeftThumbLocation().GetX() << " , " << player.LeftThumbLocation().GetY() << " " <<
			"| RIGHT_STICK " << player.RightThumbLocation().GetX() << " , " << player.RightThumbLocation().GetY() << " ";

		if (player.VibrateButton & player.ButtonPressed()) { 
			player.Vibrate(65000, 65000);
			std::cout << "VIBRATING" << std::endl;
		}
		else {
			player.Vibrate(0, 0);
			std::cout << "NOT VIBRATING" << std::endl;
		}
	}
	return TRUE;
}

int CheckConnection(GamePad::XBoxController &player, int &instance)
{
	if (player.IsConnected()) {
		std::cout << "Controller " << player.GetControllerId() << " is connected." << std::endl;
		instance++;
	}
	else {
		std::cout << "Controller " << player.GetControllerId() << " is not connected." << std::endl;
	}
	return instance;
}
